#pragma once

#ifndef MERCILESS_MARIO_ANIMATION_SYSTEM_H
#define MERCILESS_MARIO_ANIMATION_SYSTEM_H

#include "system.h"
#include "c_action.h"
#include "entity_manager.h"
#include "c_sprite_group.h"
#include "c_movement.h"
#include "c_transform.h"
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
        static void handlePlayerGroundAnimations(const std::shared_ptr<CAction>& cAction,
                std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimation);
        static void updateSpriteAnimationFrame(std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimationToUpdate,
                sf::IntRect rectBounds, uint32_t currentFrame, uint32_t totalAnimationFrames);

    private:
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_ANIMATION_SYSTEM_H
