#include "DeliveryManager.h"
#include <algorithm>
#include <iostream>
#include "Package.h"
#include "Truck.h"

// 물류 배열을 우선순위 내림차순으로 정렬하는 함수
void DeliveryManager::sortPackagesByPriority(Package** packages, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            if (packages[j] == nullptr || packages[j+1] == nullptr) {
                continue;
            }

            // 뒤에 있는 객체의 우선순위가 더 크면 위치를 바꿈 (내림차순)
            if (packages[j]->getCurrentPriority() < packages[j+1]->getCurrentPriority()) {
                // 포인터(주소)의 위치만 교환 (Swapping)
                Package* temp = packages[j];
                packages[j] = packages[j+1];
                packages[j+1] = temp;
            }
        }
    }
}

// DP를 활용한 최적 적재 알고리즘
void DeliveryManager::dispatchOptimalPackages(Truck* truck, Package** waitingQueue, int queueSize) {
    // 무게 정수화 (스케일링: x10)
    int W = static_cast<int>(truck->getRemainingCapacity() * 10.0);
    int n = queueSize;

    if (W <= 0 || n <= 0) return;

    // 2차원 DP 테이블 동적 할당 (dp[i][w])
    // dp[i][w] = i번째 물류까지 고려했을 때, 가용 용량 w에서 얻을 수 있는 최대 우선순위 합
    int** dp = new int*[n + 1];
    for (int i = 0; i <= n; ++i) {
        dp[i] = new int[W + 1]{0}; // 0으로 초기화
    }

    // DP 테이블 채우기 (Bottom-Up)
    for (int i = 1; i <= n; ++i) {
        Package* currentPkg = waitingQueue[i - 1];

        // 이미 트럭에 실렸거나 배송 완료된 물류는 패스 (nullptr 처리된 경우)
        if (currentPkg == nullptr || currentPkg->getStatus() != DeliveryStatus::WAITING) {
            for (int w = 0; w <= W; ++w) {
                dp[i][w] = dp[i - 1][w];
            }
            continue;
        }

        int w_i = static_cast<int>(currentPkg->getWeight() * 10.0);
        int v_i = currentPkg->getCurrentPriority();

        for (int w = 0; w <= W; ++w) {
            if (w_i <= w) {
                // 현재 물류를 실을 수 있는 경우: (싣지 않는 경우 vs 싣는 경우의 가치) 중 최댓값
                dp[i][w] = std::max(dp[i - 1][w], dp[i - 1][w - w_i] + v_i);
            } else {
                // 현재 물류가 너무 무거워서 실을 수 없는 경우
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // 역추적을 통해 실제로 트럭에 실을 물류 찾아내기
    int currentW = W;
    for (int i = n; i > 0; --i) {
        if (dp[i][currentW] != dp[i - 1][currentW]) {
            // 값이 다르다는 것은 i-1번째 물류가 배낭(트럭)에 포함되었다는 뜻
            Package* selectedPkg = waitingQueue[i - 1];
            truck->loadPackage(selectedPkg);
            selectedPkg->updateStatus(DeliveryStatus::DELIVERING); // 상태를 '배송 중'으로 변경

            currentW -= static_cast<int>(selectedPkg->getWeight() * 10.0);

            // 대기열에서 삭제 (적재되었으므로 nullptr로 처리)
            waitingQueue[i - 1] = nullptr;
        }
    }

    // 메모리 누수 방지 (DP 테이블 해제)
    for (int i = 0; i <= n; ++i) {
        delete[] dp[i];
    }
    delete[] dp;
}

void DeliveryManager::processTick(Truck** trucks, int truckCount, Package** waitingQueue, int queueSize) {
    cout << "\n========== [시스템 턴(Tick) 진행] ==========\n";

    // 배송 중인 화물 '완료' 처리 및 트럭 비우기
    for (int i = 0; i < truckCount; ++i) {
        Truck* truck = trucks[i];
        if (truck->getCurrentCount() > 0) {
            Package** loaded = truck->getLoadedPackages();
            for (int j = 0; j < truck->getCurrentCount(); ++j) {
                // 패키지 상태를 '배송 중(DELIVERING)'에서 '완료(COMPLETED)'로 전이
                loaded[j]->updateStatus(DeliveryStatus::COMPLETED);
                cout << "[배송 완료] " << loaded[j]->getName() << "이(가) 최종 목적지에 도착했습니다.\n";
            }
            // 화물을 모두 내렸으므로 트럭 용량 및 카운트 초기화
            truck->clearLoadedPackages();
            cout << "- " << (i + 1) << "호차가 복귀하여 배차 가능한 상태가 되었습니다.\n";
        }
    }

    // 대기열(WAITING)에 남은 물류들의 TTL 감소 연산
    cout << "--- [대기열 상태 갱신] ---\n";
    for (int i = 0; i < queueSize; ++i) {
        if (waitingQueue[i] != nullptr && waitingQueue[i]->getStatus() == DeliveryStatus::WAITING) {
            // 내부적으로 콜드체인인 경우에만 TTL을 깎도록 decreaseTTL()이 구현되어 있음
            waitingQueue[i]->decreaseTTL();

            if (waitingQueue[i]->getType() == DeliveryType::COLD_CHAIN) {
                cout << "- " << waitingQueue[i]->getName()
                          << "의 TTL이 감소했습니다. (현재 우선순위: "
                          << waitingQueue[i]->getCurrentPriority() << ")\n";
            }
        }
    }
}