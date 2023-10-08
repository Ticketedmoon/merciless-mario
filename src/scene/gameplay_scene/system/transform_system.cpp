#include <iostream>
#include "scene/gameplay_scene/system/transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void TransformSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::USER_INPUT, Component::Type::TRANSFORM, Component::Type::COLLISION, Component::Type::GRAVITY});
    for (std::shared_ptr e : entities)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                e->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(
                e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CCollision> cCollision = std::static_pointer_cast<CCollision>(
                e->getComponentByType(Component::Type::COLLISION));
        std::shared_ptr<CGravity> cGravity = std::static_pointer_cast<CGravity>(
                e->getComponentByType(Component::Type::GRAVITY));

        checkForWindowCollision(cTransform, cCollision);
        updateVelocity(cAction, cTransform, cGravity);
        applyGravity(cTransform, cGravity);
        updatePosition(cTransform);
        reduceVelocity(cTransform);
    }
}
void TransformSystem::checkForWindowCollision(std::shared_ptr<CTransform>& cTransform,
        const std::shared_ptr<CCollision>& cCollision)
{
    if (cCollision->isCollidingLeft || cCollision->isCollidingRight || cCollision->isCollidingUp || cCollision->isCollidingDown)
    {
        // FIXME this isn't very good, is there a way we can get the manifold distance with the window sides?
        cTransform->m_position.x += cCollision->isCollidingLeft ? std::abs(cTransform->m_velocity.x*2) : 0;
        cTransform->m_position.x -= cCollision->isCollidingRight ? std::abs(cTransform->m_velocity.x*2) : 0;
        cTransform->m_position.y += cCollision->isCollidingUp ? std::abs(cTransform->m_velocity.y*2) : 0;
        cTransform->m_position.y -= cCollision->isCollidingDown ? std::abs(cTransform->m_velocity.y*2) : 0;
        cTransform->m_velocity = sf::Vector2f(
                cCollision->isCollidingLeft || cCollision->isCollidingRight ? 0 : cTransform->m_velocity.x,
                cCollision->isCollidingUp || cCollision->isCollidingDown ? 0 : cTransform->m_velocity.y);
    }
}

void TransformSystem::updatePosition(std::shared_ptr<CTransform>& cTransform)
{
    cTransform->m_position.x += cTransform->m_velocity.x;
    cTransform->m_position.y += cTransform->m_velocity.y;
}

void TransformSystem::applyGravity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CGravity>& cGravity)
{
    cTransform->m_velocity.y = cGravity->gravity;
}

void TransformSystem::updateVelocity(const std::shared_ptr<CAction>& cAction, std::shared_ptr<CTransform>& cTransform,
        std::shared_ptr<CGravity>& cGravity)
{
    if (cAction->isMovingLeft)
    {
        cTransform->m_velocity.x -= 0.05;
    }
    if (cAction->isMovingRight)
    {
        cTransform->m_velocity.x += 0.05;
    }
    if (cAction->isMovingUp)
    {
        cTransform->m_velocity.y -= 0.05;
    }
    if (cAction->isMovingDown)
    {
        cTransform->m_velocity.y += 0.05;
    }
    if (cAction->isJumping)
    {
        cTransform->m_position.y -= 100;
        cAction->isJumping = false;
    }
}
void TransformSystem::reduceVelocity(std::shared_ptr<CTransform>& cTransform)
{
    if (cTransform->m_velocity.x > 0)
    {
        cTransform->m_velocity.x -= cTransform->m_velocity.x < 0.025f ? cTransform->m_velocity.x : 0.025f;
    }
    if (cTransform->m_velocity.x < 0)
    {
        cTransform->m_velocity.x += cTransform->m_velocity.x > -0.025f ? cTransform->m_velocity.x : 0.025f;
    }

    if (cTransform->m_velocity.y > 0)
    {
        cTransform->m_velocity.y -= cTransform->m_velocity.y < 0.025f ? cTransform->m_velocity.y : 0.025f;
    }
    if (cTransform->m_velocity.y < 0)
    {
        cTransform->m_velocity.y += cTransform->m_velocity.y > -0.025f ? cTransform->m_velocity.y : 0.025f;
    }
}
