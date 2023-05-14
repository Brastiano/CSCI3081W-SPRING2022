#include "AscendingDecorator.h"

AscendingDecorator::AscendingDecorator(IStrategy* strategy_, IEntity* robot_, IEntity* ufo_) {
  strategy = strategy_;
  robot = robot_;
  ufo = ufo_;
  done = false;
}

void AscendingDecorator::Move(IEntity* entity, double dt) {
  if (robot->GetCarrier() == ufo && !IsCompleted()) {
    Vector3 tempPos = entity->GetPosition();
    float newHeight = tempPos[1] + 1.0;
    if (newHeight <= 500) {
      entity->SetPosition(Vector3(tempPos[0], newHeight, tempPos[2]));
    } else {
      done = true;
    }
  } else {
      strategy->Move(entity, dt);
  }
}

bool AscendingDecorator::IsCompleted() {
  return done;
}
