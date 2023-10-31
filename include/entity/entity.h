#pragma once

#ifndef MERCILESS_MARIO_ENTITY_H
#define MERCILESS_MARIO_ENTITY_H

#include <ranges>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "component/component.h"

const uint8_t TOTAL_COMPONENTS = static_cast<uint8_t>(Component::Type::COUNT);

class Entity
{
    public:
        friend class EntityManager;

        void addComponent(Component::Type type, const std::shared_ptr<Component>& component);
        [[nodiscard]] bool hasComponent(Component::Type componentType) const;
        [[nodiscard]] bool hasComponents(const std::vector<Component::Type>& componentTypes) const;
        std::shared_ptr<Component> getComponentByType(Component::Type componentType);

    public:
        enum class Type
        {
            PLAYER,
            ENEMY,
            BULLET,
            PLATFORM,
            QUESTION_BLOCK,
            NONE
        };

        size_t getId() const;
        Type getType();
        [[nodiscard]] bool isAlive() const;
        void destroy();

    private:
        Entity(size_t id, Type type);

    private:
        size_t m_id;
        Type m_type;
        bool m_isAlive;

        std::array<std::shared_ptr<Component>, TOTAL_COMPONENTS> m_components;

};


#endif //MERCILESS_MARIO_ENTITY_H
