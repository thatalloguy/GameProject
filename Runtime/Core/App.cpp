//
// Created by allos on 6/1/2024.
//

#define USING_JOLT
#pragma once

#include "App.h"
#include "../Character/FisherMan.h"
#include "../Character/DebtCollector.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Objects/Entity.h"
#include "imgui.h"
#include "FishingManager.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/UI/BookUI.h"
#include "Audio/AudioEngine.h"
#include "MapLoader.h"
#include "TimeManager.h"
#include "QuestManager.h"
#include "SoundPlayer.h"


namespace Level {
    Quack::Entity* floor;
    Player* player;
    Quack::Entity* car_trigger;
    Quack::Entity* house_trigger;

    Quack::Entity* Clock;
    Quack::Entity* EvilMoon;
    Quack::Entity* Hand;
    Quack::Entity* Stone;
    Quack::Entity* Shovel;
    Quack::Entity* Moon;
    Quack::Entity* Sun;
    Quack::Entity* Candel;
    Quack::Entity* Reaper;

    Characters::FisherMan fisherMan;
    Characters::DebtCollector debtCollector;



    //Animations.

    tweeny::tween<float> ReaperWalkingAnimation = tweeny::from(-41.0f).to(36.0f).during(700).onStep([](float  pos){
        Level::Reaper->position = Quack::Math::Vector3{pos, 11.7f, -54.0f};

        return false;
    });

    tweeny::tween<float> MoonUpAnimation = tweeny::from(-40.0f).to(128.0f).during(100).onStep([](float y){
        Level::Moon->position.y = y;
        return false;
    });

    tweeny::tween<float> MoonDownAnimation = tweeny::from(128.0f).to(-41.0f).during(100).onStep([](float y){
        Level::Moon->position.y = y;
        return false;
    });

    tweeny::tween<float> SunUpAnimation = tweeny::from(-40.0f).to(128.0f).during(100).onStep([](float y){
        Level::Sun->position.y = y;
        return false;
    });

    tweeny::tween<float> SunDownAnimation = tweeny::from(128.0f).to(-41.0f).during(100).onStep([](float y){
        Level::Sun->position.y = y;
        return false;
    });

    tweeny::tween<float> EvilMoonUpAnimation = tweeny::from(-40.0f).to(128.0f).during(100).onStep([](float y){
        Level::EvilMoon->position.y = y;
        return false;
    });

    tweeny::tween<float> EvilMoonDownAnimation = tweeny::from(128.0f).to(-41.0f).during(100).onStep([](float y){
        Level::EvilMoon->position.y = y;
        return false;
    });

    tweeny::tween<float> handAnimation = tweeny::from(1.4f).to(-4.0f).during(100).onStep([](float y){
        Level::Hand->position.y = y;
        return false;
    });



    DialogBox test{
            .author = "Allo",
            .dialogs = {"Hello world, Its me Allo!\n",  "These typing animations are quite nice!", "Bye bye now!"},
            .next = nullptr
    };



    ImVec2 upBarPos{0, 0};
    ImVec2 downBarPos{0, 720};

    tweeny::tween<float, float> barTween = tweeny::from(0.0f, 720.0f - 50.0f).to(720.0f / 2.0f, 720.0f / 2.0f).during(100).onStep([](float x, float b) {
        Level::upBarPos.y = x;
        Level::downBarPos.y = b;
        return false;
    });;
    bool isTraveling = false;

    void goToBeach() {
        if (!isTraveling) {
            isTraveling = true;
            spdlog::info("Going to the beach");

            Level::car_trigger->position = Quack::Math::Vector3{-16, -8, 194};
        }
    };
    void goToForest() {
        if (!isTraveling) {
            isTraveling = true;
            spdlog::info("Going to the forest");
            Level::car_trigger->position = Quack::Math::Vector3{8, 0, 28};
            Level::barTween = Level::barTween.forward();
        }
    };
    void goToBed();

    static void resizeCall(GLFWwindow* window, int w, int h) {
        downBarPos.y = h;
    }
}


// Core Game Systems
namespace Game {

    bool renderAllUI = true;

    VulkanEngine renderer;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;
    Quack::AudioEngine* audioEngine;

    Loader::MapLoader* mapLoader;
    TimeManager timeManager{};

    FishingManager* fishingManager;
    BookUI* bookUI;

    SoundPlayer* soundPlayer;

    float deltaTime = 0.0f;

