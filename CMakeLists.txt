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