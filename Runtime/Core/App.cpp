//
// Created by allos on 6/1/2024.
//

#define USING_JOLT

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Objects/Entity.h"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"


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
                .size = {1, 1, 1},
                .model = 1,
                .isPhysical = false
        };

        _entity = new Quack::Entity(player_info);


        // initialize physics character

        _standingShape = RotatedTranslatedShapeSettings({0, 8, 0}, Quat::sIdentity(), new BoxShape({1, 1, 1})).Create().Get();
        _crouchingShape = RotatedTranslatedShapeSettings({0, 8, 0}, Quat::sIdentity(), new BoxShape({1, 1, 1})).Create().Get();

        Ref<CharacterSettings> settings = new CharacterSettings();
        settings->mMaxSlopeAngle = DegreesToRadians(45.0f);
        settings->mLayer = Quack::Layers::MOVING;
        settings->mShape = _standingShape;
        settings->mFriction = 0.5f;
        settings->mSupportingVolume = Plane(Vec3::sAxisY(), -0.3f);

        _character = new Character(settings, RVec3::sZero(), Quat::sIdentity(), 0, &Game::physicsEngine->getSystem());
        _character->AddToPhysicsSystem(EActivation::Activate);
    }
    ~Player() {
        _character->RemoveFromPhysicsSystem();
        delete _entity;
    }

    void preUpdate() {

        //Capture input?
        if (Quack::Input::isControllerPresent(0)) {
            auto vec = Quack::Input::getJoystickAxis(0);

            vec.x = ceil(vec.x * 11) / 11;
            vec.y = ceil(vec.y * 11) / 11;



            if (vec.x > 0.1 || vec.x < -0.1 || vec.y > 0.1 || vec.y < -0.1) {
                vec.x *= speed;
                vec.y *= speed;
                _character->SetLinearVelocity({vec.x, _character->GetLinearVelocity().GetY(), vec.y});
                spdlog::info("VEC: {} {} ", vec.x, vec.y);
            }

        }
    }

    void postUpdate() {

        _entity->setPosition(Quack::Math::Vector3{_character->GetPosition()});
        _entity->render(Game::engine);
        _character->PostSimulation(0.05f);
    }

private:
    Quack::Entity* _entity = nullptr;

    Ref<Character> _character;
    RefConst<Shape> _standingShape;
    RefConst<Shape> _crouchingShape;

    float speed = 5.0f;
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

        Game::physicsEngine->update();

        Level::floor->render(Game::engine);
        Level::floor->updatePhysics(*Game::physicsEngine);
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
