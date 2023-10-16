#pragma once

#ifndef MERCILESS_MARIO_LIFESPAN_SYSTEM_H
#define MERCILESS_MARIO_LIFESPAN_SYSTEM_H

#include <ranges>
#include <memory>

#include "entity/entity.h"
#include "system.h"
#include "entity/entity_manager.h"

#include "c_lifespan.h"
#include "c_sprite_group.h"

class LifespanSystem : public System
{
    public:
        explicit LifespanSystem(EntityManager& entityManager);
        void execute() override;

    private:
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_LIFESPAN_SYSTEM_H
