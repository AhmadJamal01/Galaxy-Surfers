#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    // This component denotes that the FreeCameraControllerSystem will move the owning entity using user inputs.
    // It will also control the camera field of view accroding to the scrolling of the mouse wheel
    // This component is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/free-camera-controller.hpp"
    // For a more simple example of how to use the ECS framework, see "movement.hpp"
    class NinjaControllerComponent : public Component {
    public:
        // The senstivity paramter defined sensitive the camera rotation & fov is to the mouse moves and wheel scrolling
        glm::vec3 positionSensitivity = {3.0f, 3.0f, 3.0f}; // The unity per second of camera movement if WASD is pressed
        float speedupFactor = 5.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.

        // The ID of this component type is "Free Camera Controller"
        static std::string getID() { return "Ninja Camera Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}