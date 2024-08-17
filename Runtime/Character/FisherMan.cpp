//
// Created by allos on 8/7/2024.
//

#include "FisherMan.h"

void Characters::FisherMan::initialize(Quack::Entity &fisherManBaseEntity) {

    _entity = &fisherManBaseEntity;

    setupTrigger();
}

Characters::FisherMan::~FisherMan() {
    delete _trigger;
}

void Characters::FisherMan::setupTrigger() {
    Quack::EntityCreationInfo triggerInfo = {
            .position {_entity->position.x, _entity->position.y - 3, _entity->position.z - 6},
            .size = {3, 8, 5}
    };

    _trigger = new Quack::Entity(triggerInfo);
}

void Characters::FisherMan::update(Player &player) {

    if (_trigger->hasHit(player.position) && player.state == PlayerState::Moving) {
        if (Quack::Input::isControllerPresent(0)) {
            if (Quack::Input::isButtonPressed(0, 0)) {
                spdlog::info("Fisherman Interaction");
                shopStart(player);
            }
        } else {
            if (Quack::Input::isKeyPressed(Quack::Key::E)) {
                spdlog::info("Fisherman Interaction");
                shopStart(player);
            }
        }
    }
}

void Characters::FisherMan::drawUI(ImVec2 windowSize, Player& player) {

    auto drawList = ImGui::GetForegroundDrawList();

    if (player.state == PlayerState::Shopping) {
        ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.3f, windowSize.y * 0.6f));
        ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.4f, windowSize.y * 0.3f));
        ImGui::Begin("Bob the FisherMan", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImGui::SetWindowFontScale(sqrt((windowSize.x * windowSize.x) + (windowSize.y * windowSize.y)) / 850);

        switch (_currentTab) {

            case ShopTab::Selection:
                drawSelectionTab(windowSize, player);
                break;

            case ShopTab::Dialog:
                //drawSelectionTab(windowSize, player);
                break;

            case ShopTab::Selling:
                drawSellingTab(windowSize, player);
                break;

            case ShopTab::Upgrading:
                drawUpgradingTab(windowSize, player);
                break;

            default:
                drawSelectionTab(windowSize, player);
                break;
        }

        ImGui::SetWindowFontScale(1.0f);

        ImGui::End();
    } else if (_trigger->hasHit(player.position)) {
        ImGui::SetNextWindowPos(ImVec2(-700, -700));
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        ImGui::Begin(" ", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImGui::SetWindowFontScale(sqrt((windowSize.x * windowSize.x) + (windowSize.y * windowSize.y)) / 600);

        auto drawList = ImGui::GetForegroundDrawList();

        if (Quack::Input::isControllerPresent(0)) {
            drawList->AddText(ImVec2(windowSize.x * 0.4f, windowSize.y * 0.8f), ImColor(255, 255, 255), "Press Down to interact");
        } else {
            drawList->AddText(ImVec2(windowSize.x * 0.4f, windowSize.y * 0.8f), ImColor(255, 255, 255), "Press E to interact");
        }

        ImGui::SetWindowFontScale(1.0f);
        ImGui::End();
    }
}

void Characters::FisherMan::drawSellingTab(ImVec2 windowSize, Player &player) {
    ImGui::Text("Ya wanna Sell?");
    ImGui::Text("Il pay 10 for each");


    ImGui::Text(" ");
    ImGui::Text(" ");

    if (ImGui::Button("Nevermind")) {
        _currentTab = ShopTab::Selection;
    }
    ImGui::SameLine();

    if (ImGui::Button("Sell")) {
        _currentTab = ShopTab::Selection;
        // do selling stuff :)
        player.inventory.money += (int) player.inventory.fish * 10;
        player.inventory.fish = 0;
    }
}

void Characters::FisherMan::drawUpgradingTab(ImVec2 windowSize, Player &player) {

    ImGui::Text("Ya wanna Upgrade?");
    ImGui::Text("cost ye a 50");
    ImGui::Text("|| Your FishingRod Level: %i ||", player.inventory.fishingRodLevel);
    ImGui::Text("|| Your Money: %i ||", player.inventory.money);



    if (ImGui::Button("Nevermind")) {
        _currentTab = ShopTab::Selection;
    }
    ImGui::SameLine();

    if (ImGui::Button("Upgrade")) {
        _currentTab = ShopTab::Selection;
        // do upgrade stuff :)
        if (player.inventory.money >= 50) {
            player.inventory.fishingRodLevel += 1;
            player.inventory.money -= 50;
        } else {
            // play sound effect
        }
    }
}

void Characters::FisherMan::drawSelectionTab(ImVec2 windowSize, Player &player) {
    ImGui::Text("What would ya like?");

    ImGui::Text(" ");
    ImGui::Text(" ");
    ImGui::Text(" ");
    if (ImGui::Button("Sell Fish")) {
        _currentTab = ShopTab::Selling;
    }
    ImGui::SameLine();
    if (ImGui::Button("Upgrade Fishing Rod")) {
        _currentTab = ShopTab::Upgrading;
    }
    ImGui::SameLine();

    if (ImGui::Button("Bye")) {
        shopEnd(player);
    }
}

void Characters::FisherMan::shopStart(Player& player) {
    player.state = PlayerState::Shopping;
    player._camera.yaw = -3.2f;
    player._camera.pitch = 0.3f;

    player.position.x = -16;
    player.position.z = 204;
    Quack::Input::setMouseMode(Quack::MouseMode::Normal);
}

void Characters::FisherMan::shopEnd(Player& player) {
    player.state = PlayerState::Moving;
    //unneeded?
    _currentTab = ShopTab::Selection;
    Quack::Input::setMouseMode(Quack::MouseMode::Disabled);
}