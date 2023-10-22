#include <iostream>
#include "render_system.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
}

void RenderSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> animationEntitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::ANIMATION, Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : animationEntitiesToUpdate)
    {
        std::shared_ptr<CAnimation> cAnimation = std::static_pointer_cast<CAnimation>(e->getComponentByType(Component::Type::ANIMATION));
        m_renderTarget.draw(cAnimation->animationSprite);
    }

    std::vector<std::shared_ptr<Entity>> shapeEntitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP, Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : shapeEntitiesToUpdate)
    {
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));

        std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(e->getComponentByType(Component::Type::SPRITE_GROUP));
        for (sf::RectangleShape& sprite : cSpriteGroup->getSprites())
        {
            sprite.setPosition(cTransform->m_position);
            m_renderTarget.draw(sprite);
        }
    }
}