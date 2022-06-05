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
#include <string>  
// include glfw

#include "./collision-system.hpp"

namespace our
{   //= similar to free-camera-controller.
    //= The ninja-system is responsible for moving every entity which contains a NinjaControllerComponent.

    class NinjaSystem {
        Application* app;               // The application in which the state runs (app holds general data, controlling events)
        bool mouse_locked = false;      // Is the mouse locked (e.g. unlock it in main menu only)
        CameraComponent * playerCamera;           //=We need external acess to the camera component of the player for ImGUI
        NinjaControllerComponent *controller;         //= For control of player and IMGui
        ForwardRenderer * postprocessControl;    //=to use the togglePostProcessing function implemented in the forward renderer (will be set upon enter).
        enum PostprocessingEffect {def, cartoonize, chromatic_aberration, convolution, fisheye, grayscale, radial_blur, vignette};
        int index;      //= to rotate postprocessing effects with each press of p (changePostProcessingFunction).
        float curr_time;
        float start_time;
        int score;
        int extra_score;
        int lives = 3;

        our::CollisionSystem collisionSystem;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app, ForwardRenderer * FR){  //= This and the ninja's update function will be invoked in playe-state  (initialize and ondraw)   
            this->app = app;
            this->postprocessControl = FR;                  //=to set postprocessControl as well.
            this->start_time = (float)glfwGetTime();
            this->curr_time = (float)glfwGetTime() - start_time;
            this->extra_score= 0;
        }

        // This should be called every frame to update all entities containing a NinjaControllerComponent
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a NinjaControllerComponent
            // As soon as we find one, we break
            curr_time = (float)glfwGetTime() - start_time;
            score = static_cast<int>(curr_time) + extra_score;

            for(auto entity : world->getEntities()){
                 //= Look for the NinjaControllerComponent (has speed data so far)
                 //= and the camera component (in the camera entity). Note that even the nested camera entity will be looped on (2nd if condition)
                 if(auto controllerCandidate = entity->getComponent<NinjaControllerComponent>(); controllerCandidate != nullptr){
                     controller = controllerCandidate;          //= The Ninja's controller is what will help move it.
                 }

                 if(auto cameraCandidate = entity->getComponent<CameraComponent>(); cameraCandidate != nullptr){
                     playerCamera = cameraCandidate;            //= For IMGUI to access the camera component of the player.
                 }
                if(controller && playerCamera) break;
            }
            // If there is no entity with both a CameraComponent and a NinjaControllerComponent, we can do nothing so we return
            if(!(controller ) || !(playerCamera)) return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity* entity = controller->getOwner();            //= This is Ninja. Any component gives you access to owning entity.

            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;  // Ninja's position.
            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4(); //= Put Ninja's position, rotation and scale in one matrix

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),   // Getting Ninja's front direction in world space.
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)), 
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 speed = controller->positionSensitivity;              //= It stores speed.
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) speed *= controller->speedupFactor;   // and speed up factor    

            // We change the camera position based on the keys WASD/QE
            // Q & E moves the player up and down
            //if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * (deltaTime * speed.y);
            //if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * (deltaTime * speed.y);
            if(app->getKeyboard().justPressed(GLFW_KEY_P)) {
            // Cycle through postprocessing effects    
            postprocessControl->choosePostProcessing((++index) % 7);
            }


            // S & W moves the player back and forth
            //! Ninja Controls.
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) 
            {
                this->extra_score += 1;
                position -= front * (deltaTime * speed.z );
                playerCamera->getOwner()->localTransform.position.z = -155.0f;
                if(app->getKeyboard().justPressed(GLFW_KEY_W)) {
                    postprocessControl->choosePostProcessing(radial_blur);
                }
                
            }
            else{
                 if(app->getKeyboard().justReleased(GLFW_KEY_W)) {
                    postprocessControl->choosePostProcessing(def);
                }
                playerCamera->getOwner()->localTransform.position.z = -125.0f;
            }

            


            // A & D moves the player left or right 

            if(app->getKeyboard().isPressed(GLFW_KEY_D)) 
            {   
                if( entity->localTransform.position.x > -4.0f){
                position -= right * (deltaTime * speed.x * 0.3f);
                }
                //if( entity->localTransform.rotation.z > glm::radians(-5.0f)){
                //entity->localTransform.rotation.z -= glm::radians(15.0f) * deltaTime;
                //}
            }   


            if(app->getKeyboard().isPressed(GLFW_KEY_A)) 
            {
                if( entity->localTransform.position.x < 4.0f){
                position += right * (deltaTime * speed.x * 0.3f);
                }
                //if( entity->localTransform.rotation.z < glm::radians(5.0f)){
                //entity->localTransform.rotation.z += glm::radians(15.0f) * deltaTime;
                //}
            }
            // note that the previous four directions are reversed because by default the camera is rotated to look from behind the ninja.
        
            if(collisionSystem.detectCollision(world)){
                position += front * (deltaTime * speed.z)*200.0f;
                postprocessControl->choosePostProcessing(chromatic_aberration);                
                lives--;
                if(lives<=0){
                    std::cout << "You Lose" << std::endl;
                }
                std::cout << "Lives: " << lives << std::endl;
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

        void imgui(){           //= Will be called every frame in application.cpp to provide a slider for camera position and rotation (use camera component to get its own entity = camera)
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoBackground;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            // etc.
            bool * open_ptr = (bool*) true;
            if(playerCamera && ImGui::Begin("Ninja", open_ptr, window_flags)){      // playerCamera is initially null (else get a segmentationf fault)
                //ImGui::DragFloat3("Camera position", &playerCamera->getOwner()->localTransform.position.x, -2.0f, 2.0f);
                //ImGui::DragFloat3("Camera rotation", &playerCamera->getOwner()->localTransform.rotation.x, -20.0f, 20.0f);
                //ImGui::DragFloat3("Player position", &controller->getOwner()->localTransform.position.x, -2.0f, 2.0f);
                //ImGui::DragFloat3("Player scale", &controller->getOwner()->localTransform.scale.x, -2.0f, 2.0f);
                // Set imgui window position to top right
                ImGui::SetWindowPos(ImVec2(1100, 0));            // Will be back.
                std::string player_score = "score: " + std::to_string(this->score);
                std::string hearts = (this->lives == 3)? "Lives: ♥ ♥ ♥ " : (this->lives == 2)? "Lives: ♥ ♥ " : "Lives: ♥ ";
                ImGui::SetWindowFontScale(2.5f); 
                ImGui::TextUnformatted(player_score.c_str());
                ImGui::TextUnformatted(hearts.c_str());
                ImGui::End();
                
            }
        }
    };

}
