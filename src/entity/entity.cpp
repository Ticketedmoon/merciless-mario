#include "entity/entity.h"

Entity::Entity(size_t id, Entity::Type type) : m_id(id), m_type(type), m_isAlive(true)
{
}

void Entity::addComponent(Component::Type type, const std::shared_ptr<Component>& component)
{
    m_components[type] = component;
}

bool Entity::hasComponent(const Component::Type componentType) const
{
    auto index = static_cast<uint8_t>(componentType);
    return m_components[index] != nullptr;
}

void Entity::removeComponent(const Component::Type componentType)
{
    auto index = static_cast<uint8_t>(componentType);
    m_components[index] = nullptr;
}

bool Entity::hasComponents(const std::vector<Component::Type>& componentTypes) const
{
    return std::ranges::all_of(componentTypes.cbegin(), componentTypes.cend(),
            [this](const Component::Type componentType) {
                int index = static_cast<int>(componentType);
                return m_components[index] != nullptr;
            });
}

std::shared_ptr<Component> Entity::getComponentByType(const Component::Type componentType)
{
    auto index = static_cast<uint8_t>(componentType);
    return m_components[index];
}

size_t Entity::getId() const
{
    return m_id;
}

Entity::Type Entity::getType()
{
    return m_type;
}

bool Entity::isAlive() const
{
    return m_isAlive;
}

void Entity::destroy()
{
    m_isAlive = false;
}