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

        void update(World *world, float deltaTime)
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
                    spawnController->timeElapsed += deltaTime;
                    // if the object is out of sight, re-spawn it in the world in a random position
                    // in front of the camera
                    if (pos.z <= spawnController->getMinLimit().z)
                    {
                        if (spawnController->timeElapsed > spawnController->respawningFreq)
                        {
                            entity->visible = true;
                            spawnController->timeElapsed = 0;
                        }
                        else
                        {
                            entity->visible = false;
                        }
                        // re-spawn the asteroid ahead by a value in the range
                        // [20% of distance from camera far plane, 50% of distance from camera far plane]
                        float z_random_displacement = randomNumber(camera->far / 2, 0.2 * camera->far, 1);
                        entity->localTransform.position.z += z_random_displacement;

                        // add random number to player x coordinate so it's not always the same value.
                        float x_random_displacement = randomNumber(spawnController->getRangeValues().x, -1, 1);

                        // keep asteroid position in x-axis within the range [-5.0, 5.0]
                        if (entity->localTransform.position.x + x_random_displacement <= spawnController->getMinLimit().x || entity->localTransform.position.x + x_random_displacement >= spawnController->getMaxLimit().x)
                            entity->localTransform.position.x -= x_random_displacement;
                        else
                            entity->localTransform.position.x += x_random_displacement;

                        // add random number to player y coordinate so it's not always the same level.
                        float y_random_displacement = randomNumber(spawnController->getRangeValues().y, -1, 5.0f);
                        entity->localTransform.position.y = y_random_displacement;
                    }
                }
            }
        }
    };
};