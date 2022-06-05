#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

#include "../mesh/mesh.hpp"

namespace our
{

    // This component denotes that the CollisionSystem will detect and handle when the owning entity collides with another entity.
    class CollisionComponent : public Component
    {
    public:
        Mesh *mesh;                                 // keeps track of the mesh to retrueve the min and max vertices
        glm::vec3 minCollider = {0.0f, 0.0f, 0.0f}; // The minimum coordinates of the bounding box
        glm::vec3 maxCollider = {0.0f, 0.0f, 0.0f}; // The maximum coordinates of the bounding box
        float bonus = 0;
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads minCollider & maxCollider from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}