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
)


file(GLOB_RECURSE RUNTIME_SOURCE
        Runtime/Core/App.h
        Runtime/Core/App.cpp
)

#Third party  dependencies
add_subdirectory("Thirdparty/glfw")
add_subdirectory("Thirdparty/spdlog")
add_subdirectory("Thirdparty/glad")



include_directories("Engine")
include_directories("Thirdparty/glfw/include")
include_directories("Thirdparty/glad/include")
include_directories("Thirdparty/spdlog/include")





add_executable(Game Runtime/main.cpp ${RUNTIME_SOURCE} ${ENGINE_SOURCE})


target_link_libraries(Game PUBLIC dwmapi)
target_link_libraries(Game PUBLIC glfw)
target_link_libraries(Game PUBLIC glad)