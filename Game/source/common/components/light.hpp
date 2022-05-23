#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {
    class LightComponent : public Component {
    public:
        int kind;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 attenuation;
		glm::vec2 cone_angles;
        static std::string getID() { return "Light"; }

        void deserialize(const nlohmann::json& data) override;
    };

}