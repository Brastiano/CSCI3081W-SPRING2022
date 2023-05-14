#define _USE_MATH_DEFINES
#include "UFO.h"
#include "Beeline.h"
#include <cmath>
#include <limits>

UFO::UFO(JsonObject& obj) : details(obj) {
    JsonArray pos(obj["position"]);
    position = {pos[0], pos[1], pos[2]};

    JsonArray dir(obj["direction"]);
    direction = {dir[0], dir[1], dir[2]};

    speed = obj["speed"];

    available = true;
    delay = 10.0;
}

UFO::~UFO() {
    // Delete dynamically allocated variables
}

void UFO::GetNearestEntity(std::vector<IEntity*> scheduler) {
    // IEntity* nearestEntity_ = nullptr;
    float minDis = std::numeric_limits<float>::max();
    for(auto entity : scheduler){
        std::string droneType = entity->GetDroneType();
        if(!entity->GetAvailability()){
            if (delay > 0.0) {
                delay -= 0.1;
            } else {
                  float disToEntity = this->position.Distance(entity->GetPosition());
                  if(disToEntity <= minDis){
                      minDis = disToEntity;
                      nearestEntity = entity;
// This will tell what is transporting the customer at the moment, the UFO or the drone
                      carrier = nearestEntity->GetCarrier();
                }
            }
        }
    }
    if(nearestEntity) {
       float distanceRobotDestination = nearestEntity->GetPosition().Distance(nearestEntity->GetDestination());
       std::string robotStratName = nearestEntity->GetStrategyName();
       if (robotStratName.compare("beeline") == 0 && distanceRobotDestination > 1.0 || carrier == this) {
// Beeline strategy tends to drop the customer approximately 1.0 distance unit away from destination
// Therefore, as long as distanceRobotDestination > 1.0, the customer can still be captured
// The last condition will be true when using AscendingDecorator
         available = false;
         toTargetPosStrategy = new Beeline(this->GetPosition(), nearestEntity->GetPosition());
         toTargetPosStrategy = new AscendingDecorator(toTargetPosStrategy, nearestEntity, this);
      } else if (robotStratName.compare("beeline") != 0 && distanceRobotDestination > 25.5 || carrier == this) {
// Other strategies tend to drop the customer at approximately 25.5 distance unit away from destination to spin
// Therefore, as long as distanceRobotDestination > 25.5, the customer can still be captured
// The last condition will be true when using AscendingDecorator
          available = false;
          toTargetPosStrategy = new Beeline(this->GetPosition(), nearestEntity->GetPosition());
          toTargetPosStrategy = new AscendingDecorator(toTargetPosStrategy, nearestEntity, this);
      } else {
// This will be for the case when the customer reaches their destination before the UFO comes near enough
// And for the case when the AscendingDecorator is done
           available = true;
           delete toTargetPosStrategy;
           toTargetPosStrategy = NULL;
           carrier = NULL;
           nearestEntity = NULL;
      }
    }
}

void UFO::Update(double dt, std::vector<IEntity*> scheduler) {
// Call GetNearestEntity function to always update the robot's latest position
    GetNearestEntity(scheduler);
    if (nearestEntity) {
      float distanceRobotDestination = nearestEntity->GetPosition().Distance(nearestEntity->GetDestination());
      float distanceRobotUFO = this->position.Distance(nearestEntity->GetPosition());
      if (distanceRobotUFO <= 0.5 || nearestEntity->GetCarrier() == this) {
// If distanceRobotUFO <= 0.5 when the customer has not arrived at their destination, they will be captured
// The UFO will then start ascending to the sky, bringing the customer with it
// The second condition will be true when using AscendingDecorator
              nearestEntity->SetCarrier(this);
              toTargetPosStrategy->Move(this, dt);
              nearestEntity->SetPosition(this->GetPosition());
              nearestEntity->SetDirection(this->GetDirection());
              if (toTargetPosStrategy->IsCompleted()) {
                nearestEntity->SetAvailability(true);
              }
      } else { // Keep chasing if the customer has not arrived at their destination
              toTargetPosStrategy->Move(this, dt);
      }
    }
}

void UFO::Rotate(double angle){
    direction.x = direction.x*std::cos(angle) - direction.z*std::sin(angle);
    direction.z = direction.x*std::sin(angle) + direction.z*std::cos(angle);
}
