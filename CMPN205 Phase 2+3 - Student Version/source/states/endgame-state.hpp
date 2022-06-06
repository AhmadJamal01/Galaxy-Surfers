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
#include "./play-state.hpp"


// This state shows how to use the ECS framework and deserialization.
class EndgameState: public our::State {
    bool fontLoaded = true;
    int highScore = 0;
    int score = 0;

    void onInitialize() override {
        std::ifstream ifs("source/states/endgame.jsonc");
        auto json = nlohmann::json::parse(ifs);
        ifs.close();
        highScore = json.value("highScore", 0);
        score = json.value("score", 0);



    // Theme Source: https://github.com/ocornut/imgui/issues/707
    if(!fontLoaded) {
        ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/Fonts/Ruda-Bold.ttf", 14.0f);
        ImGui::GetIO().Fonts->Build();
        fontLoaded = true;
    }
        auto &style = ImGui::GetStyle();
        style.FrameRounding = 4.0f;
        style.WindowBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        style.GrabRounding = 4.0f;

        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
        colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
    }

    void onDraw(double deltaTime) override
    {
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void onDestroy() override
    {
    }

    void onImmediateGui() override
    {
         ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoBackground;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoScrollbar;
        bool *open_ptr = (bool *)true;
        ImGui::Begin("Game Over", open_ptr, window_flags);
        ImGui::SetWindowSize(ImVec2((float)500, (float)400));
        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetWindowPos(ImVec2(400.0f, 300.0f));
        std::string message = "Game Over \nThe Galaxy Has been Invaded.\nYour Score is: " + std::to_string(score) +"\n The high score is: " + std::to_string(highScore);
        ImGui::SetWindowFontScale(2.5f);
        ImGui::TextUnformatted(message.c_str());

        if (ImGui::Button("Play Again"))
        {
            getApp()->registerState<Playstate>("main");                 //! Need this here. 
            getApp()->changeState("main");
        }

        if (ImGui::Button("Exit"))
        {
            glfwSetWindowShouldClose(getApp()->getWindow(), GLFW_TRUE);
        }
        ImGui::End();
    }
};
