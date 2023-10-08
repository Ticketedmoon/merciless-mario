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
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_TRANSFORM_SYSTEM_H
