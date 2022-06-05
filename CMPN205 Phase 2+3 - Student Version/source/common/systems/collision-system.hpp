#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/collision-component.hpp"
#include "../components/mesh-renderer.hpp"
#include "../ecs/entity.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "../asset-loader.hpp"

#include <iostream>
namespace our
{
    class CollisionSystem
    {
        // the player we will be checking for collisions
        CollisionComponent *player;
        // will store the entities that we will be checking for collisions
        CollisionComponent *collider;

    public:
        // This should be called every frame to detect if the player and any other entity collide.
        Entity *detectCollision(World *world)
        {
            for (auto entity1 : world->getEntities())
            {
                // Look for the player
                if (auto playerCandidate = entity1->getComponent<CollisionComponent>(); playerCandidate != nullptr && entity1->name == "player")
                {
                    player = playerCandidate;
                    // std::cout << "Got the ship" << '\n';
                }
                if (player)
                    break;
            }
            for (auto entity2 : world->getEntities())
            {
                if (!entity2->visible)
                    continue;
                // looping over all entities with collision components who are not the player
                if (auto boxCandidate = entity2->getComponent<CollisionComponent>(); boxCandidate != nullptr && entity2->name != "player")
                {
                    // setting the collider as the current iterator
                    collider = boxCandidate;

                    // get the owner entities of the collision components
                    Entity *myEntity1 = player->getOwner();
                    Entity *myEntity2 = collider->getOwner();
                    // gets the name of the collider specified in the json file for testing
                    std::string name = myEntity2->name;

                    // gets the min and max vertices using the mesh class
                    glm::vec3 minPlayerVertex = myEntity1->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxPlayerVertex = myEntity1->getComponent<CollisionComponent>()->mesh->maxvertex;
                    // glm::vec4 minPlayerVertex = glm::vec4(myEntity2->getComponent<CollisionComponent>()->mesh->minvertex, 1);
                    // glm::vec4 maxPlayerVertex = glm::vec4(myEntity2->getComponent<CollisionComponent>()->mesh->maxvertex, 1);

                    // transforms the min and max vertices to the wold space
                    minPlayerVertex *= myEntity1->localTransform.scale[0];
                    maxPlayerVertex *= myEntity1->localTransform.scale[0];
                    minPlayerVertex += myEntity1->localTransform.position;
                    maxPlayerVertex += myEntity1->localTransform.position;
                    // minPlayerVertex = myEntity2->getLocalToWorldMatrix() * minPlayerVertex;
                    // maxPlayerVertex = myEntity2->getLocalToWorldMatrix() * maxPlayerVertex;

                    // gets the min and max vertices using the mesh class
                    glm::vec3 minCollider = myEntity2->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxCollider = myEntity2->getComponent<CollisionComponent>()->mesh->maxvertex;
                    // glm::vec4 minCollider = glm::vec4(myEntity2->getComponent<CollisionComponent>()->mesh->minvertex, 1);
                    // glm::vec4 maxCollider = glm::vec4(myEntity2->getComponent<CollisionComponent>()->mesh->maxvertex, 1);

                    // transforms the min and max vertices to the wold space
                    minCollider *= myEntity2->localTransform.scale[0];
                    maxCollider *= myEntity2->localTransform.scale[0];
                    minCollider += myEntity2->localTransform.position;
                    maxCollider += myEntity2->localTransform.position;
                    // minCollider = myEntity2->getLocalToWorldMatrix() * minCollider;
                    // maxCollider = myEntity2->getLocalToWorldMatrix() * maxCollider;

                    // collision between AABBs check
                    if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                        (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                        (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
                    {
                        // i++;
                        // std::cout << "collision detected with " << name << " " << i << std::endl;
                        return myEntity2;
                    }
                }
            }
            return nullptr;
        }
    };

}
