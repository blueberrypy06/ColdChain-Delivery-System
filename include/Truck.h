#ifndef PROJECT2025112431_TRUCK_H
#define PROJECT2025112431_TRUCK_H

#include "Package.h"

using namespace std;

class Truck
{
private:
    double capacity;            // 최대 적재 용량
    double currentWeight;       // 현재 적재된 총 무게

    int maxPackages;            // 동적 배열 최대 크기
    int currentCount;           // 현재 배열에 실린 Package의 개수

    Package** loadedPackages;   // Package 객체들의 주소를 담는 배열을 가르키는 포인터
public:
    // 생성자
    Truck(double maxCapacity, int maxPackages);
    // 소멸자
    ~Truck();

    //화물 적재 로직
    bool loadPackage(Package* pkg);

    // 적재된 물류 목록 초기화 (배송 완료 후 처리)
    void clearLoadedPackages();

    // Getter
    double getRemainingCapacity();
    int getCurrentCount();
    Package** getLoadedPackages();

    void printLoadedPackages();
};


#endif //PROJECT2025112431_TRUCK_H
