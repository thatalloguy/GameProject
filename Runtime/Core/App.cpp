//
// Created by allos on 6/1/2024.
//

#define USING_JOLT
#pragma once

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Objects/Entity.h"
#include "imgui.h"
#include "FishingManager.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/UI/BookUI.h"
#include "Audio/AudioEngine.h"
#include "MapLoader.h"


// Core Game Systems
namespace Game {
    VulkanEngine renderer;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;
    Quack::AudioEngine* audioEngine;

    Loader::MapLoader* mapLoader;

    Quack::SoundID pianoId;

    FishingManager* fishingManager;
    BookUI* bookUI;

    float deltaTime = 0.0f;

    void loadAssets() {
        // Temp model
        std::string structurePath = {"../../Assets/basicmesh.glb"};
        auto structureFile = VkLoader::loadGltf(&Game::renderer, structurePath);
        auto testFile = VkLoader::loadGltf(&Game::renderer, "../../Assets/cube.glb");
        auto sphereFile = VkLoader::loadGltf(&Game::renderer, "../../Assets/sphere.glb");
        auto bobberFile = VkLoader::loadGltf(&Game::renderer, "../../Assets/bobber.glb");
        // just a check, not necessary
        assert(structureFile.has_value());
        Game::renderer.loadedScenes[11] = *structureFile;
        Game::renderer.loadedScenes[20] = *testFile;
        Game::renderer.loadedScenes[30] = *sphereFile;
        Game::renderer.loadedScenes[40] = *bobberFile;
        Game::renderer.loadedScenes[50] = *VkLoader::loadGltf(&Game::renderer, "../../Assets/Chest.glb");;


        Game::mapLoader->loadMap();
    }
    void initPhysics() {

        // load Physics
        Game::engineCreationInfo = new Quack::PhysicsEngineCreationInfo{};
        Game::physicsEngine = new Quack::PhysicsEngine(*Game::engineCreationInfo);

    }
}


namespace Level {
    Quack::Entity* floor;
    Player* player;
    Quack::Entity* chest;
}


void App::init() {

    //initialize the window
    auto windowCreationData = Quack::WindowCreationData {
      .title = "Duck Watchers Remake"
    };

    window = new Quack::Window(windowCreationData);


    Quack::Input::setTargetWindow(*window);

    Game::renderer.Init(window->getRawWindow(), false);

    Game::initPhysics();


    Game::mapLoader = new Loader::MapLoader(Game::renderer, *Game::physicsEngine);


    Game::loadAssets();

    //


    /////// Load the Test Scene

    //Setup camera start.
    Game::camera = &Game::renderer.getMainCamera();
    Game::camera->position.z = 20;
    Game::camera->position.y = 3;


    // Beautiful code, fight me >:(
    Quack::EntityCreationInfo floor_info {
            .position = {0, -5, 0},
            .size = {100, 1.0f, 100},
            .model = 20,
            .isPhysical = false,
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
            .model = 20,
            .isPhysical = false,

    };

    Quack::EntityCreationInfo Chest_info {
            .position = {20, -2, 20},
            .size = {1, 1.0f, 1},
            .model = 50,
            .isPhysical = false,

    };


    Level::player = new Player(Game::renderer.getMainCamera(), *Game::physicsEngine);
    Level::floor = new Quack::Entity(floor_info);
    Level::chest = new Quack::Entity(Chest_info);


    Game::fishingManager = new FishingManager(Game::renderer, *Level::player, *Game::physicsEngine);
    Game::bookUI = new BookUI(Game::renderer);

    Game::audioEngine = new Quack::AudioEngine();

    Game::audioEngine->Init();

    Game::pianoId = Game::audioEngine->registerSound({"../../Assets/Audio/bluebonnet_in_b_major_looped.wav", true});





}

void App::run() {
    std::chrono::steady_clock::time_point last;

    Game::renderer.debugRenderFuncs.pushFunction([=](){

        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
       ImGui::Begin("Duck Watcher Debug");

       ImGui::SliderFloat("player speed", &Level::player->speed, 0.1f, 30.0f);
       ImGui::Text("PlayerPos: %f | %f | %f", Level::player->position.x, Level::player->position.y, Level::player->position.z);
       ImGui::Text("PlayerState: %i", static_cast<unsigned int>(Level::player->state));

       ImGui::End();
    });

    bool toggle = false;
    bool booktoggle = false;

    Game::audioEngine->playSound(1);
    Game::audioEngine->setSoundPosition(Game::pianoId, Level::chest->position);



    while (!window->shouldShutdown()) {
        Game::renderer.updateScene();


        Game::physicsEngine->update();

        Level::floor->render(Game::renderer);
        Level::chest->render(Game::renderer);
        Game::mapLoader->renderMap();
        Level::floor->updatePhysics(*Game::physicsEngine);
        Level::chest->updatePhysics(*Game::physicsEngine);
        Level::player->update(Game::deltaTime);

        //via f3 you can toggle debug menu
        if (Quack::Input::isKeyPressed(Quack::Key::F3) && toggle) {
            Game::renderer.displayDebugMenu = !Game::renderer.displayDebugMenu;
            toggle = false;
        } else if (!Quack::Input::isKeyPressed(Quack::Key::F3)) {
            toggle = true;
        }

        if (Quack::Input::isKeyPressed(Quack::Key::F5) && booktoggle) {
            Game::bookUI->shouldRender(!Game::bookUI->isRendering());
            booktoggle = false;
        } else if (!Quack::Input::isKeyPressed(Quack::Key::F5)) {
            booktoggle = true;
        }

        Game::fishingManager->Update(Game::deltaTime);

        if (Game::renderer.displayDebugMenu) {
            Quack::Input::setMouseMode(Quack::MouseMode::Normal);
        } else {
            Quack::Input::setMouseMode(Quack::MouseMode::Disabled);
        }

        Game::renderer.Run();

        window->update();

        //Update the delta time
        auto now = std::chrono::steady_clock::now();
        Game::deltaTime = (float) std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() / 1000000.0f;
        last = now;


        auto dir = glm::normalize(glm::vec3(Game::camera->getRotationMatrix() * glm::vec4(Level::chest->position.x - Level::player->position.x  , Level::player->position.y - Level::chest->position.y, Level::player->position.z - Level::chest->position.z, 0)));
        Quack::Math::Vector3 soundDir{0, 0, 0};
        soundDir.x = dir.x;
        soundDir.y = dir.y;
        soundDir.z = dir.z;

        Game::audioEngine->updateSound(Game::pianoId, Level::player->position, soundDir);

    }

}

void App::cleanup() {
    Game::audioEngine->CleanUp();

    delete Level::player;
    delete Level::chest;
    delete Level::floor;


    delete Game::fishingManager;
    delete Game::mapLoader;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;
    delete Game::bookUI;
    delete Game::audioEngine;

    Game::renderer.CleanUp();

    delete window;
}
