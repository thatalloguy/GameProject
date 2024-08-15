//
// Created by allos on 6/10/2024.
//

#ifndef GAME_FISHINGMANAGER_H
#define GAME_FISHINGMANAGER_H

#include <Objects/Entity.h>
#include <Physics/PhysicsEngine.h>
#include <Input/InputManager.h>

#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/Shape/CylinderShape.h"

#include <tweeny.h>

enum class PlayerState : unsigned int {
    Moving = 0,
    Fishing = 1,
    Idle = 2,
    Cutscene = 3,
    Shopping = 4
};

struct BackPack {
    unsigned int fish = 0;
    int money = 0; // broke lmao
    unsigned int fishingRodLevel = 1;
};

class Player  {

public:

    Player(Camera& camera, Quack::PhysicsEngine& physicsEngine) : _camera(camera) {
        // initialize physics character

        // First create the physics shapes
        _standingShape = RotatedTranslatedShapeSettings({0, 8, 0}, Quat::sIdentity(), new CylinderShape(1, 1)).Create().Get();
        _crouchingShape = RotatedTranslatedShapeSettings({0, 8, 0}, Quat::sIdentity(), new BoxShape({1, 1, 1})).Create().Get();

        // Create the character by hand since the Entity class doenst support it.
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

        lastMousePos = Quack::Input::getMousePos();

        _character->SetPosition(RVec3(position.x, position.y, position.z));
    }

    ~Player() {
        _character->RemoveFromPhysicsSystem();
    }

    void update(float deltaTime) {

        // Only update the movement while in the moving state
        if (state == PlayerState::Moving) {

            //temp
            if (Quack::Input::isKeyPressed(Quack::Key::C)) {
                updateCamera(deltaTime);
                auto vec = getMovement();

                // gross.
                position.x += vec.x * 0.1f;
                position.y += vec.y * 0.1f;
                position.z += vec.z * 0.1f;
            } else {
                //getMovement relies on 'updateCamera' so it must be called first.
                updateCamera(deltaTime);
                auto vec = getMovement();

                // Set the linear velocity and update the position of the player.
                _character->SetLinearVelocity({vec.x, vec.y, vec.z});

                position = _character->GetPosition();
            }



        } else {
            // stop movement
            _character->SetLinearVelocity({0, 0, 0});

        }


        _character->PostSimulation(0.05f);
    }


    float speed = 10.0f;
    const float sensitivity = 0.02f;
    const float mouseSensitivity = 0.1f;
    float playerHeight = 6.0f;
    float jumpForce = 3.0f;

    Quack::Math::Vector3 position{3, 1, 33};
    PlayerState state = PlayerState::Moving;

    Ref<Character> _character;
    BackPack inventory;
    Camera& _camera;

    int sleepingCounter = 0;
private:
    void updateCamera(float deltaTime) {

        auto character_position = _character->GetPosition();

        _camera.position.x = position.x;
        _camera.position.y = position.y + playerHeight;
        _camera.position.z = position.z;

        auto rel = Quack::Input::getMousePos() - lastMousePos;

        rel.x *= deltaTime * mouseSensitivity;
        rel.y *= deltaTime * mouseSensitivity;

        _camera.yaw += rel.x;

        //hacky fix
        if (_camera.yaw >= 4000 || _camera.yaw <= -4000) {
            _camera.yaw = 0.0f;
        }


        // prevent a bug from happening when the player moves their mouse too quickly.
        _camera.yaw = max(-5000.0f, min(5000.0f, _camera.yaw));

        _camera.pitch += rel.y;
        _camera.pitch = max(-0.7f, min(0.7f, _camera.pitch));

        lastMousePos = Quack::Input::getMousePos();

    };


    Quack::Math::Vector3 getMovement() {
        auto currentVel = _character->GetLinearVelocity();
        Quack::Math::Vector3 out = {0, currentVel.GetY(), 0};

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
                out = Quack::Math::Vector3{vec.x, _character->GetLinearVelocity().GetY(), vec.y};
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

/*            if (_character->GetGroundNormal().GetY() > 0 && Quack::Input::isButtonPressed(0, 0)) {
                out.y += jumpForce;
            }*/
            if (Quack::Input::isButtonPressed(0, 2)) {
                speed = 17.5f;
                _camera.fov = 94.0f;
            } else {
                speed = 10.0f;
                _camera.fov = 90.0f;
            }


        }

