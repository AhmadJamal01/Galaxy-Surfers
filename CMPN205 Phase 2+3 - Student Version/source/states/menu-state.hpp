#pragma once

#include <application.hpp>

#include <imgui-utils/utils.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/ninja-system.hpp>
#include <systems/light.hpp>
// This state shows how to use the ECS framework and deserialization.
class MenuState: public our::State {

    void onInitialize() override {
       
       
    }

    void onDraw(double deltaTime) override {
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void onDestroy() override {
    }

     void onImmediateGui() override{

        ImGui::Begin("Welcome to Subway");
        ImGui::SetWindowSize(ImVec2((float)500, (float)400));
        ImGui::SetWindowFontScale(1.5f); 
        ImGui::Text("Menu"); 
        if (ImGui::Button("New Game")){
                getApp()->changeState("main");
            }
        if (ImGui::Button("Exit")){
            glfwSetWindowShouldClose(getApp()->getWindow(), GLFW_TRUE);
        }
        ImGui::End();
    }
};
