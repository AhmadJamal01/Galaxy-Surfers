#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    // This component denotes that the FreeCameraControllerSystem will move the owning entity using user inputs.
    // For a more simple example of how to use the ECS framework, see "movement.hpp"
    class NinjaControllerComponent : public Component {
    public:
        glm::vec3 positionSensitivity = {3.0f, 3.0f, 3.0f}; //= The speed the entity when the user presses the WASD keys.
        float speedupFactor = 5.0f; //= A multiplier for the positionSensitivity if "Left Shift" is held.

        //= The ID of this component type is "Free Camera Controller" (will be used in JSON)
        static std::string getID() { return "Ninja Camera Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}