#pragma once

#ifndef MERCILESS_MARIO_ENTITY_MANAGER_H
#define MERCILESS_MARIO_ENTITY_MANAGER_H


#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include "entity.h"

class EntityManager
{
    public:
        void update();
        std::shared_ptr<Entity>& addEntity(Entity::Type type);
        std::vector<std::shared_ptr<Entity>> getEntitiesByComponentTypes(const std::vector<Component::Type>& componentTypes);
        std::vector<std::shared_ptr<Entity>>& getEntitiesByType(Entity::Type type);
        void destroyAllEntities();

    private:
        static void removeDeadEntities(std::vector<std::shared_ptr<Entity>>& entities);

    private:
        size_t m_totalEntities = 0;
        std::vector<std::shared_ptr<Entity>> m_entities;
        std::vector<std::shared_ptr<Entity>> m_entitiesToAdd; // Holds enemies to add each frame, will be added to m_entities on next frame update
        std::unordered_map<Entity::Type, std::vector<std::shared_ptr<Entity>>> m_entitiesByType;

};


#endif //MERCILESS_MARIO_ENTITY_MANAGER_H
