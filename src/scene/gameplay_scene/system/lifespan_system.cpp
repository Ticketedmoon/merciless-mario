#include "scene/gameplay_scene/system/lifespan_system.h"

LifespanSystem::LifespanSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LifespanSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::LIFESPAN, Component::Type::SPRITE_GROUP});

    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CLifespan> lifeSpanComponent = std::static_pointer_cast<CLifespan> (e->getComponentByType(Component::Type::LIFESPAN));
        if (!lifeSpanComponent->isAlive())
        {
            e->destroy();
        }
        else
        {
            std::shared_ptr<CSpriteGroup> spriteGroup = std::static_pointer_cast<CSpriteGroup> (e->getComponentByType(Component::Type::SPRITE_GROUP));
            std::shared_ptr<sf::Sprite>& sprite = spriteGroup->sprites.at(0);
            sf::Color color = sprite->getColor();
            color.a -= color.a >= 1 ? 1 : 0;

            sprite->setColor(color);
            lifeSpanComponent->decreaseTimeToLive();
        }
    }
}
