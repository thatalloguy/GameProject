//
// Created by allos on 6/1/2024.
//

#define USING_JOLT

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Objects/Entity.h"
#include "imgui.h"
#include "FishingManager.h"


namespace Game {
    VulkanEngine engine;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;


    FishingManager* fishingManager;

    float deltaTime = 0.0f;
}


namespace Level {
    Quack::Entity* floor;
    Player* player;
}


void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});
    Quack::Input::setTargetWindow(*window);

    Game::engine.Init(window->getRawWindow(), true);


    // Temp model
    std::string structurePath = {"..//Assets/basicmesh.glb"};
    auto structureFile = VkLoader::loadGltf(&Game::engine, structurePath);
    auto testFile = VkLoader::loadGltf(&Game::engine, "..//Assets/cube.glb");
    auto sphereFile = VkLoader::loadGltf(&Game::engine, "..//Assets/sphere.glb");
    // just a check, not necessary
    assert(structureFile.has_value());
    Game::engine.loadedScenes[1] = *structureFile;
    Game::engine.loadedScenes[2] = *testFile;
    Game::engine.loadedScenes[3] = *sphereFile;


    //Setup camera start.
    Game::camera = &Game::engine.getMainCamera();
    Game::camera->position.z = 20;
    Game::camera->position.y = 3;


    // load Physics
    Game::engineCreationInfo = new Quack::PhysicsEngineCreationInfo{};
    Game::physicsEngine = new Quack::PhysicsEngine(*Game::engineCreationInfo);

    // Beautiful code, fight me >:(
    Quack::EntityCreationInfo floor_info {
            .position = {0, -5, 0},
            .size = {100, 1.0f, 100},
            .model = 2,
            .isPhysical = true,
            .bodyCreationInfo = {
                    .position = {0, -5, 0},
                    .rotation = Quat::sIdentity(),
                    .shape = new BoxShape(RVec3(100.f, 10.0f, 100.f)),
                    .shouldActivate = EActivation::DontActivate,
                    .motionType = EMotionType::Static,
                    .layer = Quack::Layers::NON_MOVING,
                    .physicsEngine = Game::physicsEngine
            }
    };

    Quack::EntityCreationInfo cube_info {
            .position = {-10, -1.5f, 20},
            .size = {4, 1.0f, 4},
            .model = 2,
            .isPhysical = false,

    };


    Level::player = new Player(Game::engine.getMainCamera(), *Game::physicsEngine);
    Level::floor = new Quack::Entity(floor_info);


    Game::fishingManager = new FishingManager(Game::engine, *Level::player, *Game::physicsEngine);


}

void App::run() {
    int count;

    std::chrono::steady_clock::time_point last;

    Game::engine.imguiFunc.pushFunction([=](){

        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
       ImGui::Begin("Duck Watcher Debug");

       ImGui::SliderFloat("player speed", &Level::player->speed, 0.1f, 30.0f);
       ImGui::Text("PlayerPos: %f | %f | %f", Level::player->position.x, Level::player->position.y, Level::player->position.z);
       ImGui::Text("PlayerState: %i", static_cast<unsigned int>(Level::player->state));

       ImGui::End();
    });

    while (!window->shouldShutdown()) {
        Game::engine.updateScene();


        Game::physicsEngine->update();

        Level::floor->render(Game::engine);
        Level::floor->updatePhysics(*Game::physicsEngine);
        Level::player->update();

        Game::fishingManager->Update(Game::deltaTime);


        Game::engine.Run();

        window->update();
        auto now = std::chrono::steady_clock::now();
        Game::deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() / 1000000.0f;
        last = now;

    }

}

void App::cleanup() {
    delete Level::player;
    delete Level::floor;
    delete Game::fishingManager;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;


    Game::engine.CleanUp();
    delete window;
}
