#pragma once

#ifndef MERCILESS_MARIO_ANIMATION_SYSTEM_H
#define MERCILESS_MARIO_ANIMATION_SYSTEM_H

#include "system.h"
#include "entity_manager.h"
#include "c_sprite_group.h"
#include "common_constants.h"

class AnimationSystem : public System
{
    public:
        explicit AnimationSystem(EntityManager& entityManager);

        void execute() override;

    private:
        EntityManager& m_entityManager;

};


#endif //MERCILESS_MARIO_ANIMATION_SYSTEM_H
