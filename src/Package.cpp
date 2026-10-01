#include "Package.h"

Package::Package(string id, string name, DeliveryType type, double weight, int basePriority, int ttl)
    : id(id), name(name), type(type), weight(weight), basePriority(basePriority), status(DeliveryStatus::WAITING), ttl(ttl) {}

void Package::updateStatus(DeliveryStatus newStatus)
{
    status = newStatus;
}

void Package::decreaseTTL()
{
    if (type == DeliveryType::COLD_CHAIN && ttl > 0)
    {
        ttl--;
    }
}

int Package::getCurrentPriority()
{
    int currentPriority = basePriority;

    // 콜드체인 가중치 부여를 위한 임계치
    const int TTL_THRESHOLD = 5;

    if (type == DeliveryType::COLD_CHAIN && ttl <= TTL_THRESHOLD && ttl >= 0)
    {
        // 남은 TTL이 적을수록 우선순위가 기하급수적으로 증가하는 가중치 연산
        double weight = pow(2.0, (TTL_THRESHOLD - ttl)) * 10.0;
        currentPriority += static_cast<int>(weight);
    }
    return currentPriority;
}

string Package::getId()
{
    return id;
}

string Package::getName()
{
    return name;
}

DeliveryType Package::getType()
{
    return type;
}

double Package::getWeight()
{
    return weight;
}

DeliveryStatus Package::getStatus()
{
    return status;
}

int Package::getTtl()
{
    return ttl;
}

int Package::getBasePriority()
{
    return basePriority;
}