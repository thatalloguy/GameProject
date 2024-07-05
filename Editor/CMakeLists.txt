
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE EDITOR_SRC

        src/LakeUtils.h
        src/ProjectManager.h
        src/ProjectManager.cpp
        src/AssetManager.h
        src/AssetManager.cpp
        src/EntityManager.h
        src/EntityManager.cpp

        src/Application.h
        src/Application.cpp


)

add_executable(Lake-Editor main.cpp ${EDITOR_SRC})


target_link_libraries(Lake-Editor PRIVATE quack-runtime-static)