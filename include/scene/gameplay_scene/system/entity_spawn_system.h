#pragma once

#ifndef MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H
#define MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <memory>
#include <fstream>
#include <sstream>

#include "entity.h"
#include "c_cursor_follower.h"
#include "c_sprite_group.h"
#include "c_transform.h"
#include "c_collision.h"
#include "c_action.h"
#include "c_lifespan.h"
#include "c_movement.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "system.h"
#include "texture_manager.h"

class EntitySpawnSystem : public System
{
    public:
        explicit EntitySpawnSystem(EntityManager& entityManager);
        void execute() override;

    private:
        struct Row
        {
            std::string entityType;
            std::string animation;
            uint16_t locationX;
            uint16_t locationY;
            bool isCollidable;
        };

        void createPlayer(sf::Vector2f position, bool isCollidable);
        void createPlatform(sf::Vector2f position, bool isCollidable);
        void createQuestionBlock(sf::Vector2f position, bool isCollidable);
        void createBullet(sf::Vector2f bulletPosition, sf::Vector2f velocity, float gunAngle);
        void createLevel();

        [[nodiscard]] static std::vector<Row> LoadLevelData(uint8_t levelNumber);

        // TODO Refactor this method
        void addAnimationTextureComponent(std::shared_ptr<CSpriteGroup>& spriteGroup,
                const sf::Vector2f& position, const std::string& animationTextureFilePath, sf::IntRect& rectBounds,
                sf::Vector2f origin, uint32_t totalAnimationFrames, uint32_t animationIncrement,
                float spriteAnimationCompletionTime, sf::Vector2f scale, float rotation);

        std::shared_ptr<sf::Texture> buildSpriteTexture(std::shared_ptr<CSpriteGroup>& spriteGroup,
                const std::string& animationTextureFilePath);

    private:
        static constexpr float PLAYER_BULLET_SPEED = 75000.0f;

        EntityManager& m_entityManager;
        TextureManager m_textureManager;
};


#endif //MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H