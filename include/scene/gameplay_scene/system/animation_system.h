#pragma once

#ifndef MERCILESS_MARIO_ANIMATION_SYSTEM_H
#define MERCILESS_MARIO_ANIMATION_SYSTEM_H

#include "system.h"
#include "c_action.h"
#include "entity_manager.h"
#include "c_sprite_group.h"
#include "common_constants.h"

class AnimationSystem : public System
{
    public:
        explicit AnimationSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void tryUpdateSpriteAnimation(std::shared_ptr<CSpriteGroup>& spriteGroup);
        static void resolveAnimation(std::shared_ptr<sf::Sprite>& sprite,
                std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimation);

    private:
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_ANIMATION_SYSTEM_H
