//
// Created by allos on 6/1/2024.
//

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"
#include "Renderer/VkEngine.h"

namespace Game {
    VulkanEngine engine;
    Camera* camera;
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
}

void App::run() {
    int count;
    while (!window->shouldShutdown()) {

        if (Quack::Input::isKeyPressed(Quack::Key::A)) {
            spdlog::info("A KEY PRESSED");
        }

       /* if (Quack::Input::isButtonPressed(0, 2)) {
            spdlog::info("A BUTTON PRESSED");
        }
*/
        Game::engine.updateScene();

        Game::engine.loadedScenes["cube"]->Draw(glm::mat4{1.f}, Game::engine.mainDrawContext);

        Game::engine.Run();

        window->update();
    }

}

void App::cleanup() {
    Game::engine.CleanUp();
    delete window;
}
