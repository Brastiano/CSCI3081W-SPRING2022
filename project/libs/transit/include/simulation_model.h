#ifndef SIMULATION_MODEL_H_
#define SIMULATION_MODEL_H_

#include "controller.h"
#include "CompositeFactory.h"
#include "IEntityFactory.h"
#include "entity.h"
#include "graph.h"
#include "robot.h"
#include "DroneS.h"
#include "DroneL.h"
#include "UFO.h"
using namespace routing;

//--------------------  Model ----------------------------

/// Simulation Model handling the transit simulation.  The model can communicate with the controller.
class SimulationModel {
public:
    SimulationModel(IController& controller);

    void SetGraph(const IGraph* graph) {this->graph = graph;}

    /// Creates an simulation entity
    void CreateEntity(JsonObject& entity);

    /// Schedules a trip for an object in the scene
    void ScheduleTrip(JsonObject& details);

    /// Updates the simulation
    void Update(double dt);

    /// Adds a new entity type
    void AddFactory(IEntityFactory* factory);

    void PrintCost(JsonObject& details);

protected:
    float price;
    IController& controller;
    std::vector<IEntity*> entities;
    std::vector<DroneS*> drones_s;
    std::vector<DroneL*> drones_l;
    std::vector<Robot*> robots;
    std::vector<UFO*> ufos;
    std::vector<IEntity*> scheduler;
    CompositeFactory* compFactory;
    const IGraph* graph;
};

#endif
