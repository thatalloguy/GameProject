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
#include "imgui.h"


namespace Game {
    VulkanEngine engine;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;


    float deltaTime = 0.0f;
}

class Player  {

public:
    Player(Camera& camera) : _camera(camera) {

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
        settings->mFriction = 0.1f;
        settings->mMass = 100.0f;
        settings->mGravityFactor = 2.0f;
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

    }

    void postUpdate() {
        updateCamera();

        auto vec = getMovement();


        _character->SetLinearVelocity({vec.x, vec.y, vec.z});

        _entity->setPosition(Quack::Math::Vector3{_character->GetPosition()});

        _character->PostSimulation(0.05f);
    }


    float speed = 10.0f;
    const float sensitivity = 0.02f;
    float playerHeight = 3.0f;
    float jumpForce = 3.0f;

    Quack::Entity* _entity = nullptr;
private:
    void updateCamera() {

        auto position = _character->GetPosition();

        _camera.position.x = position.GetX();
        _camera.position.y = position.GetY() + playerHeight;
        _camera.position.z = position.GetZ();

    };


    Quack::Math::Vector3 getMovement() {
        auto currentVel = _character->GetLinearVelocity();
        Quack::Math::Vector3 out = {currentVel.GetX(), currentVel.GetY(), currentVel.GetZ()};

        // controller
        if (Quack::Input::isControllerPresent(0)) {
            auto vec = Quack::Input::getJoystickAxis(0);

            vec.x = ceil(vec.x * 11) / 11;
            vec.y = ceil(vec.y * 11) / 11;
            vec.z = ceil(vec.z * 11) / 11;
            vec.w = ceil(vec.w * 11) / 11;



            if (vec.x > 0.1 || vec.x < -0.1 || vec.y > 0.1 || vec.y < -0.1) {
                vec.x *= speed;
                vec.y *= speed;
                out = {vec.x, _character->GetLinearVelocity().GetY(), vec.y};
            }

            // rotating
            if (vec.z > 0.1 || vec.z < -0.1 || vec.w > 0.1 || vec.w < -0.1) {
                vec.z *= sensitivity;
                vec.w *= sensitivity;

                _camera.yaw += vec.z;

                //todo fix ground camera clip
                _camera.pitch += vec.w;
                _camera.pitch = max(-0.7f, min(0.7f, _camera.pitch));
            }

            if (_character->GetGroundState() != Character::EGroundState::InAir && Quack::Input::isButtonPressed(0, 0)) {
                out.y += jumpForce;
            }
            if (_character->GetGroundState() != Character::EGroundState::InAir && Quack::Input::isButtonPressed(0, 2)) {
                speed = 17.5f;
                _camera.fov = 94.0f;
            } else {
                speed = 10.0f;
                _camera.fov = 90.0f;
            }


        }
        //todo fix keyboard controller
        else {

            //Keyboard.
            if (Quack::Input::isKeyPressed(Quack::Key::A)) {
                out.x = -10 * speed * Game::deltaTime;
            }
            if (Quack::Input::isKeyPressed(Quack::Key::D)) {
                out.x = 10 * speed * Game::deltaTime;
            }
            if (Quack::Input::isKeyPressed(Quack::Key::W)) {
                out.z = -10 * speed * Game::deltaTime;
            }
            if (Quack::Input::isKeyPressed(Quack::Key::S)) {
                out.z = 10 * speed * Game::deltaTime;
            }

            if (Quack::Input::isKeyPressed(Quack::Key::UP)) {
                _camera.pitch -= 0.03f * Game::deltaTime;
                _camera.pitch = max(-0.7f, min(0.7f, _camera.pitch));
            }

            if (Quack::Input::isKeyPressed(Quack::Key::DOWN)) {
                _camera.pitch += 0.03f * Game::deltaTime;
                _camera.pitch = max(-0.7f, min(0.7f, _camera.pitch));
            }

            if (Quack::Input::isKeyPressed(Quack::Key::LEFT)) {
                _camera.yaw -= 0.03f;
            }
            if (Quack::Input::isKeyPressed(Quack::Key::RIGHT)) {
                _camera.yaw += 0.03f;
            }
        }




        auto dir = glm::vec3(_camera.getRotationMatrix() * glm::vec4(out.x, out.y, out.z, 0));
        out.x = dir.x;
        out.y = out.y; // dont change y.
        out.z = dir.z;


        return out;
    }

    Ref<Character> _character;
    RefConst<Shape> _standingShape;
    RefConst<Shape> _crouchingShape;

    Camera& _camera;

};


namespace Level {
    Quack::Entity* floor;
    Player* player;
    Quack::Entity* cube;
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

    Quack::EntityCreationInfo cube_info {
            .position = {10, 1, 10},
            .size = {4, 4.0f, 4},
            .model = 2,
            .isPhysical = false,

    };


    Level::player = new Player(Game::engine.getMainCamera());
    Level::floor = new Quack::Entity(floor_info);
    Level::cube = new Quack::Entity(cube_info);


}

void App::run() {
    int count;

    std::chrono::steady_clock::time_point last;

    Game::engine.imguiFunc.pushFunction([=](){
       ImGui::Begin("Duck Watcher Debug");

       ImGui::SliderFloat("player speed: %f", &Level::player->speed, 0.1f, 30.0f);
       ImGui::Text("PlayerPos: %f | %f | %f", Level::player->_entity->position.x, Level::player->_entity->position.y, Level::player->_entity->position.z);

       ImGui::End();
    });

    while (!window->shouldShutdown()) {
        Game::engine.updateScene();

        Level::player->preUpdate();

        Game::physicsEngine->update();

        Level::floor->render(Game::engine);
        Level::cube->render(Game::engine);
        Level::floor->updatePhysics(*Game::physicsEngine);
        Level::cube->updatePhysics(*Game::physicsEngine);
        Level::player->postUpdate();


        Game::engine.Run();

        window->update();
        auto now = std::chrono::steady_clock::now();
        Game::deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() / 1000000.0f;
        last = now;

        spdlog::info("HIT: {}", Level::cube->hasHit(Level::player->_entity->position));
    }

}

void App::cleanup() {
    delete Level::player;
    delete Level::floor;
    delete Level::cube;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;


    Game::engine.CleanUp();
    delete window;
}
