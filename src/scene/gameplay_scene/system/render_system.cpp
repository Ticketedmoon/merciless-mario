#include "render_system.h"
#include "c_transform.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
}

void RenderSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE, Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CSprite> cSprite = std::static_pointer_cast<CSprite>(e->getComponentByType(Component::Type::SPRITE));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
        cSprite->m_shape.setPosition(cTransform->m_position);
        m_renderTarget.draw(cSprite->m_shape);
    }
}