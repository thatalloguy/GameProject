//
// Created by allos on 6/10/2024.
//

#include "FishingManager.h"
#include "imgui.h"

FishingManager::FishingManager(VulkanEngine &renderer, Player &player, Quack::PhysicsEngine& physicsEngine) : _renderer(renderer), _player(player), _physicsEngine(physicsEngine) {


    Quack::EntityCreationInfo cube_info {
            .position = {6, 1.0f, -4},
            .size = {4, 3.0f, 4},
            .model = 20,
            .isPhysical = false,

    };

    Quack::EntityCreationInfo lake_info {
            .position = {5, -2.5f, -18},
            .size = {10, 1.0f, 10},
            .model = 20,
            .isPhysical = true,
            .bodyCreationInfo = {
                    .position = {5, -2.5f, -18},
                    .rotation = Quat::sIdentity(),
                    .shape = new BoxShape(RVec3(10.f, 10.0f, 10.f)),
                    .shouldActivate = EActivation::DontActivate,
                    .motionType = EMotionType::Static,
                    .layer = Quack::Layers::NON_MOVING,
                    .physicsEngine = &physicsEngine
            }
    };


    Quack::EntityCreationInfo debugPoint_info {
            .position = {0, 4, 15},
            .size = {0.2f, 0.2f, 0.2f},
            .model = 30,
            .isPhysical = false,
    };

    Quack::EntityCreationInfo debugE_info {
            .position = {0, 4, 15},
            .size = {0.3f, 0.1f, 0.3f},
            .model = 30,
            .isPhysical = false,
    };

    Quack::EntityCreationInfo bobber_info {
            .position = {0, 4, 15},
            .size = {0.3f, 0.3f, 0.3f},
            .model = 40,
            .isPhysical = false,
    };

    fishCollider = new Quack::Entity(cube_info);
    lake = new Quack::Entity(lake_info);
    debugPoint = new Quack::Entity(debugPoint_info);

    bobber = new Quack::Entity(bobber_info);
    debugE = new Quack::Entity(debugE_info);


    // UI UI UI UI UI AHGHHHHDAHIKWDHwaiudyAWDIAWUDAYUIduaiwdyawuid. i hate UI programming >:(
    renderer.debugRenderFuncs.pushFunction([&](){

        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Fish Manager");
        ImGui::SetNextItemOpen(true);
        if (ImGui::TreeNode("FishingManager: ")) {
            ImGui::Text("Player Speed %f", _player.speed);
            ImGui::Text("Line Durability: %f", fishlineDurability);

            ImGui::TreePop();
        }

        auto drawList = ImGui::GetForegroundDrawList();

        if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving) {
            ImGui::SetWindowFontScale(2.5f);
            if (Quack::Input::isControllerPresent(0)) {
                drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press RIGHT to fish");
            } else {
                drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press E to fish");
            }
            ImGui::SetWindowFontScale(1.0f);
        }
        else if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing) {
            ImGui::SetWindowFontScale(2.5f);
            if (Quack::Input::isControllerPresent(0)) {
                drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press Up to Cancel");
            } else {
            drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press Q to Cancel");
            }
            ImGui::SetWindowFontScale(1.0f);
        }

        ImGui::SeparatorText("Fish AI");
        if (ImGui::Button("Pick new")) {
            dummy.genNextPos();
        }
        ImGui::Text("Current Corner: %i", dummy.corners[dummy.currentCorner].ID);
        ImGui::Text("CORNER 1: %f", dummy.corners[0].weight);
        ImGui::Text("CORNER 2: %f", dummy.corners[1].weight);
        ImGui::Text("CORNER 3: %f", dummy.corners[2].weight);
        ImGui::Text("CORNER 4: %f", dummy.corners[3].weight);
        ImGui::Separator();
        ImGui::Text("Desired Position: %f %f", dummy.desiredPos.x, dummy.desiredPos.z);
        ImGui::Text("Movement Speed : %f", dummy.moveSpeed);
        ImGui::Text("Fish State: %s", dummy.getStateCSTR());
        ImGui::Text("Fish curiosity: %f", dummy.curiosity);
        ImGui::Text("Fish Stamina: %f", dummy.stamina);

        if (ImGui::Button("Reset Fish")) {
            dummy.reset();
        }

        ImGui::End();
    });
}

FishingManager::~FishingManager() {
    delete fishCollider;
    delete lake;
    delete debugPoint;
    delete debugE;
    delete bobber;
}

void FishingManager::Update(float dt) {

    //fishCollider->render(_renderer);
    lake->render(_renderer);
    debugPoint->render(_renderer);
    debugE->render(_renderer);
    bobber->render(_renderer);

    lake->updatePhysics(_physicsEngine);

    checkUserCanFish(dt);

    updateBobberMovement(dt);

    cameraCutsceneTick(dt);


}

void FishingManager::setUpFishing() {

    //center debug point
    debugPoint->position.x = lake->position.x + (lake->size.x * 0.8f);
    debugPoint->position.z = lake->position.z + (lake->size.z * 0.8f);

    debugPoint->position.y  = lake->position.y + lake->size.y + 1;

    bobber->position.y = lake->position.y + lake->size.y + 0.2f;

    cursor.x = lake->position.x;
    cursor.y = lake->position.z;

    dummy.initFish({lake->position.x - lake->size.x * 0.8f, lake->position.z - lake->size.z * 0.8f}, {lake->position.x + (lake->size.x * 0.8f), lake->position.z + lake->size.z * 0.7f}, debugPoint->position);
    dummy.genNextPos();


    updateFishing = true;
}

