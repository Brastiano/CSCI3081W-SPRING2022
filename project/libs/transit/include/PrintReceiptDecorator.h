#ifndef PRINT_RECEIPT_DECORATOR_H_
#define PRINT_RECEIPT_DECORATOR_H_

#include <vector>
#include "IStrategy.h"

class PrintReceiptDecorator: public IStrategy {
    public:
        PrintReceiptDecorator(IStrategy* strategy, float rate, float cost, std::string name, std::string strategy_name);
        void Move(IEntity* entity, double dt);
        bool IsCompleted();
        float GetDistance() { return strategy->GetDistance(); }
    private:
        IStrategy* strategy;
        float droneRate;
        float baseCost;
        std::string droneName;
        std::string strategyName;
        bool donePrinting;
}; //close class

#endif // PRINT_RECEIPT_DECORATOR_H_