        if (Quack::Input::isKeyPressed(Quack::Key::W)) {
            out.z = -speed;
            out.z *= 0.9f;
        }
        else if (Quack::Input::isKeyPressed(Quack::Key::S)) {
            out.z = speed;
            out.z *= 0.9f;
        }

        if (Quack::Input::isKeyPressed(Quack::Key::A)) {
            out.x = -speed;
            out.x *= 0.9f;
        }
        else if (Quack::Input::isKeyPressed(Quack::Key::D)) {
            out.x = speed;
            out.x *= 0.9f;
        }


        if (Quack::Input::isKeyPressed(Quack::Key::LEFT_SHIFT)) {
            speed = 17.5f;
            _camera.fov = 94.0f;
        }
        else {
            speed = 10.0f;
            _camera.fov = 90.0f;
        }

        /* Jumping is not nessicairy and causes too many bugs
        if (_character->GetGroundNormal().GetY() > 0 && Quack::Input::isKeyPressed(Quack::Key::SPACE)) {
            out.y += jumpForce;
        }
*/


        // Rotate the move direction based on where the camera is looking.
        auto dir = glm::vec3(_camera.getRotationMatrix() * glm::vec4(out.x, out.y, out.z, 0));
        out.x = dir.x;
        out.y = out.y; // ignore y.
        out.z = dir.z;

        return out;
    }

    RefConst<Shape> _standingShape;
    RefConst<Shape> _crouchingShape;

    Quack::Math::Vector2 lastMousePos{0, 0};

    friend class FishingManager;

};

struct Corner {
    unsigned int ID = 0;
    float weight = 1.0f;
};

enum class FishState : unsigned  int {
    wandering = 0,
    bait = 1,
    escaping = 2
};



