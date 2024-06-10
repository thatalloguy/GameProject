//
// Created by allos on 6/10/2024.
//

#ifndef GAME_FISHINGMANAGER_H
#define GAME_FISHINGMANAGER_H

#include <Objects/Entity.h>

#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/Shape/CylinderShape.h"


#include <Physics/PhysicsEngine.h>
#include <Input/InputManager.h>

enum class PlayerState : unsigned int {
    Moving = 0,
    Fishing = 1
};

class Player  {

public:
    Player(Camera& camera, Quack::PhysicsEngine& physicsEngine) : _camera(camera), physicsEngine(physicsEngine) {
        // initialize physics character

        _standingShape = RotatedTranslatedShapeSettings({0, 8, 0}, Quat::sIdentity(), new CylinderShape(1, 1)).Create().Get();
        _crouchingShape = RotatedTranslatedShapeSettings({0, 8, 0}, Quat::sIdentity(), new BoxShape({1, 1, 1})).Create().Get();

        Ref<CharacterSettings> settings = new CharacterSettings();
        settings->mMaxSlopeAngle = DegreesToRadians(90.0f);
        settings->mLayer = Quack::Layers::MOVING;
        settings->mShape = _standingShape;
        settings->mFriction = 0.1f;
        settings->mMass = 100.0f;
        settings->mGravityFactor = 2.0f;
        settings->mSupportingVolume = Plane(Vec3::sAxisY(), -0.3f);

        _character = new Character(settings, RVec3::sZero(), Quat::sIdentity(), 0, &physicsEngine.getSystem());
        _character->AddToPhysicsSystem(EActivation::Activate);
    }
    ~Player() {
        _character->RemoveFromPhysicsSystem();
    }

    void update() {

        if (state == PlayerState::Moving) {
            updateCamera();
            auto vec = getMovement();


            _character->SetLinearVelocity({vec.x, vec.y, vec.z});

            position = _character->GetPosition();
        }


        _character->PostSimulation(0.05f);
    }


    float speed = 10.0f;
    const float sensitivity = 0.02f;
    float playerHeight = 3.0f;
    float jumpForce = 3.0f;

    Quack::Math::Vector3 position{0, 0, 0};
    PlayerState state = PlayerState::Moving;


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

            if (_character->GetGroundNormal().GetY() > 0 && Quack::Input::isButtonPressed(0, 0)) {
                out.y += jumpForce;
            }
            if (Quack::Input::isButtonPressed(0, 2)) {
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
/*            if (Quack::Input::isKeyPressed(Quack::Key::A)) {
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
            }*/
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
    Quack::PhysicsEngine& physicsEngine;

};


class FishingManager{

public:
    FishingManager(VulkanEngine& renderer, Player& player, Quack::PhysicsEngine& physicsEngine);
    ~FishingManager();

    void Update();

private:
    friend class Player;

    VulkanEngine& _renderer;
    Player& _player;
    Quack::PhysicsEngine& _physicsEngine;


    Quack::Entity* fishCollider;
    Quack::Entity* lake;

};


#endif //GAME_FISHINGMANAGER_H