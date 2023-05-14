#include "PrintReceiptDecorator.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

PrintReceiptDecorator::PrintReceiptDecorator(IStrategy *strategy_, float rate, float cost, std::string name, std::string strategy_name) {
    strategy = strategy_;
    droneRate = rate;
    baseCost = cost;
    droneName = name;
    strategyName = strategy_name;
    donePrinting = false;
}

void PrintReceiptDecorator::Move(IEntity* entity, double dt) {
    if (strategy->IsCompleted() && !IsCompleted()) {
        std::ofstream receipt("Receipt.txt");
        auto t = std::time(nullptr);
        auto time = *std::localtime(&t);
        receipt << "Time: " << std::put_time(&time, "%m/%d/%Y - %H:%M:%S") << std::endl;
        receipt << "Drone name: " << droneName << std::endl;
        receipt << std::fixed << std::setprecision(2);
        receipt << "Drone base cost: $" << baseCost << std::endl;
        receipt << "Drone rate: $" << droneRate << "/m" << std::endl;
        receipt << "Travel strategy: " << strategyName << std::endl;
        receipt << "Distance traveled: " << GetDistance() << "m" << std::endl;
        receipt << "Total cost: $" << baseCost + (GetDistance() * droneRate) / 100 << std::endl;
        receipt.close();
        donePrinting = true;
    } else {
        strategy->Move(entity, dt);
    }
}

bool PrintReceiptDecorator::IsCompleted() {
    return donePrinting;
}
