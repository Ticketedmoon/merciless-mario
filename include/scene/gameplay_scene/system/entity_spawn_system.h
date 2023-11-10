#pragma once

#ifndef MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H
#define MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <memory>
#include <fstream>
#include <sstream>

#include "entity.h"
#include "c_weapon.h"
#include "c_sprite_group.h"
#include "c_transform.h"
#include "c_interactable.h"
#include "c_collision.h"
#include "c_action.h"
#include "c_lifespan.h"
#include "c_movement.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "system.h"
#include "texture_manager.h"
#include "audio_manager.h"

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
        };

        struct EntityProperties
        {
            Entity::Type entityType;
            sf::Vector2f position;
            sf::Vector2f velocity;
            std::vector<CSpriteGroup::SpriteAnimation> animationProperties;
        };

        enum class LevelSpriteSizeType
        {
                SMALL,
                MEDIUM,
                LARGE
        };

        struct LevelSprite
        {
            std::string type;
            LevelSpriteSizeType sizeType;
            uint8_t sizeValueX;
            uint8_t sizeValueY;
        };

    private:
        void createLevel();
        void createEntity(const EntityProperties& entityProperties,
                std::unordered_map<Component::Type, std::shared_ptr<Component>>& componentGroup);
        void createLevelDecoration(const EntityProperties& entityProperties, const LevelSprite& levelSprite);
        void createLevelCollidableSprite(const EntityProperties& entityProperties, const LevelSprite& levelSprite);

        [[nodiscard]] static std::vector<Row> LoadLevelData(uint8_t levelNumber);

        // TODO Refactor this method
        void addAnimationTextureComponent(std::shared_ptr<CSpriteGroup>& spriteGroup,
                const std::string& animationTextureFilePath,
                const sf::Vector2f& position, sf::IntRect& rectBounds,
                sf::Vector2f origin, uint16_t totalAnimationFrames, uint16_t animationIncrement,
                float spriteAnimationCompletionTime, float rotation);
        static void removeLastAnimationTexture(std::shared_ptr<CSpriteGroup>& spriteGroup);

        std::shared_ptr<sf::Texture> buildSpriteTexture(std::shared_ptr<CSpriteGroup>& spriteGroup,
                const std::string& animationTextureFilePath);
        void applySpriteGroupForEntity(const sf::Vector2f& position, const LevelSprite& levelSprite,
                std::shared_ptr<Entity>& decoration);

    private:
        static constexpr float PLAYER_BULLET_SPEED = 75000.0f;

        EntityManager& m_entityManager;
        TextureManager m_textureManager;

        AudioManager* m_audioManager = AudioManager::getInstance();
};


#endif //MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H