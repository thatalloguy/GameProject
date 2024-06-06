//
// Created by allos on 6/1/2024.
//

#define USING_JOLT

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Renderer/VkEngine.h"
#include "Physics/PhysicsEngine.h"

struct BodyCreationInfo {
    RVec3 position;
    Quat rotation;
    Shape* shape = nullptr;
    EActivation shouldActivate = EActivation::DontActivate;
    EMotionType motionType = EMotionType::Static;
    JPH::ObjectLayer layer = 0; // non moving

    Quack::PhysicsEngine* physicsEngine = nullptr;
};

struct EntityCreationInfo {
    Quack::Math::Vector3 position{0, 0, 0};
    std::string* model = nullptr;
    bool isDynamic = false;
    bool isPhysical = false;
    BodyCreationInfo bodyCreationInfo{};
};

class Entity {

public:
    Entity(EntityCreationInfo& creationInfo) {
        parseInfo(creationInfo);
    };

    ~Entity() {
        //temp?
        if (body_interface) {
            body_interface->RemoveBody(physicsID);

            body_interface->DestroyBody(physicsID);
        }
        delete model;
    };


    void render(VulkanEngine& engine) {
        // only render if we have a model attached.
        if (model != nullptr) {
            engine.loadedScenes[*model]->Draw(getTransformMatrix(), engine.mainDrawContext);
        }
    };

    void updatePhysics(Quack::PhysicsEngine& physicsEngine) {
        if (body_interface) {
            position = physicsEngine.getInterface().GetCenterOfMassPosition(physicsID);
        }
    }

    void setPosition(Quack::Math::Vector3 vec) {
        position = vec;
    }

private:
    Quack::Math::Vector3 position;
    std::string* model = nullptr;
    BodyID physicsID{};
    BodyInterface* body_interface = nullptr;

    void parseInfo(EntityCreationInfo& info) {

        this->model = info.model;
        this->position = info.position;

        if (info.isPhysical && info.bodyCreationInfo.physicsEngine != nullptr) {
            BodyCreationSettings settings(info.bodyCreationInfo.shape, info.bodyCreationInfo.position, info.bodyCreationInfo.rotation, info.bodyCreationInfo.motionType, info.bodyCreationInfo.layer);
            physicsID = info.bodyCreationInfo.physicsEngine->addNewBody(settings, info.bodyCreationInfo.shouldActivate);
            body_interface = &info.bodyCreationInfo.physicsEngine->getInterface();
        }
    }

    inline glm::mat4 getTransformMatrix() {
        return glm::translate(glm::mat4{1.f}, glm::vec3{position.x, position.y, position.z});
    }
};

namespace Game {
    VulkanEngine engine;
    Camera* camera;
    Quack::PhysicsEngine* physicsEngine;
    Quack::PhysicsEngineCreationInfo* engineCreationInfo;



    Entity* testEntity;
}

void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});
    Quack::Input::setTargetWindow(*window);

    Game::engine.Init(window->getRawWindow(), true);


    std::string structurePath = {"..//Assets/basicmesh.glb"};
    auto structureFile = VkLoader::loadGltf(&Game::engine, structurePath);
    // just a check, not necessary
    assert(structureFile.has_value());

    Game::engine.loadedScenes["cube"] = *structureFile;

    Game::camera = &Game::engine.getMainCamera();
    Game::camera->position.z = 20;


    // load Physics
    Game::engineCreationInfo = new Quack::PhysicsEngineCreationInfo{};
    Game::physicsEngine = new Quack::PhysicsEngine(*Game::engineCreationInfo);

    // Beautiful code, fight me >:(
    EntityCreationInfo info {
            .model = new std::string("cube"),
            .isPhysical = true,
            .bodyCreationInfo = {
                    .position = {0, 6, 0},
                    .rotation = Quat::sIdentity(),
                    .shape = new SphereShape(0.5f),
                    .shouldActivate = EActivation::Activate,
                    .motionType = EMotionType::Dynamic,
                    .layer = Quack::Layers::MOVING,
                    .physicsEngine = Game::physicsEngine
            }
    };

    Game::testEntity = new Entity(info);



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

        Game::testEntity->render(Game::engine);



            //Game::testEntity->setPosition(Game::physicsEngine->getSpherePos());

            Game::testEntity->updatePhysics(*Game::physicsEngine);

            Game::physicsEngine->update();



        Game::engine.Run();

        window->update();
    }

}

void App::cleanup() {
    delete Game::testEntity;
    delete Game::physicsEngine;
    delete Game::engineCreationInfo;


    Game::engine.CleanUp();
    delete window;
}