    void loadAssets() {
        auto testFile = VkLoader::loadGltf(&Game::renderer, "../../Assets/cube.glb");
        auto sphereFile = VkLoader::loadGltf(&Game::renderer, "../../Assets/Models/Fish.glb");
        auto bobberFile = VkLoader::loadGltf(&Game::renderer, "../../Assets/bobber.glb");
        Game::renderer.loadedScenes[20] = *testFile;
        Game::renderer.loadedScenes[30] = *sphereFile;
        Game::renderer.loadedScenes[40] = *bobberFile;


        Game::mapLoader->loadMap([&](int entityID, Quack::Entity* entity) {

            //FisherMan
            switch (entityID) {
                case 7:
                    Level::fisherMan.initialize(*entity);
                    break;
                case 8:
                    Level::Reaper = entity;
                    break;
                case 10:
                    Level::Clock = entity;
                    break;
                case 11:
                    Level::EvilMoon = entity;
                    break;
                case 12:
                    Level::Hand = entity;
                    break;
                case 13:
                    Level::Stone = entity;
                    break;
                case 14:
                    Level::Shovel = entity;
                    break;
                case 15:
                    Level::Moon = entity;
                    break;
                case 16:
                    Level::Sun = entity;
                    break;
                case 17:
                    Level::Candel = entity;
                    break;
                case 18:
                    Level::debtCollector.initialize(*entity, Game::timeManager);
            }

        });
    }
    void initPhysics() {

        // load Physics
        Game::engineCreationInfo = new Quack::PhysicsEngineCreationInfo{};
        Game::physicsEngine = new Quack::PhysicsEngine(*Game::engineCreationInfo);

    }
}

namespace Tutorial {


    enum class TutorialProgress : unsigned int {
        Fishing = 1,
        Traveling = 2,
        Selling = 3,
        Upgrading = 4,
        Sleeping = 5
    };
    DialogBox fishingTutorial {
        .author = "Fishing Gods",
        .dialogs = {
                "First wait for the fish to bite",
                "After it has bitten the bait",
                "then you must move together with the fish",
                "Do this with either A/D or the left joystick",
                "The blue bar is the fish stamina",
                "The green bar is the line durability"
        }
    };

    DialogBox badEnding {
        .author = "_ _ _ _ _ ",
        .dialogs = {
                "Being unable to pay of their fathers debt",
                "They were murdered in their sleep",
                "Because if they could not pay it with money",
                "They would have to pay it with their soul",
                "They were unable to escape their fate",
                "... ",
                "You were unable to escape your fate"
        }
    };

    DialogBox insaneEnding {
            .author = "_ _ _ _ _ ",
            .dialogs = {
                    "A lack of sleep does unspeakable things.",
                    "Combine that with the loss of a father",
                    "They died in their sleep, their heart stopping",
                    "Their body was never discovered",
                    "Nor was the debt ever paid."
            }
    };

    DialogBox goodEnding {
            .author = "_ _ _ _ _ ",
            .dialogs = {
                    "The next day the debt collector came.",
                    "You were successful in paying the debt",
                    "You escaped your fate, unlike your father"
            }
    };


    Quest _main {
        .desc = "Pay the debt",
        .condition = []() { return false; /* >:) */ },
        .onComplete = []() { spdlog::info("HOW???"); }
    };

    Quest _fishing2 {
            .desc = "read the tutorial",
            .condition = []() {return Level::player->state == PlayerState::Fishing && DialogRenderer::isCurrentConvEmpty(); },
            .onComplete = []() { Game::fishingManager->completeTutorial(); },
            .next = &_main
    };

    Quest _fishingQ {
        .desc = "Go to the dock and Fish",
        .condition = [](){ return Level::player->state == PlayerState::Fishing; },
        .onComplete = []() { Game::fishingManager->startTutorial(); DialogRenderer::setCurrentConversation(&fishingTutorial); },
        .next = &_fishing2
    };


    TutorialProgress _currentProgress = TutorialProgress::Fishing;

    void start() {
        QuestManager::setCurrentQuest(&_fishingQ);
    };

    void update() {
        //
    };




}


namespace UI {

    bool isToBed = false;
    bool playingEnding = false;


    ImVec2 sideBar{0, 720};

    tweeny::tween<float> bedBarTween;

    void startSleepAnimation(float windowW, float windowH) {
        bedBarTween = tweeny::from(0.0f).to(windowW).during(120).onStep([=](float a){
            sideBar.x = a;
            return false;
        }).via(tweeny::easing::cubicIn);


        isToBed = true;
    };

