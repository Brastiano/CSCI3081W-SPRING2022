#ifndef ASCENDING_DECORATOR_H_
#define ASCENDING_DECORATOR_H_

#include <vector>
#include "IStrategy.h"
#include "entity.h"
#include "math/vector3.h"

class AscendingDecorator: public IStrategy {
    public:
        AscendingDecorator(IStrategy *strategy_, IEntity* robot_, IEntity* ufo_);
        bool IsCompleted();
        void Move(IEntity* entity, double dt);
        float GetDistance() {return 0.0;}
    private:
        IStrategy* strategy;
        IEntity* robot;
        IEntity* ufo;
        bool done;
}; //close class

#endif // ASCENDING_DECORATOR_H_
