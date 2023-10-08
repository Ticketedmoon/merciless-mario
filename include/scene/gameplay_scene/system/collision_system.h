#pragma once

#ifndef MERCILESS_MARIO_COLLISION_SYSTEM_H
#define MERCILESS_MARIO_COLLISION_SYSTEM_H

#include <vector>
#include <memory>

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
                const std::shared_ptr<CSprite>& renderComponentForEnemy);

    private:
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_COLLISION_SYSTEM_H
