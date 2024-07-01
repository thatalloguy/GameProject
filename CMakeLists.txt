cmake_minimum_required(VERSION 3.28)
project(Game)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE ENGINE_SOURCE
        Engine/Video/Window.h
        Engine/Video/Window.cpp

        Engine/Math/Vecs.h
        Engine/Utils/Defines.h

        Engine/Input/InputManager.h
        Engine/Input/InputManager.cpp

        Engine/Renderer/VkEngine.cpp
        Engine/Renderer/VkEngine.h
        Engine/Renderer/VkInitializers.h
        Engine/Renderer/VkImages.cpp
        Engine/Renderer/VkImages.h
        Engine/Renderer/VkPipelines.cpp
        Engine/Renderer/VkPipelines.h
        Engine/Renderer/VkDescriptors.cpp
        Engine/Renderer/VkDescriptors.h
        Engine/Renderer/VkLoader.cpp
        Engine/Renderer/VkLoader.h
        Engine/Renderer/VkTypes.h
        Engine/Renderer/VkInitializers.cpp
        Engine/Renderer/Camera.cpp
        Engine/Renderer/Camera.h

        Engine/Physics/PhysicsEngine.h
        Engine/Physics/PhysicsEngine.cpp

        Engine/Objects/Entity.h
        Engine/Objects/Entity.cpp

        Engine/Audio/AudioEngine.cpp
        Engine/Audio/AudioEngine.h
)


file(GLOB_RECURSE RUNTIME_SOURCE
        Runtime/Core/App.h
        Runtime/Core/App.cpp

        Runtime/Core/FishingManager.h
        Runtime/Core/FishingManager.cpp

        Runtime/Inventory/Inventory.h

        Runtime/Inventory/UI/BookUI.h
        Runtime/Inventory/UI/BookUI.cpp

        Runtime/Inventory/UI/Pages/Testpage.h

        Runtime/Inventory/Items/FishingRod.h
        Runtime/Inventory/Items/FishingRod.cpp
)

#Third party  dependencies
add_subdirectory("Thirdparty/glfw")
add_subdirectory("Thirdparty/spdlog")
add_subdirectory("Thirdparty/glad")
add_subdirectory("Thirdparty/tweeny")


find_package(Vulkan REQUIRED)


add_subdirectory("Thirdparty/vk-bootstrap")
add_subdirectory("Thirdparty/VulkanMemoryAllocator")
add_subdirectory("Thirdparty/fastgltf")
add_subdirectory("Thirdparty/Jolt")

file(GLOB_RECURSE IMGUI_SOURCE
        Thirdparty/imgui/imgui.cpp
        Thirdparty/imgui/imgui.h
        Thirdparty/imgui/imgui_internal.h
        Thirdparty/imgui/imgui_internal.cpp
        Thirdparty/imgui/imconfig.h
        Thirdparty/imgui/imstb_rectpack.h
        Thirdparty/imgui/imgui_draw.cpp
        Thirdparty/imgui/imgui_tables.cpp
        Thirdparty/imgui/imgui_widgets.cpp
        Thirdparty/imgui/imgui_demo.cpp
        Thirdparty/imgui/backends/imgui_impl_vulkan.h
        Thirdparty/imgui/backends/imgui_impl_vulkan.cpp
        Thirdparty/imgui/backends/imgui_impl_glfw.h
        Thirdparty/imgui/backends/imgui_impl_glfw.cpp
)

file(GLOB_RECURSE FASTGLTF_DEPS
        Thirdparty/fastgltf/deps/simdjson/simdjson.h
        Thirdparty/fastgltf/deps/simdjson/simdjson.cpp
)




include_directories("Engine")
include_directories("Thirdparty/glfw/include")
include_directories("Thirdparty/glad/include")
include_directories("Thirdparty/spdlog/include")
include_directories("Thirdparty/steamaudio/include")
include_directories("Thirdparty/Miniaudio/")






add_executable(Game Runtime/main.cpp ${RUNTIME_SOURCE} ${ENGINE_SOURCE} ${IMGUI_SOURCE} ${FASTGLTF_DEPS})
add_subdirectory(Editor)

target_link_libraries(Game PUBLIC dwmapi)
target_link_libraries(Game PUBLIC glad)
target_link_libraries(Game PUBLIC Jolt)
target_link_libraries(Game PUBLIC tweeny)


target_link_libraries(Game PRIVATE vk-bootstrap::vk-bootstrap)
target_link_libraries(Game PRIVATE GPUOpen::VulkanMemoryAllocator)
target_link_libraries(Game PRIVATE spdlog)
target_link_libraries(Game PRIVATE fastgltf)
target_link_libraries(Game PRIVATE glfw Vulkan::Vulkan)

#TODO make this work work on every platform
if(${MSVC})
    # NOTE THIS DOENST WORK ON RELEASE TARGET ;-;
    target_link_libraries(Game PUBLIC "Thirdparty/steamaudio/lib/windows-x64/phonon")
endif()

target_include_directories(Game PRIVATE "Thirdparty/imgui")
target_include_directories(Game PRIVATE "Thirdparty/stb")
target_include_directories(Game PRIVATE "Thirdparty/glm")
target_include_directories(Game PUBLIC  ${JoltPhysics_SOURCE_DIR}/..)
