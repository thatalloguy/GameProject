//
// Created by allos on 6/1/2024.
//



#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Renderer/VkEngine.h"





class Entity {

public:
    Entity() = default;
    ~Entity() {

        //temp?
        delete model;
    };

    void attachModel(std::string* assetName) {
        model = assetName;
    }

    void render(VulkanEngine& engine) {
        // only render if we have a model attached.
        if (model != nullptr) {
            engine.loadedScenes[*model]->Draw(getTransformMatrix(), engine.mainDrawContext);
        }
    };

private:
    Quack::Math::Vector3 position{0, 0, 0};

    std::string* model = nullptr;

    glm::mat4 getTransformMatrix() {
        return glm::translate(glm::mat4{1.f}, glm::vec3{position.x, position.y, position.z});
    }
};

namespace Game {
    VulkanEngine engine;
    Camera* camera;

    Entity testEntity;
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

                                            // Beautiful code, fight me >:(
    Game::testEntity.attachModel(new std::string("cube"));

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

        Game::testEntity.render(Game::engine);

        Game::engine.Run();

        window->update();
    }

}

void App::cleanup() {
    Game::engine.CleanUp();
    delete window;
}
