#pragma once

#ifndef MERCILESS_MARIO_TRANSFORM_SYSTEM_H
#define MERCILESS_MARIO_TRANSFORM_SYSTEM_H

#include <vector>
#include <memory>

#include "entity/entity.h"
#include "entity/entity_manager.h"
#include "c_transform.h"
#include "c_collision.h"
#include "c_action.h"
#include "system.h"

class TransformSystem : public System
{
    public:
        explicit TransformSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void reduceVelocity(std::shared_ptr<CTransform>& cTransform);
        static void updateVelocity(const std::shared_ptr<CAction>& cAction, std::shared_ptr<CTransform>& cTransform);
        static void updatePosition(std::shared_ptr<CTransform>& cTransform);
        static void checkForWindowCollision(std::shared_ptr<CTransform>& cTransform,
                const std::shared_ptr<CCollision>& cCollision);
        static void applyGravity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CAction>& cAction);

    private:
        // TODO Can these be part of a new component as these are specifically player properties.
        static constexpr float MAX_MOVEMENT_ACCELERATION = 0.025f;
        static constexpr float MOVEMENT_ACCELERATION = 0.1f;
        static constexpr float MAX_JUMP_HEIGHT = -6.0f;
        static constexpr float JUMP_ACCELERATION = 0.75f;
        static constexpr float MAX_GRAVITY_ACCELERATION = 3.0f;
        static constexpr float GRAVITY_RATE = 0.1f;

        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_TRANSFORM_SYSTEM_H
