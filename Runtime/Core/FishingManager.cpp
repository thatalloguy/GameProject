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


    fishCollider = new Quack::Entity(cube_info);
    lake = new Quack::Entity(lake_info);


    // UI UI UI UI UI AHGHHHHDAHIKWDHwaiudyAWDIAWUDAYUIduaiwdyawuid. i hate UI programming >:(
    renderer.imguiFunc.pushFunction([&](){

        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Fish Manager");
        if (ImGui::TreeNode("FishingManager: ")) {
            ImGui::Text("Player Speed %f", _player.speed);


            ImGui::TreePop();
        }

        auto drawList = ImGui::GetForegroundDrawList();

        if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving) {
            ImGui::SetWindowFontScale(2.5f);
            drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press RIGHT to fish");
            ImGui::SetWindowFontScale(1.0f);
        } else if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing) {
            ImGui::SetWindowFontScale(2.5f);
            drawList->AddText(ImVec2(500, 500),  ImColor(50.0f,45.0f,255.0f,255.0f), "Press RIGHT to cancel");
            ImGui::SetWindowFontScale(1.0f);
        }



        ImGui::End();
    });
}

FishingManager::~FishingManager() {
    delete fishCollider;
    delete lake;
}

void FishingManager::Update() {
    fishCollider->render(_renderer);
    lake->render(_renderer);
    lake->updatePhysics(_physicsEngine);

    if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Moving && Quack::Input::isButtonPressed(0, 1)) {
        _player.state = PlayerState::Fishing;
    }

    if (fishCollider->hasHit(_player.position) && _player.state == PlayerState::Fishing && Quack::Input::isButtonPressed(0, 3)) {
        _player.state = PlayerState::Moving;
        // cancel the rest
    }
}
