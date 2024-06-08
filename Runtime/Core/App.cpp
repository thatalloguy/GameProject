//
// Created by allos on 6/1/2024.
//

#define USING_JOLT

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Objects/Entity.h"
#include "Jolt/Physics/Character/Character.h"


namespace Game {
    VulkanEngine engine;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;
}

class Player{

public:
    Player(){

        auto player_info = Quack::EntityCreationInfo{
                .position = {0, 6, 0},
                .model = 1,
                .isPhysical = false
        };

        _entity = new Quack::Entity(player_info);


        // initialize physics character

        Ref<CharacterSettings> settings = new CharacterSettings();
        settings->mMaxSlopeAngle = DegreesToRadians(45.0f);
        settings->mLayer = Quack::Layers::MOVING;
        settings->mShape = nullptr;

    }
    ~Player() {
        delete _entity;
    }

    void preUpdate() {

        //Capture input?
        if (Quack::Input::isControllerPresent(0)) {
            Quack::Math::Vector3 out{0, 0, 0};


            auto vec = Quack::Input::getJoystickAxis(0);

            spdlog::info("VEC: {} {} ", ceil(vec.x * 10) / 10, ceil(vec.y * 10) / 10);

        }
    }

    void postUpdate() {


        _entity->render(Game::engine);
        _entity->updatePhysics(*Game::physicsEngine);
    }

private:
    Quack::Entity* _entity = nullptr;

    Character* _character = nullptr;
};


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
    // just a check, not necessary
    assert(structureFile.has_value());
    Game::engine.loadedScenes[1] = *structureFile;
    Game::engine.loadedScenes[2] = *testFile;


    //Setup camera start.
    Game::camera = &Game::engine.getMainCamera();
    Game::camera->position.z = 20;
    Game::camera->position.y = 3;


    // load Physics
    Game::engineCreationInfo = new Quack::PhysicsEngineCreationInfo{};
    Game::physicsEngine = new Quack::PhysicsEngine(*Game::engineCreationInfo);

    // Beautiful code, fight me >:(
    Quack::EntityCreationInfo floor_info {
            .size = {100, 1, 100},
            .model = 2,
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


    Level::player = new Player();
    Level::floor = new Quack::Entity(floor_info);



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

        Level::player->preUpdate();
        Level::floor->updatePhysics(*Game::physicsEngine);

        Game::physicsEngine->update();

        Level::floor->render(Game::engine);
        Level::player->postUpdate();


        Game::engine.Run();

        window->update();
    }

}

void App::cleanup() {
    delete Level::player;
    delete Level::floor;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;


    Game::engine.CleanUp();
    delete window;
}
