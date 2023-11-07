#pragma once

#ifndef MERCILESS_MARIO_TRANSFORM_SYSTEM_H
#define MERCILESS_MARIO_TRANSFORM_SYSTEM_H

#include <vector>
#include <memory>
#include <iostream>
#include <valarray>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "c_cursor_follower.h"
#include "c_sprite_group.h"
#include "entity/entity.h"
#include "entity/entity_manager.h"
#include "c_transform.h"
#include "c_collision.h"
#include "c_action.h"
#include "system.h"
#include "c_movement.h"
#include "audio_manager.h"

class TransformSystem : public System
{
    public:
        explicit TransformSystem(EntityManager& entityManager, sf::RenderWindow& window, sf::RenderTexture& renderTexture);

        void execute() override;

    private:
        void reduceVelocity(std::shared_ptr<CTransform>& cTransform, std::shared_ptr<CMovement>& cMovement);
        void updatePlayerVelocityOnAction(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CAction>& cAction,
                std::shared_ptr<CMovement>& cMovement);
        void updatePosition(std::shared_ptr<CTransform>& cTransform);
        void checkForWindowCollision(std::shared_ptr<CTransform>& cTransform,
                const std::shared_ptr<CCollision>& cCollision);
        void applyGravity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CMovement>& cMovement);
        void updatePlayerArmPositionByMousePosition(std::shared_ptr<Entity>& player);

    private:
        static constexpr int SPRITE_TEXTURE_OFFSET_DEGREES = 185;

        EntityManager& m_entityManager;
        AudioManager* m_audioManger = AudioManager::getInstance();
        sf::RenderWindow& m_window;
        sf::RenderTexture& m_renderTexture;
};


#endif //MERCILESS_MARIO_TRANSFORM_SYSTEM_H
