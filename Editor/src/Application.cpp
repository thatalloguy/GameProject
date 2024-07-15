//
// Created by allos on 7/2/2024.
//

#include "Application.h"
#include "imgui.h"

#include "backends/imgui_impl_vulkan.h"
#include "ImTween.h"
#include "imgui_internal.h"
#include "Input/InputManager.h"

#include <IconsFontAwesome6.h>
#include <iostream>

#include <spdlog/sinks/callback_sink.h>


namespace Utils {

    std::string filePath = "-1";
    std::string fileName = "-1";
    bool hasDroppedFile = false;

}



void Lake::Application::dropCallback(GLFWwindow *window, int count, const char **paths) {

    std::string path = paths[0];

    if (path.substr(path.find_last_of(".") + 1) == "glb") {
        Utils::filePath = paths[0];
        Utils::fileName = path.substr(path.find_last_of("/\\") + 1);
        Utils::hasDroppedFile = true;



    } else {
        spdlog::error("File dropped is not supported.");
    }

}



void Lake::Application::Init(ProjectManager* projectManager, AssetManager* assetManager, EntityManager* entityManager) {

    Quack::WindowCreationData windowCreationData {
            .title = "Lake Editor Version 0.0.1"
    };

    _projectMang = projectManager;
    _assetManager = assetManager;
    _entityManager = entityManager;

    _window = new Quack::Window(windowCreationData);

    _renderer = new VulkanEngine();

    _renderer->Init(_window->getRawWindow());

    _assetManager->_renderer = _renderer;

    loadImGuiStyle();
    loadImGuiFont();

    glfwSetDropCallback(_window->getRawWindow(), dropCallback);
    Quack::Input::setTargetWindow(*_window);

    auto callback = std::make_shared<spdlog::sinks::callback_sink_mt>([&](const spdlog::details::log_msg &msg) {
        //Awful looking code but hey, who cares :)
        consoleBuffer.append(to_short_c_str(msg.level));
        consoleBuffer.append(" | ");
        consoleBuffer.append(msg.payload.data());
        consoleBuffer.append("\n");
    });

    _logger = new spdlog::logger("console", callback);

    _logger->info("Hello world :)");

}

void Lake::Application::Run() {

    _renderer->uiRenderFuncs.pushFunction([=, this](){
        int width, height;
        glfwGetWindowSize(_window->getRawWindow(), &width, &height);

        _editorWindowSize.x = width;
        _editorWindowSize.y = height;

        if (Quack::Input::isKeyPressed(Quack::Key::F1) && toggle) {
            _renderEditor = !_renderConsole;
            _renderConsole = !_renderConsole; // set it to the same value so that you want swap them when the button is pressed
            toggle = false;
        } else if (!Quack::Input::isKeyPressed(Quack::Key::F1)) {
            toggle = true;
        }

        renderEditorTab();

        renderConsoleTab();


    });



    while (!_window->shouldShutdown()) {
        // Handle file drag and dropping
        if (Utils::hasDroppedFile) {
            Utils::hasDroppedFile = false;
            _projectMang->copyFileToProjectAssetFolder(Utils::filePath.c_str(), Utils::fileName.c_str());
            _assetManager->newAsset(Utils::fileName.c_str(), Quack::AssetType::Model);
        }
        _renderer->updateScene();

        _entityManager->renderEntitiesToScreen(*_renderer);
        _renderer->Run();
        _window->update();
    }

}

