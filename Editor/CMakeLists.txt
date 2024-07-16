
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE EDITOR_SRC

        src/App.h
        src/App.cpp

        src/LakeUtils.h

)

add_executable(Lake-Editor main.cpp ${EDITOR_SRC})


target_link_libraries(Lake-Editor PRIVATE quack-runtime-static)