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
        // Mesh *mesh; // The mesh that should be drawn
        // glm::vec3 boundingBox[8] = {glm::vec3(0.0, 0.0, 0.0)}; // Each entity has a bounding box that defines its collision volume.
        //? not needed
        glm::vec3 minCollider = {0.0f, 0.0f, 0.0f}; // The minimum coordinates of the bounding box
        glm::vec3 maxCollider = {0.0f, 0.0f, 0.0f}; // The maximum coordinates of the bounding box

        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads minCollider & maxCollider from the given json object
        void deserialize(const nlohmann::json &data) override;

        // should be in the system?
        // bool detetcCollision(Entity *other);
    };

}