    void render(Quack::Math::Vector2 windowSize) {


        if (isToBed) {

            bedBarTween.step(1);

            if (bedBarTween.direction() != -1 && bedBarTween.progress() >= 1.0f) {

                if (Game::timeManager.getCurrentDay() == Day::Sun) {
                    QuestManager::setCurrentQuest(nullptr);
                    // do ending.
                    if (Level::player->sleepingCounter <= 4) {


                        if (!playingEnding) {
                            spdlog::info("Ending Tick");
                            playingEnding = true;

                            // secret ending
                            Level::player->state = PlayerState::Cutscene;

                            Level::player->state = PlayerState::Cutscene;
                            Level::player->_character->SetPosition({3, -0.2f, 33});
                            Level::player->_camera.pitch = 0.0f;
                            Level::player->_camera.yaw = 0.0f;
                            Level::player->_camera.fov = 0.0f;
                            DialogRenderer::setCurrentConversation(&Tutorial::insaneEnding);
                        }

                    } else {
                        if (Level::player->inventory.money >= 500) {
                            // Good ending
                            if (!playingEnding) {
                                spdlog::info("Ending Tick");
                                playingEnding = true;

                                // secret ending
                                Level::player->state = PlayerState::Cutscene;

                                Level::player->state = PlayerState::Cutscene;
                                Level::player->_character->SetPosition({3, -0.2f, 33});
                                Level::player->_camera.pitch = 0.0f;
                                Level::player->_camera.yaw = 0.0f;
                                Level::player->_camera.fov = 0.0f;
                                DialogRenderer::setCurrentConversation(&Tutorial::goodEnding);
                            }
                        } else {
                            // Bad ending
                            if (!playingEnding) {
                                spdlog::info("Ending Tick");
                                playingEnding = true;

                                Level::player->state = PlayerState::Cutscene;

                                Level::player->state = PlayerState::Cutscene;
                                Level::player->_character->SetPosition({3, -0.2f, 33});
                                Level::player->_camera.pitch = 0.0f;
                                Level::player->_camera.yaw = 0.0f;
                                Level::player->_camera.fov = 0.0f;

                                DialogRenderer::setCurrentConversation(&Tutorial::badEnding);
                            }
                        }
                    }
                    //Game::renderAllUI = false;
                }
                    bedBarTween = bedBarTween.backward().via(tweeny::easing::exponentialOut).during(200);

                    Game::timeManager.setHour(6);
                    Game::timeManager.setDay(static_cast<Day>(static_cast<unsigned int>(Game::timeManager.getCurrentDay()) + 1));
                    Game::timeManager.forcedUpdate();


                Level::player->sleepingCounter++;

            } else if (bedBarTween.direction() <= -1 && bedBarTween.progress() <= 0.0f) {
                isToBed = false;
            }
        }

        auto drawList = ImGui::GetForegroundDrawList();

        auto& fish = Game::fishingManager->dummy;

        float staminaLength = fish.stamina / fish.maxStamina;
        float durabilityLength = Game::fishingManager->fishlineDurability / Game::fishingManager->maxDurability;
        staminaLength = 0.1f + staminaLength * 0.8f;
        durabilityLength = 0.1f + durabilityLength * 0.8f;

        if (Level::player->state == PlayerState::Fishing) {
            drawList->AddRectFilled({windowSize.x * 0.95f, windowSize.y * 0.1f}, {windowSize.x * 0.97f, windowSize.y * staminaLength}, ImColor(50, 250, 200));
            drawList->AddRectFilled({windowSize.x * 0.98f, windowSize.y * 0.1f}, {windowSize.x * 0.99f, windowSize.y * durabilityLength}, ImColor(50, 250, 100));



        }



        std::string date = toCstr(Game::timeManager.getCurrentDay()); date += " | ";
        date += std::to_string(Game::timeManager.getHour()); date += ":00";


        drawList->AddRectFilled(ImVec2(0, 0), sideBar, ImColor(1, 1, 1));
    };

    void update(Quack::Math::Vector2 windowSize) {

    };

    static void resizeCall(GLFWwindow* window, int w, int h) {
        sideBar.y = h;
        Level::downBarPos.y = h;
    }

}

