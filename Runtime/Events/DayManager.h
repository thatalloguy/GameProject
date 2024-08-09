//
// Created by allos on 8/9/2024.
//

#ifndef DUCKWATCHERS_DAYMANAGER_H
#define DUCKWATCHERS_DAYMANAGER_H

#include "../Core/TimeManager.h"
#include "Renderer/VkEngine.h"
#include "Objects/Entity.h"


class DayManager {
public:
    DayManager(TimeManager& timeManager, VulkanEngine& renderer);
    ~DayManager() = default;

    void update();

private:
    TimeManager& _timeManager;
    VulkanEngine& _renderer;

    std::vector<Quack::Entity> _eventEntities;
};


#endif //DUCKWATCHERS_DAYMANAGER_H
