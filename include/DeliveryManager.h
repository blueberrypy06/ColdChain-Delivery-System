//
// Created by 문준민 on 26. 5. 27..
//

#ifndef PROJECT2025112431_DELIVERYMANGER_H
#define PROJECT2025112431_DELIVERYMANGER_H
#include "Package.h"
#include "Truck.h"


class DeliveryManager
{
public:
    void sortPackagesByPriority(Package** packages, int count);
    void dispatchOptimalPackages(Truck* truck, Package** waitingQueue, int queueSize);
    void processTick(Truck** trucks, int truckCount, Package** waitingQueue, int queueSize);
};


#endif //PROJECT2025112431_DELIVERYMANGER_H
