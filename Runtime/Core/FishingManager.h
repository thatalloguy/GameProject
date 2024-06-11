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

#include <tweeny.h>

enum class PlayerState : unsigned int {
    Moving = 0,
    Fishing = 1
};

class Player  {
    friend class FishingManager;

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
        } else {
            // stop movement
            _character->SetLinearVelocity({0, 0, 0});

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

struct Corner {
    unsigned int ID = 0;
    float weight = 1.0f;
};

enum class FishState : unsigned  int {
    wandering = 0,
    bait = 1
};

/*
 * HOW THE AI WORKS.
 * First the Fish calc each rect of each corner. (see calcRects)
 * Then the fish picks a corner with a few rules:
 * - If the fish hasnt picked any corners in its lifetime (meaning currentCorner == -1) it will pick a random corner.
 * - Each corner has a weight. The fish will favor the corner that weighs the most
 * - Every time the fish needs to pick a  new corner a few things happen to the corners:
 * - - The current corner gets -0.2f weight
 * - - It randomly chooses between either:
 *   - - The last visited corner gets + 0.1f weight
 *   - - The diagonal corner of the current corner gets + 0.1 weight
 *
 * - For picking the best corner, the corner must:
 * - Have a weight greater then 0.9
 * - not be the last or current corner
 *
 * - If for some reason the fish cant pick the best corner (if all the weights are to low)
 *   the fish will reset all the corner weights and pick a random one.
 *
 * - For deciding which position the fish wants to go, it will pick a random point in the desired corner.
 */
struct Fish {
    friend class FishingManager;

    Quack::Math::Vector3 position{0, 0, 0};
    Quack::Math::Vector3 desiredPos{0, 0, 0};

    void initFish(Quack::Math::Vector2 rectMin, Quack::Math::Vector2 rectMax, Quack::Math::Vector3 newPos) {

        //put it here since it only really needs to be calc per lake, not every ai tick.
        //first calculate the rects of each corner
        calculateRects(rectMin, rectMax);

        this->position = newPos;
    }

    void genNextPos() {


        //then pick a corner
        pickCorner();

        //Now pick a position in a corner
        pickPositionInCorner();

        travelPath = tweeny::from(position.x, position.y, position.z).to(desiredPos.x, desiredPos.y, desiredPos.z).during(10000).via(tweeny::easing::sinusoidalInOut);
        travelPath.onStep([=, this](float x, float y, float z) {
            this->position.x = x;
            this->position.y = y;
            this->position.z = z;
            return false;
        });

        // change speed randomly.
        float neww = (std::rand() % 3) + 1.5f;
        moveSpeed = 0.0f + (neww / 10.0f);
    }

    void update(float deltaTime) {
        if (_state == FishState::wandering) {
             travelPath.step(deltaTime * moveSpeed);
             if (travelPath.progress() == 1.0f && rand() % 5) {
                genNextPos();
            }
        }
    }

private:
    void calculateRects(Quack::Math::Vector2 rectMin, Quack::Math::Vector2 rectMax) {
        // calc the halfs of the rects.
        float halfWidth  = rectMin.x + ((rectMax.x - rectMin.x)  / 2); // for example : -2 - -18 = 16. 16 / 2 = 8. -18 + 8 = -10. so halfway = 10
        float halfHeight = rectMin.y + ((rectMax.y - rectMin.y) / 2);

        // 0 = x
        // 1 = y
        // 2 = z
        // 3 = w

        /* X-------Y
         * |       |
         * |       |
         * Z-------W
         */
        //calc the rest
        rects[0] = {rectMin.x, rectMin.y, halfWidth, halfHeight}; // x
        rects[1] = {halfWidth, rectMin.y, rectMax.x, halfHeight}; // y
        rects[2] = {rectMin.x, halfHeight, halfWidth, rectMax.y}; // z
        rects[3] = {halfWidth, halfHeight, rectMax.x, rectMax.y}; // w
    }

    void pickCorner() {
        //if its the first time just pick a random corner
        if (currentCorner <= -1) {
            currentCorner = (rand() % 4);
        }
        else {

            // calc next corner based on the weight
            switch (corners[currentCorner].ID) {
                case 0:
                    diagonalCorner = 3;
                    break;
                case 1:
                    diagonalCorner = 2;
                    break;
                case 2:
                    diagonalCorner = 1;
                    break;
                case 3:
                    diagonalCorner = 0;
                    break;
                default:
                    break;
            }

            corners[currentCorner].weight -= 0.2f;
            if (rand() % 2) {
                corners[lastCorner].weight += 0.1f;
            } else {
                corners[diagonalCorner].weight += 0.1f;
            }



            // get the best corner
            Corner best{7, 0.9f};
            for (auto corner : corners) {
                if (corner.weight > best.weight && corner.ID != currentCorner && lastCorner != corner.ID) {
                    best = corner;
                }
            }

            //if for some reason the ID is still 7 reset all the weights and pick a random one
            if (best.ID == 7) {
                currentCorner = -1;
                corners[0].weight = 1.0f;
                corners[1].weight = 1.0f;
                corners[2].weight = 1.0f;
                corners[3].weight = 1.0f;
                currentCorner = (rand() % 4);
            }
            else {
                // pick the best corner.
                currentCorner = best.ID;
            }
        }

        lastCorner = currentCorner;
    }

    void pickPositionInCorner() {
        Quack::Math::Vector4 rect = rects[currentCorner];


        //pick a random point in both the width and height
        int randX = rand() % static_cast<int>(rect.z - rect.x);
        int randY = rand() % static_cast<int>(rect.w - rect.y);

        desiredPos.x = rect.x + randX;
        desiredPos.z = rect.y + randY; // z instead of y since the lake should be viewed as a 2d square
    };

    int currentCorner = -1;
    unsigned int lastCorner = 3;
    unsigned int diagonalCorner = 0;

    Corner corners[4] = {{0}, {1}, {2}, {3}};
    Quack::Math::Vector4 rects[4];

    tweeny::tween<float, float, float> travelPath;
    FishState _state = FishState::wandering;
    float moveSpeed = 0.3f;
};


class FishingManager{

public:
    FishingManager(VulkanEngine& renderer, Player& player, Quack::PhysicsEngine& physicsEngine);
    ~FishingManager();

    void Update(float dt);

private:
    friend class Player;

    VulkanEngine& _renderer;
    Player& _player;
    Quack::PhysicsEngine& _physicsEngine;


    Quack::Entity* fishCollider;
    Quack::Entity* lake;
    Quack::Entity* debugPoint;
    Quack::Entity* fish;
    tweeny::tween<float, float, float, float, float> vec3Tween;
    bool pause = false;
    bool updateFishing = false;
    Fish dummy;

    void setUpFishing();

};


#endif //GAME_FISHINGMANAGER_H
