#include "render_system.h"
#include "c_transform.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
}

void RenderSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP, Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(e->getComponentByType(Component::Type::SPRITE_GROUP));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
        for (sf::RectangleShape& sprite : cSpriteGroup->getSprites())
        {
            sprite.setPosition(cTransform->m_position);
            m_renderTarget.draw(sprite);
        }
    }
}