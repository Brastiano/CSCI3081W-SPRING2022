#include "DroneFactory.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity){
  std::string type = entity["type"];
  if(type.compare("drone_s") == 0){
    DroneS* droneTemp = new DroneS(entity);
    std::cout << "Small Drone Created" << std::endl;
    return droneTemp;
  }
  else if(type.compare("drone_l") == 0){
    DroneL* droneTemp = new DroneL(entity);
    std::cout << "Large Drone Created" << std::endl;
    return droneTemp;
  }
  else if(type.compare("ufo") == 0){
    UFO* droneTemp = new UFO(entity);
    std::cout << "UFO Created" << std::endl;
    return droneTemp;
  }
  return nullptr;
}
