#include <iostream>
#include "render_system.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
}

void RenderSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entitiesToRender = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP, Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(e->getComponentByType(Component::Type::SPRITE_GROUP));

        for (std::shared_ptr<sf::Sprite>& sprite : cSpriteGroup->sprites)
        {
            sprite->setPosition(cTransform->m_position);
            m_renderTarget.draw(*sprite);
        }
    }
}