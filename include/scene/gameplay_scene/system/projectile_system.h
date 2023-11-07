#pragma once

#ifndef MERCILESS_MARIO_PROJECTILE_SYSTEM_H
#define MERCILESS_MARIO_PROJECTILE_SYSTEM_H

#include "system.h"
#include "entity_manager.h"
#include "audio_manager.h"
#include <iostream>
#include "c_action.h"
#include "c_weapon.h"
#include "c_transform.h"
#include "c_sprite_group.h"
#include "common_constants.h"

class ProjectileSystem : public System
{
    public:
        ProjectileSystem(EntityManager& entityManager);

        void execute() override;

    private:
        EntityManager& m_entityManager;
        AudioManager* m_audioManager = AudioManager::getInstance();
};


#endif //MERCILESS_MARIO_PROJECTILE_SYSTEM_H
