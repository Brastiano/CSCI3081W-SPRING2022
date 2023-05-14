#define _USE_MATH_DEFINES
#include "DroneL.h"
#include "Beeline.h"
#include "AstarStrategy.h"
#include "DijkstraStrategy.h"
#include "DfsStrategy.h"
#include "SpinDecorator.h"
#include "PrintReceiptDecorator.h"
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

DroneL::DroneL(JsonObject& obj) : details(obj) {
    JsonArray pos(obj["position"]);
    position = {pos[0], pos[1], pos[2]};

    JsonArray dir(obj["direction"]);
    direction = {dir[0], dir[1], dir[2]};

    speed = obj["speed"];
    std::srand(std::time(NULL));
    if (std::rand() % 2 + 1 == 1) {
        speed = speed + 5.0;
    }

    available = true;

    name = "Large Drone";
    base_cost = 15.0;
    rate = 5.0;
}

DroneL::~DroneL() {
    // Delete dynamically allocated variables
}

void DroneL::GetNearestEntity(std::vector<IEntity*> scheduler) {
    // IEntity* nearestEntity_ = nullptr;
    float minDis = std::numeric_limits<float>::max();
    for(auto entity : scheduler){
        std::string droneType = entity->GetDroneType();
        float distanceRobotDestination = entity->GetPosition().Distance(entity->GetDestination());
        if(droneType.compare("drone_l") == 0 && entity->GetAvailability() && entity->GetCarrier() == NULL && distanceRobotDestination > 25.5){
            float disToEntity = this->position.Distance(entity->GetPosition());
            if(disToEntity <= minDis){
                minDis = disToEntity;
                nearestEntity = entity;
            }
        }
    }
    if(nearestEntity){               // Set strategy for the nearest entity
        nearestEntity->SetAvailability(false);  // set availability to the entity that the drone is picking up
        available = false;                      // set this drone availability as false
        SetDestination(nearestEntity->GetPosition());
        this->costSoFar = base_cost;
        nearestEntity->SetCarrier(this);
        toTargetPosStrategy = new Beeline(this->GetPosition(), nearestEntity->GetPosition());
        std::string targetStrategyName = nearestEntity->GetStrategyName();
        if(targetStrategyName.compare("beeline") == 0){
            toTargetDesStrategy = new Beeline(nearestEntity->GetPosition(), nearestEntity->GetDestination());
            toTargetDesStrategy = new PrintReceiptDecorator(toTargetDesStrategy, rate, base_cost, name, "Beeline");
        } else if (targetStrategyName.compare("astar") == 0){
            toTargetDesStrategy = new AstarStrategy(nearestEntity->GetPosition(), nearestEntity->GetDestination(), graph);
            toTargetDesStrategy = new SpinDecorator(toTargetDesStrategy); // add decorator
            toTargetDesStrategy = new PrintReceiptDecorator(toTargetDesStrategy, rate, base_cost, name, "Astar");
        } else if (targetStrategyName.compare("dfs") == 0){
            toTargetDesStrategy = new DfsStrategy(nearestEntity->GetPosition(), nearestEntity->GetDestination(), graph);
            toTargetDesStrategy = new SpinDecorator(toTargetDesStrategy); // add decorator
            toTargetDesStrategy = new PrintReceiptDecorator(toTargetDesStrategy, rate, base_cost, name, "DFS");
        } else if (targetStrategyName.compare("dijkstra") == 0){
            toTargetDesStrategy = new DijkstraStrategy(nearestEntity->GetPosition(), nearestEntity->GetDestination(), graph);
            toTargetDesStrategy = new SpinDecorator(toTargetDesStrategy); // add decorator
            toTargetDesStrategy = new PrintReceiptDecorator(toTargetDesStrategy, rate, base_cost, name, "Dijkstra");
        } else {
            // If none of the strategy name matched, use beeline as default.
            toTargetDesStrategy = new Beeline(nearestEntity->GetPosition(), nearestEntity->GetDestination());
            toTargetDesStrategy = new PrintReceiptDecorator(toTargetDesStrategy, rate, base_cost, name, "Beeline");
        }
    }
}

void DroneL::Update(double dt, std::vector<IEntity*> scheduler) {
    if(available) {
        GetNearestEntity(scheduler);
    }
// This will set the current carrier of the customer to be this drone
// Only if the drone has not registered anything as its carrier yet
    if (nearestEntity && nearestEntity->GetCarrier() == NULL) {
        nearestEntity->SetCarrier(this);
    }
    if (toTargetPosStrategy) {  // Move drone toward the entity's position
// This will set the current carrier of the customer to be this drone
        toTargetPosStrategy->Move(this, dt);
        if(toTargetPosStrategy->IsCompleted()){
            delete toTargetPosStrategy;
            toTargetPosStrategy = NULL;
        }
    } else if (toTargetDesStrategy) { // Move drone and entity toward the entity's destination
        toTargetDesStrategy->Move(this, dt);
        nearestEntity->SetPosition(this->GetPosition());
        nearestEntity->SetDirection(this->GetDirection());
        this->costSoFar = base_cost + (toTargetDesStrategy->GetDistance() * rate) / 100;
        if(nearestEntity->GetCarrier() != this || toTargetDesStrategy->IsCompleted()){
            if (toTargetDesStrategy->IsCompleted()) {
              nearestEntity->SetAvailability(true);
            }
            delete toTargetDesStrategy;
            toTargetDesStrategy = NULL;
            available = true;
            nearestEntity = NULL;
        }
    }
}

void DroneL::Rotate(double angle){
    direction.x = direction.x*std::cos(angle) - direction.z*std::sin(angle);
    direction.z = direction.x*std::sin(angle) + direction.z*std::cos(angle);
}
