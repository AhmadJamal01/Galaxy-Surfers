#pragma once

#include "../ecs/world.hpp"
#include "../components/spawning-component.hpp"
#include "../components/camera.hpp"

namespace our
{
    class SpawningSystem
    {
        // These window size will be used on multiple occasions (setting the viewport, computing the aspect ratio, etc.)
        glm::ivec2 windowSize;
        float randomNumber(int range, float minimum, float scale)
        {
            return (rand() % range + minimum) / scale;
        }

    public:
        void initialize(glm::ivec2 windowSize)
        {
            // store the window size for later use
            this->windowSize = windowSize;
        }

        void update(World *world)
        {
            CameraComponent *camera = nullptr;

            // Search the camera component
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                if (camera)
                    break;
            }
            // Construct the Projection View Matrix
            glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();

            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // check if it has a mesh renderer component
                if (auto spawnController = entity->getComponent<SpawningController>(); spawnController)
                {
                    // model matrix is the transformation matrix from local space to world space
                    glm::mat4 localToWorld = spawnController->getOwner()->getLocalToWorldMatrix();
                    // get object center position in the world space
                    glm::vec4 center = glm::vec4(localToWorld * glm::vec4(0, 0, 0, 1));
                    // transform the object center from world to screen space
                    glm::vec4 pos = VP * center;
                    // if the object is out of sight, reset its position to be in front of the camera
                    // by 16 units
                    if (pos.z <= spawnController->getMinLimit().z)
                    {
                        // add random number to player z coordinate so it's not always the same value.
                        float z_random_displacement = randomNumber(spawnController->getRangeValues().z, -2, 1);
                        entity->localTransform.position.z += 16 + z_random_displacement;

                        // add random number to player x coordinate so it's not always the same value.
                        float x_random_displacement = randomNumber(spawnController->getRangeValues().x, -1, 1);

                        // keep asteroid position in x-axis within the range [-5.0, 5.0]
                        if (entity->localTransform.position.x + x_random_displacement <= spawnController->getMinLimit().x || entity->localTransform.position.x + x_random_displacement >= spawnController->getMaxLimit().x)
                            entity->localTransform.position.x -= x_random_displacement;
                        else
                            entity->localTransform.position.x += x_random_displacement;

                        float y_random_displacement = randomNumber(spawnController->getRangeValues().y, -1, 5.0f);
                        entity->localTransform.position.y = y_random_displacement;
                    }
                }
            }
        }
    };
};