#pragma once

#ifndef MERCILESS_MARIO_SYSTEM_MANAGER_H
#define MERCILESS_MARIO_SYSTEM_MANAGER_H

#include <ranges>
#include <vector>
#include <memory>

#include "system.h"

class SystemManager
{
    public:
        enum class SystemType
        {
                UPDATE,
                RENDER
        };

        void update();
        void render();

        void registerSystem(const std::shared_ptr<System>& system, SystemType type);

    private:
        std::vector<std::shared_ptr<System>> m_systemsToUpdate;
        std::vector<std::shared_ptr<System>> m_systemsToRender;
};


#endif //MERCILESS_MARIO_SYSTEM_MANAGER_H
