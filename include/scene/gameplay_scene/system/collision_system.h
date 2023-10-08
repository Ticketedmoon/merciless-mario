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
#include "c_transform.h"
#include "c_collision.h"
#include "c_sprite.h"

class CollisionSystem : public System
{
    public:
        explicit CollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:

        static void checkForWindowCollision(const std::shared_ptr<Entity>& e);
        static bool isCollidingAABB(const std::shared_ptr<CSprite>& renderComponentForEntity,
                const std::shared_ptr<CSprite>& renderComponentForEnemy,
                sf::FloatRect& overlap);
        static sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);
        static void resolve(std::shared_ptr<CTransform>& cTransform, const sf::Vector3f& manifold);

    private:
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_COLLISION_SYSTEM_H
