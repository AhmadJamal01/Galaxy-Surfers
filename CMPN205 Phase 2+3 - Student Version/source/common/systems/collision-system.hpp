int i = 0;
#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/collision-component.hpp"
#include "../components/mesh-renderer.hpp"

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
        // Application *app;              // The application in which the state runs (app holds general data, controlling events)
        MeshRendererComponent *player; // We need external acess to the camera component of the player for ImGUI
        MeshRendererComponent *collider;

    public:
        // void enter(Application *app)
        // {
        //     this->app = app;
        // }

        // This should be called every frame to update all entities containing a NinjaControllerComponent
        void detectCollision(World *world)
        {
            // auto &config = app->getConfig()["scene"]["world"]["components"];

            // test using camera and box
            for (auto entity1 : world->getEntities())
            {
                // Look for the NinjaControllerComponent (has speed data so far)
                // and the camera component (in the camera entity). Note that even the nested camera entity will be looped on (2nd if condition)
                if (auto playerCandidate = entity1->getComponent<MeshRendererComponent>(); playerCandidate != nullptr && entity1->name == "player")
                {
                    player = playerCandidate;
                    // std::string name = AssetLoader<Mesh>::get(data["world"].get<std::string>());
                }
                if (player)
                    break;
            }
            for (auto entity2 : world->getEntities())
            {
                // if the entity is different from the current entity

                // if (auto boxCandidate = entity2->getComponent<MeshRendererComponent>(); boxCandidate != nullptr)
                if (auto boxCandidate = entity2->getComponent<MeshRendererComponent>(); boxCandidate != nullptr)
                {
                    collider = boxCandidate;
                    Entity *myEntity1 = player->getOwner();
                    Entity *myEntity2 = collider->getOwner();
                    std::string name = myEntity2->name;
                    glm::vec3 minCameraVertex = glm::vec3(myEntity1->localTransform.position.x - 1,
                                                          myEntity1->localTransform.position.y - 1,
                                                          myEntity1->localTransform.position.z - 1);

                    glm::vec3 maxCameraVertex = glm::vec3(myEntity1->localTransform.position.x + 1,
                                                          myEntity1->localTransform.position.y + 1,
                                                          myEntity1->localTransform.position.z + 1);

                    glm::vec3 minCollider = myEntity2->getComponent<MeshRendererComponent>()->mesh->minvertex;
                    glm::vec3 maxCollider = myEntity2->getComponent<MeshRendererComponent>()->mesh->maxvertex;

                    minCollider *= myEntity2->localTransform.scale[0];
                    maxCollider *= myEntity2->localTransform.scale[0];

                    minCollider += myEntity2->localTransform.position;
                    maxCollider += myEntity2->localTransform.position;

                    if ((minCameraVertex.x <= maxCollider.x && maxCameraVertex.x >= minCollider.x) &&
                        (minCameraVertex.y <= maxCollider.y && maxCameraVertex.y >= minCollider.y) &&
                        (minCameraVertex.z <= maxCollider.z && maxCameraVertex.z >= minCollider.z))
                    {
                        i++;
                        std::cout << "collision detected " << name << " " << i << std::endl;
                        // break;
                    }
                }
            }
        }
    };

}