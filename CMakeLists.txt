cmake_minimum_required(VERSION 3.28)
project(DuckWatchers)

set(CMAKE_CXX_STANDARD 20)


find_package(Vulkan REQUIRED)


#Third party  dependencies
add_subdirectory("Thirdparty/glfw")
add_subdirectory("Thirdparty/spdlog")
add_subdirectory("Thirdparty/glad")
add_subdirectory("Thirdparty/tweeny")
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


include_directories("Thirdparty/imgui")
include_directories("Thirdparty/stb")
include_directories("Thirdparty/glm")



add_subdirectory(Engine)
add_subdirectory(Runtime)
add_subdirectory(Editor)