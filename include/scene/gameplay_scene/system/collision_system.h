#pragma once

#ifndef MERCILESS_MARIO_COLLISION_SYSTEM_H
#define MERCILESS_MARIO_COLLISION_SYSTEM_H

#include <vector>
#include <memory>

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "common_constants.h"

#include "system.h"

#include "entity/entity_manager.h"
#include "entity/entity.h"

#include "c_interactable.h"
#include "c_lifespan.h"
#include "c_weapon.h"
#include "c_transform.h"
#include "c_collision.h"
#include "c_sprite_group.h"
#include "c_action.h"
#include "c_movement.h"

#include "audio_manager.h"

class CollisionSystem : public System
{
    public:
        explicit CollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:

        static void checkForWindowCollision(const std::shared_ptr<Entity>& e, std::shared_ptr<CMovement>& cMovement);
        void checkForEntityCollision(std::shared_ptr<Entity>& dynamicEntity, std::shared_ptr<Entity>& otherEntity);
        bool isCollidingAABB(const std::shared_ptr<CSpriteGroup>& entitySpriteGroup,
                const std::shared_ptr<CSpriteGroup>& otherEntitySpriteGroup,
                sf::FloatRect& overlap);

        sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);
        void resolve(std::shared_ptr<Entity>& entity, std::shared_ptr<Entity>& otherEntity,
                const sf::Vector3f& manifold);

    private:
        EntityManager& m_entityManager;

        AudioManager* m_audioManager = AudioManager::getInstance();
};


#endif //MERCILESS_MARIO_COLLISION_SYSTEM_H
