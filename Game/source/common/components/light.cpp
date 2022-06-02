#include "./light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        diffuse = data.value("diffuse", diffuse);
		kind = data.value("kind", kind);
		specular = data.value("specular", specular);
		attenuation = data.value("attenuation", attenuation);
		cone_angles =  glm::radians(data.value("cone_angles", cone_angles));

    }
}