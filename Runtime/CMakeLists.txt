
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE GAME_SRC

        Core/App.h
        Core/App.cpp

        Core/FishingManager.h
        Core/FishingManager.cpp

        Core/MapLoader.h
        Core/MapLoader.cpp

        Core/TimeManager.h
        Core/TimeManager.cpp

        Inventory/Inventory.h
        Inventory/Items/FishingRod.h
        Inventory/Items/FishingRod.cpp

        Inventory/UI/BookUI.h
        Inventory/UI/BookUI.cpp
        Inventory/UI/Pages/Testpage.h

        Character/DialogSystem.h
        Character/DialogSystem.cpp
        Character/FisherMan.h
        Character/FisherMan.cpp

)

add_executable(Game main.cpp ${GAME_SRC})


target_link_libraries(Game PRIVATE quack-runtime-static)