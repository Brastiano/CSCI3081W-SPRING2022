#ifndef ROBOT_H
#define ROBOT_H

#include "util/json.h"
#include "entity.h"
#include "math/vector3.h"
#include "IStrategy.h"

#include <vector>

class Robot : public IEntity {
public:
    Robot(JsonObject& obj);

    ~Robot() override = default;

    Vector3 GetPosition() const { return position; }

    Vector3 GetDirection() const { return direction; }

    Vector3 GetDestination() const { return destination; }

    bool GetAvailability() const {return available;}

    JsonObject GetDetails() const override;

    float GetSpeed() const {return speed;}

    std::string GetStrategyName() const {return strategyName;}

    std::string GetDroneType() const {return droneType;}

    IEntity* GetCarrier() const {return carrier;}

    void SetAvailability(bool choice);

    void SetPosition(Vector3 pos_) { position = pos_; }

    void SetDirection(Vector3 dir_) { direction = dir_; }

    void SetDestination(Vector3 des_) { destination = des_;}

    void SetStrategyName(std::string strategyName_) { strategyName = strategyName_;}

    void SetDroneType(std::string type) {droneType = type;}

    void SetCarrier(IEntity* carrier_) { carrier = carrier_; }

    void Rotate(double angle);

protected:
    JsonObject details;
    Vector3 position;
    Vector3 direction;
    Vector3 destination;
    float speed;
    bool available;
    std::string strategyName;
    std::string droneType;

    IEntity* carrier = NULL;  // Pointing to the drone carrying this robot
};

#endif //ROBOT_H
