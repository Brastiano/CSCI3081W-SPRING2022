#include "simulation_model.h"
#include "DroneFactory.h"
#include "RobotFactory.h"

SimulationModel::SimulationModel(IController& controller) : controller(controller){
  compFactory = new CompositeFactory();
  compFactory->AddFactory(new DroneFactory());
  compFactory->AddFactory(new RobotFactory());
}

/// Creates an simulation entity
void SimulationModel::CreateEntity(JsonObject& entity) {
    // int id = entity["entityId"];
    std::string type = entity["type"];
    std::string name = entity["name"];
    JsonArray position = entity["position"];

    IEntity* myNewEntity = compFactory->CreateEntity(entity);
    myNewEntity->SetGraph(graph);
    // Add to the small drone dictionary
    DroneS* drone_small = dynamic_cast<DroneS*>(myNewEntity);
    if(drone_small) {
      drones_s.push_back(drone_small);
    }
    // Add to the large drone dictionary
    DroneL* drone_large = dynamic_cast<DroneL*>(myNewEntity);
    if(drone_large) {
      drones_l.push_back(drone_large);
    }
    // Add to the robot dictionary
    Robot* robot = dynamic_cast<Robot*>(myNewEntity);
    if(robot) {
      robots.push_back(robot);
    }
    UFO* ufo = dynamic_cast<UFO*>(myNewEntity);
    if(ufo) {
      ufos.push_back(ufo);
    }
    // Call AddEntity to add it to the view
    controller.AddEntity(*myNewEntity);
    entities.push_back(myNewEntity);

    if (type.compare("robot") == 0) {
      std::cout << name << ", Initial Position: " << position << std::endl;
    } else {
      float speed = myNewEntity->GetSpeed();
      std::cout << name << ", Speed " << speed << ", Initial Position: " << position << std::endl;
    }
}

/// Schedules a trip for an object in the scene
void SimulationModel::ScheduleTrip(JsonObject& details) {
    this->price = 0;
    std::string name = details["name"];
    JsonArray start = details["start"];
    JsonArray end = details["end"];
    std::cout << name << ": " << start << " --> " << end << std::endl;

    for(auto entity : entities){  // Add the entity to the scheduler
      JsonObject detailsTemp = entity->GetDetails();
      std::string nameTemp = detailsTemp["name"];
      std::string typeTemp = detailsTemp["type"];
      if(name.compare(nameTemp) == 0 && typeTemp.compare("robot") == 0 && entity->GetAvailability()){
        std::string strategyName = details["search"];
        std::string droneType = details["drone"];
        entity->SetStrategyName(strategyName);
        entity->SetDroneType(droneType);
        entity->SetDestination(Vector3(end[0], end[1], end[2]));
        scheduler.push_back(entity);
        break;
      }
    }
    controller.SendEventToView("TripScheduled", details);

    // Add a route to visualize the path
    // controller.AddPath(pathId, path);
}

/// Updates the simulation
void SimulationModel::Update(double dt) {
    JsonObject cost = JsonObject();
    //std::cout << "Update: " << dt << std::endl;

    // Call controller to update changed entities
     JsonObject receipt = JsonObject();
    for (int i = 0; i < entities.size(); i++) {
      entities[i]->Update(dt, scheduler);
      DroneS* small = dynamic_cast<DroneS*>(entities[i]);
      DroneL* large = dynamic_cast<DroneL*>(entities[i]);
      if (small && !small->GetAvailability()) {
        cost["cost"] = small->GetCostSoFar();
        this->PrintCost(cost);
      } else if (large && !large->GetAvailability()) {
        cost["cost"] = large->GetCostSoFar();
        this->PrintCost(cost);
      }
      controller.UpdateEntity(*entities[i]);
    }

   
   

   

    // Remove entites you no longer need
    // controller.RemoveEntity(myDeletedEntityId);

    // Remove paths you no longer want to see:
    // controller.RemovePath(myDeletedPathId);
}

void SimulationModel::AddFactory(IEntityFactory* factory) {
  compFactory->AddFactory(factory);
}

void SimulationModel::PrintCost(JsonObject& details) {
  controller.SendEventToView("PrintCost", details);
}
