#ifndef PROJECT2025112431_PACKAGE_H
#define PROJECT2025112431_PACKAGE_H
#include <string>
using namespace std;

enum class DeliveryType { NORMAL, EXPRESS, COLD_CHAIN };
enum class DeliveryStatus { WAITING, DELIVERING, COMPLETED };

class Package
{
private:
    string id;                  // 교유 식별자
    string name;                // 품목명
    DeliveryType type;          // 배송 타입 (일반, 특급, 콜드체인)
    double weight;              // 무게
    int basePriority;            // 기본 우선도 점수
    DeliveryStatus status;      // 현재 상태

    int ttl;                    // 부패 마감 시간(콜드체인 적용)

public:
    // 생성자
    Package(string id, string name, DeliveryType type, double weight, int basePriority, int ttl = -1);

    int getCurrentPriority();                       // 우선도 반환
    void decreaseTTL();                             // 틱 진행시 TTL 감소
    void updateStatus(DeliveryStatus newStatus);    // 상태 전이 로직

    // Getters
    string getId();
    string getName();
    DeliveryType getType();
    int getBasePriority();
    int getTtl();
    double getWeight();
    DeliveryStatus getStatus();
};

#endif //PROJECT2025112431_PACKAGE_H