/*
 * HOW THE AI WORKS.
 * First the Fish calc each rect of each corner. (see calcRects)
 * Then the fish picks a corner with a few rules:
 * - If the fish hasn't picked any corners in its lifetime (meaning currentCorner == -1) it will pick a random corner.
 * - Each corner has a weight. The fish will favor the corner that weighs the most
 * - Every time the fish needs to pick a  new corner a few things happen to the corners:
 * - - The current corner gets -0.2f weight
 * - - It randomly chooses between either:
 *   - - The last visited corner gets + 0.1f weight
 *   - - The diagonal corner of the current corner gets + 0.1 weight
 *
 * - For picking the best corner, the corner must:
 * - Have a weight greater than 0.9
 * - not be the last or current corner
 *
 * - If for some reason the fish cant pick the best corner (if all the weights are to low)
 *   the fish will reset all the corner weights and pick a random one.
 *
 * - For deciding which position the fish wants to go, it will pick a random point in the desired corner.
 *
 * - How Curiosity works:
 *  -- Curiosity increase over time generally.
 *  -- Curiosity increases when the bobber makes small movements
 *  -- Curiosity increases automatically based on the bait used.
 *
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

    void genNextPos(Player& player) {

        curiosity += (player.inventory.fishingRodLevel / 10.0f) + (std::rand() % 3 + 1.0f) / 10.0f;

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
        moveSpeed = 0.0f + ((std::rand() % 3 + 1.5f) / 10.0f);
    }

    void update(float deltaTime, Quack::Math::Vector3& bobberPosition, Player& player) {
        switch (_state) {
            case FishState::wandering:
                // before updating anymovement stuff check if the fish is curious enough
                if (curiosity >= 1.0f) {
                    _state = FishState::bait;

                    desiredPos = bobberPosition;

                    //set new desired pos.
                    travelPath = tweeny::from(position.x, position.y, position.z).to(desiredPos.x, desiredPos.y, desiredPos.z).during(10000).via(tweeny::easing::sinusoidalInOut);
                    travelPath.onStep([=, this](float x, float y, float z) {
                        this->position.x = x;
                        this->position.y = y;
                        this->position.z = z;
                        return false;
                    });

                    // change speed randomly.
                    moveSpeed = 0.35f;
                }


                travelPath.step(deltaTime * moveSpeed);
                if (travelPath.progress() == 1.0f && std::rand() % 5) {
                    genNextPos(player);
                }
                break;

            case FishState::bait:
                travelPath.step(deltaTime * moveSpeed);

                /// the fish reached the bobber
                if (travelPath.progress() == 1.0f) {
                    _state = FishState::escaping;
                    pickDirection(bobberPosition);
                }
                break;

            case FishState::escaping:
                travelPath.step(deltaTime * moveSpeed);

                moveSpeed  = 0.75f;

                //Randomly Change direction if the fish reaches the outer edge.
                if (travelPath.progress() == 1.0f && rand() % 75 == 1) {
                    pickDirection(bobberPosition);
                }

                break;
        }
    }

    const char* getStateCSTR() {
        switch (_state) {
            case FishState::wandering:
                return "wandering";
            case FishState::bait:
                return "Bait";
            case FishState::escaping:
                return "Escaping";
        }
    }

    void reset() {
        _state = FishState::wandering;
        currentCorner  = -1;
        lastCorner = 3;
        diagonalCorner = 0;
        curiosity = 0.0f;

        stamina = maxStamina;
        moveLeft = false;
    }

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
            currentCorner = std::rand() % 4;
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
            if (std::rand() % 2) {
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
                currentCorner = std::rand() % 4;
            }
            else {
                // pick the best corner.
                currentCorner = (int) best.ID;
            }
        }

        lastCorner = currentCorner;
    }

    void pickPositionInCorner() {
        Quack::Math::Vector4 rect = rects[currentCorner];


        //pick a random point in both the width and height
        int randX = rand() % static_cast<int>(rect.z - rect.x);
        int randY = rand() % static_cast<int>(rect.w - rect.y);

        desiredPos.x = rect.x + (float) randX;
        desiredPos.z = rect.y + (float) randY; // z instead of y since the lake is viewed as a 2d square
    };

    void pickDirection(Quack::Math::Vector3& bobberPos) {
        moveLeft = !moveLeft; // only go left or right cuz me am lazy.

        if (moveLeft) {
            desiredPos.x = bobberPos.x - 5.0f;
        } else {
            desiredPos.x = bobberPos.x + 5.0f;
        }

        travelPath = tweeny::from(position.x, position.y, position.z).to(desiredPos.x, desiredPos.y, desiredPos.z).during(10000).via(tweeny::easing::sinusoidalInOut);
        travelPath.onStep([=, this](float x, float y, float z) {
            this->position.x = x;
            this->position.y = y;
            this->position.z = z;
            return false;
        });

    };

    int currentCorner = -1;
    unsigned int lastCorner = 3;
    unsigned int diagonalCorner = 0;

    float curiosity = 0.0f;

    Corner corners[4] = {{0}, {1}, {2}, {3}};
    Quack::Math::Vector4 rects[4];

    tweeny::tween<float, float, float> travelPath;
    FishState _state = FishState::wandering;
    float moveSpeed = 0.3f;
    float stamina = 30.0f;
    const float maxStamina = 30.0f;

    bool moveLeft = false;
};


class FishingManager{

public:
    FishingManager(VulkanEngine& renderer, Player& player, Quack::PhysicsEngine& physicsEngine);
    ~FishingManager();

    void Update(float dt);

    void startTutorial() { isInTutorial = true; };
    void completeTutorial() { isInTutorial = false; };
    Fish dummy;

    float fishlineDurability = 10.0f;
    float maxDurability = 10.0f;

private:
    friend class Player;

    VulkanEngine& _renderer;
    Player& _player;
    Quack::PhysicsEngine& _physicsEngine;

    //Entities
    Quack::Entity* fishCollider;
    Quack::Entity* lake;
    Quack::Entity* debugPoint;
    Quack::Entity* bobber;
    Quack::Entity* debugE;


    tweeny::tween<float, float, float, float, float> vec3Tween;
    bool pause = false;
    bool updateFishing = false;
    bool isInTutorial = false;


    Quack::Math::Vector2 cursor{0, 0};

    void setUpFishing();
    void cleanUpFishing();

    void checkUserCanFish(float deltaTime);
    void updateBobberMovement(float deltaTime);
    void cameraCutsceneTick(float dt);


    //bobber movement
    bool bobberMovedLastFrame = false;
};


#endif //GAME_FISHINGMANAGER_H
