#ifndef DRONE_FACTORY_H_
#define DRONE_FACTORY_H_

#include "entity.h"
#include "IEntityFactory.h"
#include "DroneS.h"
#include "DroneL.h"
#include "UFO.h"

#include <vector>

class DroneFactory : public IEntityFactory {
  public:
    virtual ~DroneFactory() {}
    IEntity* CreateEntity(JsonObject& entity);
};

#endif