void FishingManager::cleanUpFishing() {
    _player.state = PlayerState::Moving;
    _player._camera.pitch = 0.f;
    // cancel the rest
    updateFishing = false;
    cursor.x = lake->position.x;
    cursor.y = lake->position.z;
    dummy.reset();

    fishlineDurability = maxDurability;
}

void FishingManager::checkUserCanFish(float deltaTime) {

    if (Quack::Input::isControllerPresent(0)) {
        if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving && Quack::Input::isButtonPressed(0, 1)) {
            _player.state = PlayerState::Fishing;

            //setup animation
            vec3Tween = tweeny::from(_player.position.x, _player.position.y + _player.playerHeight, _player.position.z, _player._camera.pitch, _player._camera.yaw).to(lake->position.x, lake->position.y + 15, lake->position.z + 8, 1.1f, 3.45f).during(1000).via(tweeny::easing::sinusoidalOut);
            vec3Tween.onStep([=, this](float x, float y, float z, float pitch, float yaw) {
                this->_player._camera.position.x = x;
                this->_player._camera.position.y = y;
                this->_player._camera.position.z = z;
                this->_player._camera.pitch = pitch;
                //this->_player._camera.yaw = yaw;
                return false;
            });
            pause = true;
        }
        if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing && Quack::Input::isButtonPressed(0, 3)) {
            cleanUpFishing();
        }

    }

    if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving && Quack::Input::isKeyPressed(Quack::Key::E)) {
        _player.state = PlayerState::Fishing;

        //setup animation
        vec3Tween = tweeny::from(_player.position.x, _player.position.y + _player.playerHeight, _player.position.z, _player._camera.pitch, _player._camera.yaw).to(lake->position.x, lake->position.y + 15, lake->position.z + 8, 1.1f, 3.45f).during(1000).via(tweeny::easing::sinusoidalOut);
        vec3Tween.onStep([=, this](float x, float y, float z, float pitch, float yaw) {
            this->_player._camera.position.x = x;
            this->_player._camera.position.y = y;
            this->_player._camera.position.z = z;
            this->_player._camera.pitch = pitch;
            //this->_player._camera.yaw = yaw;
            return false;
        });
        pause = true;
    }
    if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing && Quack::Input::isKeyPressed(Quack::Key::Q)) {
        cleanUpFishing();
    }


}

void FishingManager::updateBobberMovement(float deltaTime) {


    if (updateFishing && !isInTutorial) {
        dummy.update(deltaTime, bobber->position, _player);
        debugPoint->position = dummy.position;
        bobber->position.x = cursor.x;
        bobber->position.z = cursor.y;

        //TODO add keyboard support

        // Bobber movement
        if (Quack::Input::isControllerPresent(0)) {

            if (dummy._state == FishState::escaping) {
                auto vec = Quack::Input::getJoystickAxis(0);


                // We movin correctly
                if (vec.x > 0.1 && !dummy.moveLeft) {
                    dummy.stamina -= vec.x * deltaTime * 1.45f;
                } else if (vec.x < 0.1 && dummy.moveLeft) {
                    dummy.stamina -= -vec.x * deltaTime * 1.45f;
                } else {// We arent movin correctly
                    fishlineDurability -= deltaTime * 1.05f;
                }



            }

            if (Quack::Input::isButtonPressed(0, 0)) {
                cursor.y += 2 * (deltaTime * 2.5f);
                if (bobber->position.z < lake->position.z - (lake->size.z * 0.75f)) {
                    cleanUpFishing();
                }

                if (!bobberMovedLastFrame) {
                    dummy.curiosity += 0.1f;
                    bobberMovedLastFrame = true;
                }
            } else {
                // reset if the button is not pressed anymore.
                bobberMovedLastFrame = false;
            }
        }
        else {


            // Keyboard version of bobber / fishing
            if (dummy._state == FishState::escaping) {



                // We movin correctly
                if (Quack::Input::isKeyPressed(Quack::Key::D) && !dummy.moveLeft) {
                    dummy.stamina -= 1.0f * deltaTime * 1.45f;
                } else if (Quack::Input::isKeyPressed(Quack::Key::A) && dummy.moveLeft) {
                    dummy.stamina -= 1.0f * deltaTime * 1.45f;
                } else {// We arent movin correctly
                    fishlineDurability -= deltaTime * 1.05f;
                }



            }

            if (Quack::Input::isKeyPressed(Quack::Key::E)) {
                cursor.y += 2 * (deltaTime * 2.5f);
                if (bobber->position.z < lake->position.z - (lake->size.z * 0.75f)) {
                    cleanUpFishing();
                }

                if (!bobberMovedLastFrame) {
                    dummy.curiosity += 0.1f;
                    bobberMovedLastFrame = true;
                }
            } else {
                // reset if the button is not pressed anymore.
                bobberMovedLastFrame = false;
            }
        }



        if (fishlineDurability <= 0.1f || dummy.stamina <= 0.1f) {
            cleanUpFishing();
        }
    }


}

void FishingManager::cameraCutsceneTick(float dt) {
    if (pause) {
        vec3Tween.step(dt);
        if (vec3Tween.progress() == 1.0f) {

            spdlog::info("setting up fishing");
            setUpFishing();

            pause = false;
        }
    }
}
