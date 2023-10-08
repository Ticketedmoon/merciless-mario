#include "scene/gameplay_scene/system/transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void TransformSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::USER_INPUT, Component::Type::TRANSFORM, Component::Type::COLLISION});
    for (std::shared_ptr e : entities)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                e->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(
                e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CCollision> cCollision = std::static_pointer_cast<CCollision>(
                e->getComponentByType(Component::Type::COLLISION));

        if (!cCollision->isCollidingLeft && cAction->isMovingLeft)
        {
            cTransform->m_position.x -= (cTransform->m_velocity.x);
        }
        if (!cCollision->isCollidingRight && cAction->isMovingRight)
        {
            cTransform->m_position.x += (cTransform->m_velocity.x);
        }
        if (!cCollision->isCollidingUp && cAction->isMovingUp)
        {
            cTransform->m_position.y -= (cTransform->m_velocity.y);
        }
        if (!cCollision->isCollidingDown && cAction->isMovingDown)
        {
            cTransform->m_position.y += (cTransform->m_velocity.y);
        }
    }
}