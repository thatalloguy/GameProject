add_library(quack-runtime INTERFACE

)


find_package(Vulkan REQUIRED)


#static lib


file(GLOB_RECURSE QUACK_ENGINE_SRC

        Animations/AnimationManager.h
        Animations/AnimationManager.cpp

        Video/Window.h
        Video/Window.cpp

        Math/Vecs.h
        Utils/Defines.h
        Utils/Array.h

        Input/InputManager.h
        Input/InputManager.cpp

        Renderer/VkEngine.cpp
        Renderer/VkEngine.h
        Renderer/VkInitializers.h
        Renderer/VkImages.cpp
        Renderer/VkImages.h
        Renderer/VkPipelines.cpp
        Renderer/VkPipelines.h
        Renderer/VkDescriptors.cpp
        Renderer/VkDescriptors.h
        Renderer/VkLoader.cpp
        Renderer/VkLoader.h
        Renderer/VkTypes.h
        Renderer/VkInitializers.cpp
        Renderer/Camera.cpp
        Renderer/Camera.h

        Physics/PhysicsEngine.h
        Physics/PhysicsEngine.cpp

        Objects/Entity.h
        Objects/Entity.cpp

        Audio/AudioEngine.cpp
        Audio/AudioEngine.h
)





add_library(quack-runtime-static STATIC ${QUACK_ENGINE_SRC}  ${IMGUI_SOURCE} ${FASTGLTF_DEPS})

target_link_libraries(quack-runtime-static PUBLIC quack-runtime)






target_link_libraries(quack-runtime-static PUBLIC dwmapi)
target_link_libraries(quack-runtime-static PUBLIC glad)
target_link_libraries(quack-runtime-static PUBLIC Jolt)
target_link_libraries(quack-runtime-static PUBLIC tweeny)


target_link_libraries(quack-runtime-static PUBLIC vk-bootstrap::vk-bootstrap)
target_link_libraries(quack-runtime-static PUBLIC GPUOpen::VulkanMemoryAllocator)
target_link_libraries(quack-runtime-static PUBLIC spdlog)
target_link_libraries(quack-runtime-static PUBLIC fastgltf)
target_link_libraries(quack-runtime-static PUBLIC glfw Vulkan::Vulkan)

#TODO make this work work on every platform
if(${MSVC})
    # NOTE THIS DOENST WORK ON RELEASE TARGET ;-;
    target_link_libraries(quack-runtime-static PUBLIC "Thirdparty/steamaudio/lib/windows-x64/phonon")
endif()

target_include_directories(quack-runtime-static PUBLIC "Thirdparty/imgui")
target_include_directories(quack-runtime-static PUBLIC "Thirdparty/stb")
target_include_directories(quack-runtime-static PUBLIC "Thirdparty/glm")
target_include_directories(quack-runtime-static PUBLIC  ${JoltPhysics_SOURCE_DIR}/..)
