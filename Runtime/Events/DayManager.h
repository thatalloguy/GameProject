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
    ~DayManager();

    void update();

private:
    TimeManager& _timeManager;
    VulkanEngine& _renderer;

    std::vector<Quack::Entity*> _eventEntities;
    /*
     * 0 = brief case
     * 1 = EVIL moon
     */
};


#endif //DUCKWATCHERS_DAYMANAGER_H
