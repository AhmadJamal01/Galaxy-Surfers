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
#include <GLFW/glfw3.h>

#include "./collision-system.hpp"


namespace our
{ //= similar to free-camera-controller.
    //= The ninja-system is responsible for moving every entity which contains a NinjaControllerComponent.

    class NinjaSystem
    {
        Application *app; // The application in which the state runs (app holds general data, controlling events)
        State *state;
        bool mouse_locked = false;            // Is the mouse locked (e.g. unlock it in main menu only)
        CameraComponent *playerCamera;        //=We need external acess to the camera component of the player for ImGUI
        NinjaControllerComponent *controller; //= For control of player and IMGui
        ForwardRenderer *postprocessControl;  //=to use the togglePostProcessing function implemented in the forward renderer (will be set upon enter).
        MovementComponent *playerMovement;    //= to slow down automatic movement upon collision.
        enum PostprocessingEffect
        {
            def,                              // The default shader (no postprocessing)
            cartoonize,
            chromatic_aberration,
            convolution,
            fisheye,
            grayscale,
            radial_blur,
            vignette
        };
        int index; //= to rotate postprocessing effects with each press of p (changePostProcessingFunction).
        //= Time variables are to control the score and collision exhausion
        float curr_time;
        float start_time;               
        float collision_time = 0.0f;
        //= Score increases spontaneously with time meanwhile extra_score is bonus score due to speed up or monkey hit (both get added)
        int score;
        int extra_score;
        int lives = 3;
        float exhaustion_time = 3.0f;      //= For how many seconds should the ship be exhausted after a collision          
        float slow_down_effect = 1.0f;    //= By how much should it be slowed down during exhaustion time (1.0 means no slowdown)
        bool hurt = false;                  //= if collided with meteor then it becomes true until exhaustion time is over.
        bool monkey_hit = false;            //= if collided with monkey then it becomes true until exhaustion time is over.
        float monkey_buffer = 0.0f;        //= delay before we can feel rocks again again descending.
        our::CollisionSystem collisionSystem;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app, ForwardRenderer *FR, State *state)
        { //= This and the ninja's update function will be invoked in playe-state  (initialize and ondraw)
            this->app = app;
            this->postprocessControl = FR;          //=to set postprocessControl as well.
            this->state = state;                    //= to be able to change the state.
            this->start_time = (float)glfwGetTime();
            this->curr_time = (float)glfwGetTime() - start_time;
            this->extra_score = 0;
        }

