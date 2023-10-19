#pragma once

#ifndef MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H
#define MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <memory>
#include <fstream>
#include <sstream>

#include "entity.h"
#include "c_sprite_group.h"
#include "c_transform.h"
#include "c_collision.h"
#include "c_action.h"
#include "c_lifespan.h"
#include "c_movement.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "system.h"

class EntitySpawnSystem : public System
{
    public:
        explicit EntitySpawnSystem(EntityManager& entityManager);
        void execute() override;

    private:
        struct Row
        {
            std::string entityType;
            uint16_t locationX;
            uint16_t locationY;
            bool isCollidable;
        };

        void createPlayer(sf::Vector2f size, sf::Vector2f position, bool isCollidable);
        void createPlatform(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor, bool isCollidable);
        void createBullet(sf::Vector2f bulletPosition, sf::Vector2f velocity);

        void createLevel();
        [[nodiscard]] static std::vector<Row> LoadLevelData(uint8_t levelNumber);

    private:
        const sf::Vector2f ENTITY_SIZE{32, 32};

        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H