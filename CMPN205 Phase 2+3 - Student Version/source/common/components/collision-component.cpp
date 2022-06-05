#include "collision-component.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"
#include "../mesh/mesh.hpp"

namespace our
{
    // reads the mesh from the json object and populates the min and max vertices
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());

        minCollider = mesh->minvertex;
        maxCollider = mesh->maxvertex;
    }

}