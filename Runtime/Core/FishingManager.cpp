//
// Created by allos on 6/10/2024.
//

#include "FishingManager.h"
#include "imgui.h"

FishingManager::FishingManager(VulkanEngine &renderer, Player &player, Quack::PhysicsEngine& physicsEngine) : _renderer(renderer), _player(player), _physicsEngine(physicsEngine) {


    Quack::EntityCreationInfo cube_info {
            .position = {-10, -1.5f, 20},
            .size = {4, 1.0f, 4},
            .model = 2,
            .isPhysical = false,

    };

    Quack::EntityCreationInfo lake_info {
            .position = {-10, -1.0f, 40},
            .size = {10, 1.0f, 10},
            .model = 2,
            .isPhysical = true,
            .bodyCreationInfo = {
                    .position = {-10, -1, 40},
                    .rotation = Quat::sIdentity(),
                    .shape = new BoxShape(RVec3(10.f, 10.0f, 10.f)),
                    .shouldActivate = EActivation::DontActivate,
                    .motionType = EMotionType::Static,
                    .layer = Quack::Layers::NON_MOVING,
                    .physicsEngine = &physicsEngine
            }
    };


    Quack::EntityCreationInfo debugPoint_info {
            .position = {-10, 4, 40},
            .size = {0.2f, 0.2f, 0.2f},
            .model = 3,
            .isPhysical = false,
    };

    Quack::EntityCreationInfo debugE_info {
            .position = {-10, 4, 40},
            .size = {0.3f, 0.1f, 0.3f},
            .model = 3,
            .isPhysical = false,
    };

    Quack::EntityCreationInfo bobber_info {
            .position = {-10, 4, 40},
            .size = {0.3f, 0.3f, 0.3f},
            .model = 4,
            .isPhysical = false,
    };

    fishCollider = new Quack::Entity(cube_info);
    lake = new Quack::Entity(lake_info);
    debugPoint = new Quack::Entity(debugPoint_info);

    bobber = new Quack::Entity(bobber_info);
    debugE = new Quack::Entity(debugE_info);


    // UI UI UI UI UI AHGHHHHDAHIKWDHwaiudyAWDIAWUDAYUIduaiwdyawuid. i hate UI programming >:(
    renderer.imguiFunc.pushFunction([&](){

        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Fish Manager");
        if (ImGui::TreeNode("FishingManager: ")) {
            ImGui::Text("Player Speed %f", _player.speed);

            ImGui::Text("Current Item: %s", _player.currentItem->name);

            ImGui::TreePop();
        }

        auto drawList = ImGui::GetForegroundDrawList();

        if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving) {
            ImGui::SetWindowFontScale(2.5f);
            drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press RIGHT to fish");
            ImGui::SetWindowFontScale(1.0f);
        }
        else if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing) {
            ImGui::SetWindowFontScale(2.5f);
            drawList->AddText(ImVec2(500, 650),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press UP to cancel");
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

    fishCollider->render(_renderer);
    lake->render(_renderer);
    debugPoint->render(_renderer);
    debugE->render(_renderer);
    bobber->render(_renderer);

    lake->updatePhysics(_physicsEngine);

    if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving && Quack::Input::isButtonPressed(0, 1)) {
        _player.state = PlayerState::Fishing;

        //setup animation
        vec3Tween = tweeny::from(_player.position.x, _player.position.y + _player.playerHeight, _player.position.z, _player._camera.pitch, _player._camera.yaw).to(-10, 10, 30, 0.9f, 3.15f).during(1000).via(tweeny::easing::sinusoidalOut);
        vec3Tween.onStep([=, this](float x, float y, float z, float pitch, float yaw) {
            this->_player._camera.position.x = x;
            this->_player._camera.position.y = y;
            this->_player._camera.position.z = z;
            this->_player._camera.pitch = pitch;
            this->_player._camera.yaw = yaw;
            return false;
        });
        pause = true;
    }
    if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing && Quack::Input::isButtonPressed(0, 3)) {
        _player.state = PlayerState::Moving;
        _player._camera.pitch = 0.f;
        // cancel the rest
        updateFishing = false;
    }

    if (pause) {
        vec3Tween.step(dt);
        if (vec3Tween.progress() == 1.0f) {

            spdlog::info("setting up fishing");
            setUpFishing();

            pause = false;
        }
    }
    if (updateFishing) {
        dummy.update(dt);
        debugPoint->position = dummy.position;
        bobber->position.x = cursor.x;
        bobber->position.z = cursor.y;

        //TODO add keyboard support

        // Bobber movement
        if (Quack::Input::isControllerPresent(0)) {
            auto vec = Quack::Input::getJoystickAxis(0);

            vec.x = ceil(vec.x * 11) / 11;
            vec.y = ceil(vec.y * 11) / 11;



            if (vec.x > 0.1 || vec.x < -0.1 || vec.y > 0.1 || vec.y < -0.1) {
                vec.x *= (dt * 1.45f);
                vec.y *= (dt * 1.45f);

                cursor.x -= vec.x;
                cursor.y -= vec.y;
                cursor.x = max(lake->position.x - 2.0f, min(lake->position.x + 2.0f, cursor.x));
                cursor.y = max(lake->position.z - 2.0f, min(lake->position.z + 2.0f, cursor.y));
            }

        }
    }



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
