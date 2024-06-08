//
// Created by allos on 6/1/2024.
//

#define USING_JOLT

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Objects/Entity.h"


namespace Game {
    VulkanEngine engine;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;


    Quack::Entity* floor;
    Quack::Entity* testEntity;
}

void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});
    Quack::Input::setTargetWindow(*window);

    Game::engine.Init(window->getRawWindow(), true);


    // Temp model
    std::string structurePath = {"..//Assets/basicmesh.glb"};
    auto structureFile = VkLoader::loadGltf(&Game::engine, structurePath);
    // just a check, not necessary
    assert(structureFile.has_value());
    Game::engine.loadedScenes[1] = *structureFile;


    //Setup camera start.
    Game::camera = &Game::engine.getMainCamera();
    Game::camera->position.z = 20;
    Game::camera->position.y = 3;


    // load Physics
    Game::engineCreationInfo = new Quack::PhysicsEngineCreationInfo{};
    Game::physicsEngine = new Quack::PhysicsEngine(*Game::engineCreationInfo);

    // Beautiful code, fight me >:(
    Quack::EntityCreationInfo test_info {
            .model = 1,
            .isPhysical = true,
            .bodyCreationInfo = {
                    .position = {0, 6, 0},
                    .rotation = Quat::sIdentity(),
                    .shape = new SphereShape(1.0f),
                    .shouldActivate = EActivation::Activate,
                    .motionType = EMotionType::Dynamic,
                    .layer = Quack::Layers::MOVING,
                    .physicsEngine = Game::physicsEngine
            }
    };

    Quack::EntityCreationInfo floor_info {
            .model = 1,
            .isPhysical = true,
            .bodyCreationInfo = {
                    .position = {0, -5, 0},
                    .rotation = Quat::sIdentity(),
                    .shape = new BoxShape(RVec3(100.f, 1.0f, 100.f)),
                    .shouldActivate = EActivation::DontActivate,
                    .motionType = EMotionType::Static,
                    .layer = Quack::Layers::NON_MOVING,
                    .physicsEngine = Game::physicsEngine
            }
    };


    Game::testEntity = new Quack::Entity(test_info);
    Game::floor = new Quack::Entity(floor_info);



}

void App::run() {
    int count;
    while (!window->shouldShutdown()) {

        if (Quack::Input::isKeyPressed(Quack::Key::A)) {
            spdlog::info("A KEY PRESSED");
        }

       /* if (Quack::Input::isButtonPressed(0, 2)) {
            spdlog::info("A BUTTON PRESSED");
        }*/



        Game::engine.updateScene();

        Game::testEntity->render(Game::engine);
        Game::floor->render(Game::engine);



            //Game::testEntity->setPosition(Game::physicsEngine->getSpherePos());

            Game::testEntity->updatePhysics(*Game::physicsEngine);
            Game::floor->updatePhysics(*Game::physicsEngine);

            Game::physicsEngine->update();



        Game::engine.Run();

        window->update();
    }

}

void App::cleanup() {
    delete Game::testEntity;
    delete Game::floor;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;


    Game::engine.CleanUp();
    delete window;
}
