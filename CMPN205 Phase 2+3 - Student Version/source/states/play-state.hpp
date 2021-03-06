#pragma once

#include <application.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/ninja-system.hpp>
#include <systems/collision-system.hpp>
#include <systems/objects-spawning.hpp>

#include <systems/light.hpp>
// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::NinjaSystem ninjaSystem; //= will need its update function for each draw (control the ninja).
    our::MovementSystem movementSystem;

    our::SpawningSystem spawningController;
    our::State state;
    // our::CollisionSystem collisionSystem;

    our::LightSystem lightSystem;
    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        ninjaSystem.enter(getApp(), &renderer, this);         // pass the forward renderer which will be needed in the ninja. this = play_state (can switch from any state to another)
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        spawningController.initialize(size);
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override //= Will be called in application.cpp's run which is responsible for the whole app
    {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        ninjaSystem.update(&world, (float)deltaTime);
        lightSystem.update(&world);
        spawningController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // collisionSystem.detectCollision(&world);
    }


    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
        // clear world
        world.clear();
    }

    void onImmediateGui() override
    { //= gets called in application.cpp every frame
        //= Here, we just draw the camera controller system's gui
        ninjaSystem.imgui();
    }
};