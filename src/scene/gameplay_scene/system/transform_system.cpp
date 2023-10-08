#include <iostream>
#include "scene/gameplay_scene/system/transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void TransformSystem::execute()
{
    std::shared_ptr<Entity>& player = m_entityManager.getEntityByType(Entity::Type::PLAYER);
    if (player == nullptr)
    {
        return;
    }

    std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(player->getComponentByType(Component::Type::USER_INPUT));
    std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> cCollision = std::static_pointer_cast<CCollision>(player->getComponentByType(Component::Type::COLLISION));

    checkForWindowCollision(cTransform, cCollision);
    updateVelocity(cAction, cTransform);
    applyGravity(cTransform, cAction);
    updatePosition(cTransform);
    reduceVelocity(cTransform);
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

void TransformSystem::applyGravity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CAction>& cAction)
{
    if (cAction->isRising && cTransform->m_velocity.y > MAX_JUMP_HEIGHT)
    {
        cTransform->m_velocity.y -= JUMP_ACCELERATION;
        cAction->isAirborne = true;
    }
    else
    {
        cAction->isRising = false;
    }

    if (cTransform->m_velocity.y < MAX_GRAVITY_ACCELERATION)
    {
        cTransform->m_velocity.y += GRAVITY_RATE;
    }
}

void TransformSystem::updateVelocity(const std::shared_ptr<CAction>& cAction, std::shared_ptr<CTransform>& cTransform)
{
    if (cAction->isMovingLeft)
    {
        cTransform->m_velocity.x -= MOVEMENT_ACCELERATION;
    }
    if (cAction->isMovingRight)
    {
        cTransform->m_velocity.x += MOVEMENT_ACCELERATION;
    }
    if (cAction->isMovingUp)
    {
        cTransform->m_velocity.y -= MOVEMENT_ACCELERATION;
    }
    if (cAction->isMovingDown)
    {
        cTransform->m_velocity.y += MOVEMENT_ACCELERATION;
    }
    if (cAction->isJumping)
    {
        cAction->isRising = !cAction->isAirborne;
    }
}
void TransformSystem::reduceVelocity(std::shared_ptr<CTransform>& cTransform)
{
    if (cTransform->m_velocity.x > 0)
    {
        cTransform->m_velocity.x -= cTransform->m_velocity.x < MAX_MOVEMENT_ACCELERATION
                ? cTransform->m_velocity.x : MAX_MOVEMENT_ACCELERATION;
    }
    if (cTransform->m_velocity.x < 0)
    {
        cTransform->m_velocity.x += cTransform->m_velocity.x > -MAX_MOVEMENT_ACCELERATION
                ? cTransform->m_velocity.x : MAX_MOVEMENT_ACCELERATION;
    }

    if (cTransform->m_velocity.y > 0)
    {
        cTransform->m_velocity.y -= cTransform->m_velocity.y < MAX_MOVEMENT_ACCELERATION
                ? cTransform->m_velocity.y : MAX_MOVEMENT_ACCELERATION;
    }
    if (cTransform->m_velocity.y < 0)
    {
        cTransform->m_velocity.y += cTransform->m_velocity.y > -MAX_MOVEMENT_ACCELERATION
                ? cTransform->m_velocity.y : MAX_MOVEMENT_ACCELERATION;
    }
}