void App::init() {

    //initialize the window
    auto windowCreationData = Quack::WindowCreationData {
      .title = "Duck Watchers Remake"
    };

    window = new Quack::Window(windowCreationData);


    Quack::Input::setTargetWindow(*window);

    Game::renderer.Init(window->getRawWindow(), false);

    Game::initPhysics();


    Game::mapLoader = new Loader::MapLoader(Game::renderer, *Game::physicsEngine);


    Game::timeManager.start();


    Game::loadAssets();

    std::chrono::seconds(1);


    /////// Load the Test Scene

    //Setup camera start.
    Game::camera = &Game::renderer.getMainCamera();
    Game::camera->position.z = 20;
    Game::camera->position.y = 3;


    // Beautiful code, fight me >:(
    Quack::EntityCreationInfo floor_info {
            .position = {0, -5, 0},
            .size = {100, 1.0f, 100},
            .model = 20,
            .isPhysical = false,
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
            .position = {-10, -1.5f, 20},
            .size = {4, 1.0f, 4},
            .model = 20,
            .isPhysical = false,

    };

    Quack::EntityCreationInfo ctriggerI {
            .position = {8, 0, 28},
            .size = {2, 1.0f, 2},
            .model = 20,
            .isPhysical = false,

    };

    Quack::EntityCreationInfo htriggerI {
            .position = {0, 0, 19},
            .size = {3, 1.0f, 3},
            .model = 20,
            .isPhysical = false,

    };

    Level::player = new Player(Game::renderer.getMainCamera(), *Game::physicsEngine);
    Level::floor = new Quack::Entity(floor_info);
    Level::car_trigger = new Quack::Entity(ctriggerI);
    Level::house_trigger = new Quack::Entity(htriggerI);


    Game::fishingManager = new FishingManager(Game::renderer, *Level::player, *Game::physicsEngine);
    Game::bookUI = new BookUI(Game::renderer);

    Game::audioEngine = new Quack::AudioEngine();

    Game::audioEngine->Init();
    Game::soundPlayer = new SoundPlayer(*Game::audioEngine, *Level::player);


}


