#include "collision-component.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"
#include "../mesh/mesh.hpp"

namespace our
{
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());
        minCollider = data.value("minCollider", minCollider);
        maxCollider = data.value("maxCollider", maxCollider);
    }

}