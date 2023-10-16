#pragma once

#ifndef MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H
#define MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <memory>

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
        void createPlayer();
        void createPlatform(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor);
        void createBullet(sf::Vector2f playerPos, sf::Vector2f velocity) const;

    private:
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_ENTITY_SPAWN_SYSTEM_H
