#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include "../mesh/mesh.hpp"

namespace our
{
    class SpawningController : public Component
    {
        glm::vec3 minLimit = {-5.0f, -1.0 / 5.0f, 0.0f};
        glm::vec3 maxLimit = {5.0f, 1.0 / 5.0f, 500.0f};
        glm::vec3 rangeValues = {3, 3, 100};

    public:
        float respawningFreq = 0;
        float timeElapsed = 0;
        //= The ID of this component type is "Spawning Controller" (will be used in JSON)
        static std::string getID() { return "Spawning Controller"; }

        glm::vec3 getMinLimit();
        glm::vec3 getMaxLimit();
        glm::vec3 getRangeValues();
        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
}