        // This should be called every frame to update all entities containing a NinjaControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a NinjaControllerComponent
            // As soon as we find one, we break
            curr_time = (float)glfwGetTime() - start_time;
            score = static_cast<int>(curr_time) + extra_score;      // Both the time score and the extra score make up the score as mentioned above
            for (auto entity : world->getEntities())
            {
                //= Look for the NinjaControllerComponent (has speed data so far)
                //= and the camera component (in the camera entity). Note that even the nested camera entity will be looped on (2nd if condition)
                if (auto controllerCandidate = entity->getComponent<NinjaControllerComponent>(); controllerCandidate != nullptr)
                {
                    controller = controllerCandidate; //= The Ninja's controller is what will help move it.
                }

                if (auto cameraCandidate = entity->getComponent<CameraComponent>(); cameraCandidate != nullptr)
                {
                    playerCamera = cameraCandidate; //= For Dear ImGui to access the camera component of the player.
                }
                if (controller && playerCamera)
                    break;
            }
            // If there is no entity with both a CameraComponent and a NinjaControllerComponent, we can do nothing so we return
            if (!(controller) || !(playerCamera))
                return;

            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = controller->getOwner();        //= This is Ninja. Any component gives you access to owning entity.

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;      // Ninja's position.
            glm::mat4 matrix = entity->localTransform.toMat4();         //= Put Ninja's position, rotation and scale in one matrix

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),       // Getting Ninja's front direction in world space (so we can move it along that).
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 speed = controller->positionSensitivity; //= It stores speed.
            //! Shift is a cheat code.
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                speed *= controller->speedupFactor; // and speed up factor

        
            if (app->getKeyboard().justPressed(GLFW_KEY_P))
            {
                // Cycle through postprocessing effects
                postprocessControl->setPostProcessing((++index) % 7);
            }

            //! Ninja Forward Direction.
            if (app->getKeyboard().isPressed(GLFW_KEY_W) && !hurt && !monkey_hit)
            {
                this->extra_score += 1;
                position -= front * (deltaTime * speed.z);
                playerCamera->getOwner()->localTransform.position.z = -155.0f;          // Cammera immersive effect
                if (app->getKeyboard().justPressed(GLFW_KEY_W))
                {
                    index = radial_blur;                                                // speed effect
                    postprocessControl->setPostProcessing(index);
                }
            }
            else                                                                        // Reset Camera effects
            {
                if (app->getKeyboard().justReleased(GLFW_KEY_W) && !hurt && !monkey_hit)
                {
                    index = def;
                    postprocessControl->setPostProcessing(index);
                }
                playerCamera->getOwner()->localTransform.position.z = -125.0f;
            }


            //Fixed points (to prevent vertical motion during rotation)
            entity->localTransform.rotation.z*=0.9;
            entity->localTransform.rotation.y*=0.1;
            entity->localTransform.rotation.x*=0.1;
            entity->localTransform.position.y*=0.1;
            //= Motion along the horizontal direction along with rotation
            if(app->getKeyboard().isPressed(GLFW_KEY_D)) 
            {   
                if( entity->localTransform.position.x > -4.0f)
                {
                position -= right * (deltaTime * speed.x * 0.3f);
                }
                entity->localTransform.rotation.z += 0.01;
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_A))
            {
                if (entity->localTransform.position.x < 4.0f)
                {
                    position += right * (deltaTime * speed.x * 0.3f);
                }
                entity->localTransform.rotation.z -= 0.01;
            
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_S))
            {
                        
            }
            // note that the previous four directions are reversed because by default the camera is rotated to look from behind the ninja.
            
            //! Collision Logic
            Entity *collidedWith = collisionSystem.detectCollision(world);
            if (collidedWith && collidedWith->name != "bonus" && !monkey_hit && (float)glfwGetTime() - start_time > 0.7  )                  //= collision with asteroid
            {
                collidedWith->visible = false;                                  // Asteroids collided with should cease to exist
                collision_time = (float)glfwGetTime();                          // Exhaustion time counts from now
                position += front * (deltaTime * speed.z) * 76.0f;              // Push back the Ninja
                postprocessControl->setPostProcessing(chromatic_aberration);    // The ship gets damaged
                lives--;                                                        // los
                slow_down_effect = 0.09f;                                       // Set the slow down effect
                hurt = true;                                                    // Set the hurt state

            }
            else if (collidedWith && collidedWith->name == "bonus")             //= collision with bonus
            {
                collidedWith->visible = false;
                monkey_hit = true;
                extra_score += (collidedWith->getComponent<CollisionComponent>()->bonus);   //= An increase in the player's score (hmm.)
                collision_time = (float)glfwGetTime();
                postprocessControl->setPostProcessing(convolution);
            }

            //! Exhaustion time logic
            if ((float)glfwGetTime() - collision_time > exhaustion_time)
            {
                postprocessControl->setPostProcessing(index);                               //= Once exhaustion is over go back to the previous postprocessing
                slow_down_effect = 1.0f;
                hurt = false;
                monkey_hit = false;
                
            }
            else
            {
                if (hurt)
                    position += front * (deltaTime * speed.z) * (1.0f - slow_down_effect);      //like multiplying the forward speed by slow_down_effect
                if (monkey_hit && (float)glfwGetTime() - collision_time < exhaustion_time*0.8)
                    {position += up * (deltaTime * 85.0f * speed.y * 0.3f);    
                    speed *= controller->speedupFactor;
                    position -= front * (deltaTime * speed.z);
                    }
            }

            //! Endgame Logic 
             if(lives == 0){
                    state-> getApp()->changeState("endgame-state");
                    std::ifstream ifs("source/states/endgame.jsonc");           // Read the score json (has last score and high score)
                    auto json = nlohmann::json::parse(ifs);
                    ifs.close();
                    int highScore = json.value("highScore", 0);

                    if (score > highScore)  highScore = score;  // Compare json highscore with current score

                    std::ofstream ofs("source/states/endgame.jsonc");
                    nlohmann::json j = {{"highScore", highScore}, {"score", score}};    // Write the new highscore and score to JSON
                    ofs << j;
                    ofs.close();
                }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

        void imgui()
        { //= [OLD]: Will be called every frame in application.cpp to provide a slider for camera position and rotation (use camera component to get its own entity = camera)
          //= A transparent ImGui window with no artifacts for score on top right.
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoBackground;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoScrollbar;
            bool *open_ptr = (bool *)true;
            if (playerCamera && ImGui::Begin("Ninja", open_ptr, window_flags))          // playerCame is redundant here (was needed in old slider)
            { 
                ImGui::SetWindowPos(ImVec2(app->getWindowSize().x - 200.0f, 0.0f));                 // Set position to top right
                std::string player_score = "score: " + std::to_string((int)this->score);            // Set score
                std::string hearts = "Lives: " + std::to_string(this->lives);                       // Set lives
                ImGui::SetWindowFontScale(2.5f);
                ImGui::TextUnformatted(player_score.c_str());
                ImGui::TextUnformatted(hearts.c_str());
                ImGui::End();
            }
        }
    };

}
