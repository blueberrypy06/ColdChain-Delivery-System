#include <iostream>
#include <string>
#include "DeliveryManager.h"
#include "Package.h"
#include "FileHandler.h"
#include "Truck.h"
using namespace std;


int main() {
    cout << "========== [물류 배송 최적화 연속 시뮬레이션] ==========\n\n";

    int maxPackages = 20; // 물류 한도를 20개로 넉넉히 확장
    Package** initialPackages = new Package*[maxPackages];
    for (int i = 0; i < maxPackages; ++i) initialPackages[i] = nullptr;
    int count = 0;

    // 초기 데이터 세팅
    cout << "--- 1. 물류 데이터 생성 및 파일 저장 (Save) ---\n";
    initialPackages[count++] = new Package("P01", "신선육류", DeliveryType::COLD_CHAIN, 5.0, 20, 1);
    initialPackages[count++] = new Package("P02", "긴급의약품", DeliveryType::EXPRESS, 6.0, 120);
    initialPackages[count++] = new Package("P03", "고급전자기기", DeliveryType::NORMAL, 4.0, 100);
    initialPackages[count++] = new Package("P04", "일반서류", DeliveryType::NORMAL, 5.0, 80);
    initialPackages[count++] = new Package("P05", "생수묶음", DeliveryType::NORMAL, 3.0, 50);
    initialPackages[count++] = new Package("P06", "냉동수산물", DeliveryType::COLD_CHAIN, 6.0, 10, 4);
    initialPackages[count++] = new Package("P07", "건축자재", DeliveryType::NORMAL, 8.0, 40);
    initialPackages[count++] = new Package("P08", "특급우편", DeliveryType::EXPRESS, 1.0, 150);
    initialPackages[count++] = new Package("P09", "과일세트", DeliveryType::COLD_CHAIN, 3.0, 15, 3);
    initialPackages[count++] = new Package("P10", "가구", DeliveryType::NORMAL, 7.0, 60);

    string filename = "final_logistics.csv";
    if (FileHandler::savePackages(filename, initialPackages, count)) {
        cout << "[성공] " << filename << " 파일에 10개의 데이터 저장 완료.\n";
    }

    // 파일에 구웠으므로 원본 데이터 메모리 1차 삭제
    for (int i = 0; i < count; i++) delete initialPackages[i];
    delete[] initialPackages;


    // 파일에서 불러오기 (Load)
    cout << "\n--- 2. 파일에서 데이터 불러오기 (Load) ---\n";
    int loadedCount = 0;
    Package** myPackages = FileHandler::loadPackages(filename, loadedCount, maxPackages);

    // 메모리 누수 방지: 백업 주소 모으기
    Package** backupForCleanup = new Package*[maxPackages];
    for (int i = 0; i < loadedCount; i++) {
        backupForCleanup[i] = myPackages[i];
    }

    DeliveryManager manager;
    Truck* truck1 = new Truck(10.0, 5);
    Truck* truck2 = new Truck(10.0, 5);
    Truck* myTrucks[] = { truck1, truck2 }; // 트럭 배열화

    int turn = 1;
    while (true) {
        cout << "\n================= [ 턴 (Tick) " << turn << " 배차 진행 ] =================\n";

        // 잔여 대기열 우선순위 정렬 (TTL 변동분 반영)
        manager.sortPackagesByPriority(myPackages, loadedCount);

        // 배차 진행 (트럭이 비어있을 때만)
        for(int i = 0; i < 2; i++) {
            if(myTrucks[i]->getCurrentCount() == 0) { // 트럭이 복귀하여 비어있다면
                manager.dispatchOptimalPackages(myTrucks[i], myPackages, loadedCount);

                if(myTrucks[i]->getCurrentCount() > 0) {
                    cout << "--- [" << (i+1) << "호차 출고 내역] ---\n";
                    myTrucks[i]->printLoadedPackages();
                }
            }
        }

        // 틱 진행 (트럭 이동, 배송 완료 처리, 잔여 물류 TTL 감소)
        manager.processTick(myTrucks, 2, myPackages, loadedCount);

        // 종료 조건 검사 (대기열에 남은 화물이 있는지 확인)
        int remainingCount = 0;
        for(int i = 0; i < loadedCount; i++) {
            if (myPackages[i] != nullptr) remainingCount++;
        }

        if (remainingCount == 0) {
            cout << "\n [시뮬레이션 종료] 대기열의 모든 물류가 출고 완료되었습니다!\n";
            break;
        }

        turn++; // 다음 턴으로
    }


    // 메모리 해제
    for (int i = 0; i < loadedCount; ++i) {
        delete backupForCleanup[i];
    }
    delete[] backupForCleanup;
    delete[] myPackages;
    delete truck1;
    delete truck2;

    cout << "========== [메모리 정상 반환 및 프로그램 종료] ==========\n";
    return 0;
}