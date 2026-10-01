#include <iostream>
#include "Truck.h"

Truck::Truck(double MaxCapacity, int maxPackages)
    : capacity(MaxCapacity), currentWeight(0.0), maxPackages(maxPackages), currentCount(0)
{
    loadedPackages = new Package*[maxPackages];
}

Truck::~Truck()
{
    delete[] loadedPackages;
}

bool Truck::loadPackage(Package* pkg)
{
    // 배열 공간이 다 찼는지 확인
    if (currentCount >= maxPackages)
    {
        return false;
    }

    // 무게 용량을 초과하는지 확인
    if (currentWeight + pkg->getWeight() > capacity)
    {
        return false;
    }

    // 배열에 물류 포인터 추가 후 카운트,무게 추가
    loadedPackages[currentCount] = pkg;
    currentCount++;
    currentWeight += pkg->getWeight();

    return true;
}

void Truck::clearLoadedPackages()
{
    // 객체 자체를 지우는 것이 아닌 트럭 안을 비우는 연산
    currentCount = 0;
    currentWeight = 0.0;
}

double Truck::getRemainingCapacity()
{
    return capacity - currentWeight;
}

int Truck::getCurrentCount() { return currentCount; }

Package** Truck::getLoadedPackages() { return loadedPackages; }

void Truck::printLoadedPackages() {
    cout << "[트럭 적재 완료] 총 적재 무게: " << currentWeight << " / " << capacity << "\n";
    for (int i = 0; i < currentCount; i++) {
        cout << "  - " << loadedPackages[i]->getName()
                  << " (무게: " << loadedPackages[i]->getWeight()
                  << ", 우선도: " << loadedPackages[i]->getCurrentPriority() << ")\n";
    }
}

