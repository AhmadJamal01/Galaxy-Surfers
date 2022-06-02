#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/ninja-controller-component.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>
namespace our
{
    // The free camera controller system is responsible for moving every entity which contains a NinjaControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class NinjaSystem {
        Application* app;               // The application in which the state runs (app holds general data, controlling events)
        bool mouse_locked = false;      // Is the mouse locked (e.g. unlock it in main menu only)
        CameraComponent * playerCamera; // We need external acess to the camera component of the player for ImGUI

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        // This should be called every frame to update all entities containing a NinjaControllerComponent
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a NinjaControllerComponent
            // As soon as we find one, we break
            NinjaControllerComponent *controller = nullptr;
            for(auto entity : world->getEntities()){
                 // Look for the NinjaControllerComponent (has speed data so far)
                 // and the camera component (in the camera entity). Note that even the nested camera entity will be looped on (2nd if condition)
                 if(auto controllerCandidate = entity->getComponent<NinjaControllerComponent>(); controllerCandidate != nullptr){
                     controller = controllerCandidate;
                 }

                 if(auto cameraCandidate = entity->getComponent<CameraComponent>(); cameraCandidate != nullptr){
                     playerCamera = cameraCandidate;
                 }
                if(controller && playerCamera) break;
            }
            // If there is no entity with both a CameraComponent and a NinjaControllerComponent, we can do nothing so we return
            if(!(controller ) || !(playerCamera)) return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity* entity = controller->getOwner();            // This is Ninja. Any component gives you access to owning entity.

            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;  // Ninja's position.
            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4(); // Put Ninja's position, rotation and scale in one matrix

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),   // Getting Ninja's front direction in world space.
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)), 
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 speed = controller->positionSensitivity;              // It stores speed.
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) speed *= controller->speedupFactor;   // and speed up factor    
            
            // We change the camera position based on the keys WASD/QE
            // Q & E moves the player up and down
            //if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * (deltaTime * speed.y);
            //if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * (deltaTime * speed.y);

            // S & W moves the player back and forth
            //! These will be disabled.
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) position -= front * (deltaTime * speed.z);
            if(app->getKeyboard().isPressed(GLFW_KEY_S)) position += front * (deltaTime * speed.z);

            // A & D moves the player left or right 
            if(app->getKeyboard().justPressed(GLFW_KEY_D)) position -= right * (deltaTime * speed.x);
            if(app->getKeyboard().justPressed(GLFW_KEY_A)) position += right * (deltaTime * speed.x);
            // note that the previous four directions are reversed because by default the camera is rotated to look from behind the ninja.
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

        void imgui(){           // Will be called every freame to provide a slider for camera position and rotation (use camera component to get its own entity = camera)
            if(playerCamera && ImGui::Begin("Ninja")){      // playerCamera is initially null (else get a segmentationf fault)
                ImGui::DragFloat3("Camera position", &playerCamera->getOwner()->localTransform.position.x, -2.0f, 2.0f);
                ImGui::DragFloat3("Camera rotation", &playerCamera->getOwner()->localTransform.rotation.x, -20.0f, 20.0f);
            ImGui::End();
            }
        }
    };

}