void App::run() {
    std::chrono::steady_clock::time_point last;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    Game::renderer.debugRenderFuncs.pushFunction([=](){


       ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
       ImGui::Begin("Duck Watcher Debug");

       ImGui::SliderFloat("player speed", &Level::player->speed, 0.1f, 30.0f);
       ImGui::Text("PlayerPos: %f | %f | %f", Level::player->position.x, Level::player->position.y, Level::player->position.z);
       ImGui::Text("PlayerState: %i", static_cast<unsigned int>(Level::player->state));


       ImGui::Separator();
       if (ImGui::Button("Skip a hour")) {
           Game::timeManager.setHour(Game::timeManager.getHour() + 1);
       }

        if (ImGui::Button("Skip a Day")) {
            Game::timeManager.setDay(static_cast<Day>(static_cast<unsigned int>(Game::timeManager.getCurrentDay()) + 1));
            Game::timeManager.forcedUpdate();
        }

       if (ImGui::Button("Play sound")) {
           Game::soundPlayer->playRandomSound();
       }


        if (ImGui::Button("Give 10 bucks")) {
            Level::player->inventory.money += 10;
        }

       ImGui::End();


    });

    Game::renderer.uiRenderFuncs.pushFunction([=](){

        Quack::Math::Vector2 windowSize = window->getSize();


        ImVec2 size{windowSize.x, windowSize.y};

        Level::fisherMan.drawUI(size, *Level::player);

        // dont blame me, blame imgui
        ImGui::SetNextWindowPos(ImVec2(-10000,-10000));
        ImGui::Begin("C");


        auto drawList = ImGui::GetForegroundDrawList();
        if (Game::renderAllUI) {

            std::string date = toCstr(Game::timeManager.getCurrentDay());
            date += " | ";
            date += std::to_string(Game::timeManager.getHour());
            date += ":00";

            if (Level::player->state == PlayerState::Moving) {
                ImGui::SetWindowFontScale(4.0f);
                drawList->AddText(ImVec2{windowSize.x * 0.05f, windowSize.y * 0.05f}, ImColor(255, 255, 255),
                                  date.c_str());
                ImGui::SetWindowFontScale(1.0f);
            }

            // car logic
            if (Level::car_trigger->hasHit(Level::player->position)) {
                if (Quack::Input::isControllerPresent(0)) {
                    ImGui::SetWindowFontScale(4.0f);
                    drawList->AddText(ImVec2{windowSize.x * 0.4f, windowSize.y * 0.7f}, ImColor(255, 255, 255),
                                      "Press Left to travel");
                    ImGui::SetWindowFontScale(1.0f);

                    if (Quack::Input::isButtonPressed(0, 0)) {
                        // check if we are in the forest and we are allowed to travel to the beach
                        if (Level::player->position.z <= 60 && Game::timeManager.getHour() <= 19) {
                            Level::goToBeach();
                            Level::barTween = tweeny::from(0.0f, windowSize.y - 50).to(windowSize.y / 2.0f,
                                                                                       windowSize.y / 2.0f).during(
                                    100).onStep([=, this](float x, float b) {
                                Level::upBarPos.y = x;
                                Level::downBarPos.y = b;
                                return false;
                            });
                        } else {
                            Level::goToForest();
                        }
                    }
                } else {
                    ImGui::SetWindowFontScale(4.0f);
                    drawList->AddText(ImVec2{windowSize.x * 0.4f, windowSize.y * 0.7f}, ImColor(255, 255, 255),
                                      "Press E to travel");
                    ImGui::SetWindowFontScale(1.0f);

                    if (Quack::Input::isKeyPressed(Quack::Key::E)) {
                        // check if we are in the forest and we are allowed to travel to the beach
                        if (Level::player->position.z <= 60 && Game::timeManager.getHour() <= 19) {
                            Level::goToBeach();
                            Level::barTween = tweeny::from(0.0f, windowSize.y - 50).to(windowSize.y / 2.0f,
                                                                                       windowSize.y / 2.0f).during(
                                    100).onStep([=, this](float x, float b) {
                                Level::upBarPos.y = x;
                                Level::downBarPos.y = b;
                                return false;
                            });
                        } else {
                            Level::goToForest();
                        }
                    }
                }
            }

            // house logic
            if (Level::house_trigger->hasHit(Level::player->position) && Game::timeManager.getHour() >= 19) {
                if (Quack::Input::isControllerPresent(0)) {
                    ImGui::SetWindowFontScale(4.0f);
                    drawList->AddText(ImVec2{windowSize.x * 0.4f, windowSize.y * 0.7f}, ImColor(255, 255, 255),
                                      "Press DOWN to sleep");
                    ImGui::SetWindowFontScale(1.0f);

                    if (Quack::Input::isButtonPressed(0, 0)) {
                        UI::startSleepAnimation(windowSize.x, windowSize.y);
                    }
                } else {
                    ImGui::SetWindowFontScale(4.0f);
                    drawList->AddText(ImVec2{windowSize.x * 0.4f, windowSize.y * 0.7f}, ImColor(255, 255, 255),
                                      "Press E to Sleep");
                    ImGui::SetWindowFontScale(1.0f);

                    if (Quack::Input::isKeyPressed(Quack::Key::E)) {
                        UI::startSleepAnimation(windowSize.x, windowSize.y);
                    }
                }
            }


            QuestManager::renderUI(size);
        }
        UI::render(windowSize);
        DialogRenderer::render(size);

        if (Game::renderAllUI) {
            // The cinematic bars
            drawList->AddRectFilled(ImVec2{0, 0}, ImVec2{windowSize.x, Level::upBarPos.y + 50}, ImColor(1, 1, 1));
            drawList->AddRectFilled(ImVec2{0, windowSize.y}, ImVec2{windowSize.x, Level::downBarPos.y - 50},
                                    ImColor(1, 1, 1));

        }

        ImGui::End();
    });

    bool toggle = false;
    bool booktoggle = false;

    auto& sky = Game::renderer.backgroundEffects[0].data;


    glfwSetWindowSizeCallback(window->getRawWindow(), UI::resizeCall);


    Quack::Input::setMouseMode(Quack::MouseMode::Disabled);


    Tutorial::start();

    while (!window->shouldShutdown()) {
        Game::renderer.updateScene();


        Game::physicsEngine->update();

        Level::floor->render(Game::renderer);
        Game::mapLoader->renderMap();
        Level::floor->updatePhysics(*Game::physicsEngine);
        Level::player->update(Game::deltaTime);
        Level::fisherMan.update(*Level::player);
        Level::debtCollector.update(*Level::player);

        //Event Entities
        switch (Game::timeManager.getCurrentDay()) {
            case Day::Mon:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 4) {
                    // day
                    Level::SunUpAnimation.step(1);
                } else if (Game::timeManager.getHour() <= 4) {
                    // prev night
                } else {
                    //current night
                    Level::SunDownAnimation.step(1);
                    Level::EvilMoonUpAnimation.step(1);
                }
                break;
            case Day::Tue:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 12) {
                    // day
                    Level::EvilMoonDownAnimation.step(1);
                    Level::ReaperWalkingAnimation.step(1);

                } else if (Game::timeManager.getHour() <= 4) {
                    // prev night
                } else if (Game::timeManager.getHour() >= 20) {
                    //current night
                    Level::Reaper->position = Quack::Math::Vector3{-13.4f, 9.5f, -47.2f};
                    Level::Reaper->rotation.y = -147.9f;
                    Level::SunDownAnimation.step(1.0f);
                }

                break;
            case Day::Wen:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 4) {
                    Level::Reaper->position = Quack::Math::Vector3{-13.4f, -20.5f, -47.2f};
                }
                break;
            case Day::Thu:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 4) {
                    // day
                    Level::Clock->position.y = 4;
                } else if (Game::timeManager.getHour() >= 20) {
                    //current night
                    Level::handAnimation.step(1);
                }
                break;
            case Day::Fri:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 4) {
                    // day
                    Level::Stone->position.y = 0.2f;
                } else if (Game::timeManager.getHour() <= 4) {
                    // prev night
                } else {
                    //current night
                }
                break;
            case Day::Sat:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 4) {
                    // day
                } else if (Game::timeManager.getHour() <= 4) {
                    // prev night
                } else {
                    //current night
                    Level::Candel->position.y = 3.9f;
                }
                break;
            case Day::Sun:
                if (Game::timeManager.getHour() <= 20 && Game::timeManager.getHour() >= 4) {
                    // day
                    Level::Candel->position.y = -13.9f;
                }
                break;
            case Day::End:
                break;
        }


        //via f3 you can toggle debug menu
        if (Quack::Input::isKeyPressed(Quack::Key::F3) && toggle) {
            Game::renderer.displayDebugMenu = !Game::renderer.displayDebugMenu;
            if (Game::renderer.displayDebugMenu) {
                Quack::Input::setMouseMode(Quack::MouseMode::Normal);
            } else {
                Quack::Input::setMouseMode(Quack::MouseMode::Disabled);
            }

            toggle = false;
        } else if (!Quack::Input::isKeyPressed(Quack::Key::F3)) {
            toggle = true;
        }

        if (Quack::Input::isKeyPressed(Quack::Key::F5) && booktoggle) {
            Game::bookUI->shouldRender(!Game::bookUI->isRendering());
            booktoggle = false;
        } else if (!Quack::Input::isKeyPressed(Quack::Key::F5)) {
            booktoggle = true;
        }

        Game::fishingManager->Update(Game::deltaTime);


        Game::renderer.Run();


        Game::soundPlayer->update();

        Game::timeManager.tick();

        window->update();

        UI::update(window->getSize());
        QuestManager::update();

        if (Level::isTraveling) {
            Level::barTween.step(1);

            if (Level::barTween.direction() == -1 && Level::barTween.progress() <= 0.02f) {
                Level::isTraveling = false;
            }

            if (Level::barTween.progress() >= 1.0f) {
                if (Level::isTraveling) {


                    if (Level::player->position.z < 100) {
                        Level::player->_character->SetPosition({-14, -8, 194});
                    } else {
                        Level::player->_character->SetPosition({9, 0, 28});
                    }
                    Level::barTween = Level::barTween.backward();
                }



            }

        }


        //Update the delta time
        auto now = std::chrono::steady_clock::now();
        Game::deltaTime = (float) std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() / 1000000.0f;
        last = now;

        sky.data1.x = Game::timeManager.skyCol.x;
        sky.data1.y = Game::timeManager.skyCol.y;
        sky.data1.z = Game::timeManager.skyCol.z;

        Game::renderer.sceneData.sunLightDirection.y = Game::timeManager.skyCol.w;


    }

}

void App::cleanup() {
    delete Level::player;
    delete Level::floor;
    delete Level::car_trigger;
    delete Level::house_trigger;


    delete Game::fishingManager;
    delete Game::mapLoader;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;
    delete Game::bookUI;

    Game::audioEngine->CleanUp();
    delete Game::audioEngine;

    delete Game::soundPlayer;

    Game::renderer.CleanUp();

    delete window;
}