void Lake::Application::renderEditorTab() {
    ImTween<float>::Tween(
            std::tuple{0.75f, 0.975f, &_editorWindowOffset})
            .OfType(ImTween<>::PingPong)
            .Speed(0.03f)
            .When([&](){
                return !_renderEditor;
            }).Tick();

    ImGui::SetNextWindowPos({_editorWindowSize.x * _editorWindowOffset,0});
    ImGui::SetNextWindowSize({_editorWindowSize.x * 0.25f, (float) _editorWindowSize.y});
    ImGui::Begin(ICON_FA_BARS_STAGGERED " Editor", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);

    if (ImGui::Button(_renderEditor ? ICON_FA_ARROW_RIGHT : ICON_FA_ARROW_LEFT)) {
        _renderEditor = !_renderEditor;
    }
    ImGui::Separator();
    if (_renderEditor) {
        ImGui::BeginTabBar("tabs");
        if (ImGui::BeginTabItem(ICON_FA_PERSON " Entities")) {

            // search / selection stuff


            ImGui::BeginChild("tree", ImVec2(_editorWindowSize.x * 0.25f, _editorWindowSize.y * 0.3f));
            if (ImGui::Button(ICON_FA_USER_PLUS)) {
                _entityManager->newEntity();
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::SetTooltip("Create a new entity");
            }

            _entityManager->renderEntityTree();

            ImGui::EndChild();

            // Object inspector
            ImGui::BeginChild("inspector", ImVec2(_editorWindowSize.x * 0.25f, _editorWindowSize.y * 0.6f), ImGuiChildFlags_Border);

            _entityManager->renderEntityInfo(_editorWindowSize.x);

            ImGui::EndChild();


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_FA_FOLDER_TREE " Assets")) {

            _assetManager->renderAssetUI(_editorWindowSize.x,_editorWindowSize.y);

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();

    }


    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu(ICON_FA_FOLDER " Project")) {

            if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK" Save All")) {
                _assetManager->exportAssetData();

                // other save.
            }

            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }
    ImGui::End();
}

void Lake::Application::renderConsoleTab() {
    ImTween<float>::Tween(
            std::tuple{0.75f, 0.975f, &_consoleWindowOffset})
            .OfType(ImTween<>::PingPong)
            .Speed(0.05f)
            .When([&](){
                return !_renderConsole;
            }).Tick();

    ImTween<float>::Tween(
            std::tuple{0.65f, 0.75f, &_consoleWindowOffset})
            .OfType(ImTween<>::PingPong)
            .Speed(0.02f)
            .When([&](){
                return !_renderAnimator;
            }).Tick();

    ImGui::SetNextWindowPos({0,_editorWindowSize.y * _consoleWindowOffset});
    ImGui::SetNextWindowSize({_editorWindowSize.x * 0.75f, (float) _editorWindowSize.y * 1 - _consoleWindowOffset});
    ImGui::Begin(ICON_FA_GEARS " Utilities", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);

    // Render cool button
    const ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImRect titleBarRect = window->TitleBarRect();
    ImGui::PushClipRect( titleBarRect.Min, titleBarRect.Max, false );
    ImGui::SetCursorPos( ImVec2( titleBarRect.Max.x - 20.0f, 0.0f ) );
    if (ImGui::Button( _renderConsole ? ICON_FA_ARROW_DOWN : ICON_FA_ARROW_UP)) {
        _renderConsole = !_renderConsole;
    };
    ImGui::PopClipRect();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_SpanAllColumns);

    if (_renderConsole) {
        //render the actuall console
        ImGui::BeginTabBar("tabs");
        if (ImGui::BeginTabItem(ICON_FA_TERMINAL " Console")) {
            _renderAnimator = false;

            ImGui::BeginChild("Logs");
            ImGui::BeginDisabled();
            ImGui::TextUnformatted(consoleBuffer.begin());
            ImGui::EndDisabled();

            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_FA_CLOCK " Animator")) {
            _renderAnimator = true;
            ImGui::Text("animator :)");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Lake::Application::Destroy() {

    delete _window;
    delete _logger;

    _renderer->CleanUp();
    delete _renderer;

}

void Lake::Application::loadImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    auto& imGuiIO{ ImGui::GetIO() };
    imGuiIO.IniFilename = NULL;
    imGuiIO.LogFilename = NULL;
    imGuiIO.FontGlobalScale = 0.3f; // tf did i need this for back in lomus?
    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 5.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.FrameRounding = 7.5f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5921568870544434f, 0.5921568870544434f, 0.5921568870544434f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1400980454683304f, 0.1400980454683304f, 0.1400196138620377f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.1f, 0.1f, 0.104f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1350980454683304f, 0.1350980454683304f, 0.1390196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);

}

void Lake::Application::loadImGuiFont() {


    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    ImFontConfig c;
    c.SizePixels = 50.0f;
    io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/Roboto-Regular.ttf", 60.0f);

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    auto f = io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/fa-solid-900.ttf", 50.0f, &config, icon_ranges);

    io.Fonts->Build();

    ImGui_ImplVulkan_DestroyFontsTexture();

    ImGui_ImplVulkan_CreateFontsTexture();
}
