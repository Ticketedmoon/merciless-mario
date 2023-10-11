#include <iostream>
#include "scene/gameplay_scene/system/transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void TransformSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> players = m_entityManager.getEntitiesByComponentTypes({Component::Type::TRANSFORM, Component::Type::USER_INPUT});
    if (players.empty())
    {
        return;
    }

    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(player->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(player->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
        updateVelocity(cTransform, cAction, cMovement);
        applyGravity(cTransform, cMovement);
    }

    std::vector<std::shared_ptr<Entity>> collisionEntities = m_entityManager.getEntitiesByComponentTypes({Component::Type::TRANSFORM, Component::Type::COLLISION});
    for (std::shared_ptr<Entity>& entity : collisionEntities)
    {
        std::shared_ptr<CCollision> cCollision = std::static_pointer_cast<CCollision>(entity->getComponentByType(Component::Type::COLLISION));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(entity->getComponentByType(Component::Type::TRANSFORM));
        checkForWindowCollision(cTransform, cCollision);
    }

    std::vector<std::shared_ptr<Entity>> transformEntities = m_entityManager.getEntitiesByComponentTypes({Component::Type::TRANSFORM});
    for (std::shared_ptr<Entity>& entity : transformEntities)
    {
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(entity->getComponentByType(Component::Type::TRANSFORM));
        updatePosition(cTransform);
        if (entity->hasComponent(Component::Type::DYNAMIC_MOVEMENT))
        {
            std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(entity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
            reduceVelocity(cTransform, cMovement);
        }
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

void TransformSystem::applyGravity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CMovement>& cMovement)
{
    if (cMovement->isRising && cTransform->m_velocity.y > cMovement->maxJumpHeight)
    {
        cTransform->m_velocity.y -= cMovement->jumpAcceleration;
        cMovement->isAirborne = true;
    }
    else
    {
        cMovement->isRising = false;
    }

    if (cTransform->m_velocity.y < cMovement->maxGravityAcceleration)
    {
        cTransform->m_velocity.y += cMovement->gravityRate;
    }
}

void TransformSystem::updateVelocity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CAction>& cAction,
        std::shared_ptr<CMovement>& cMovement)
{
    if (cAction->isMovingLeft && cTransform->m_velocity.x > -cMovement->maxMovementAcceleration)
    {
        cTransform->m_velocity.x -= cMovement->movementAcceleration;
    }
    if (cAction->isMovingRight && (cTransform->m_velocity.x < cMovement->maxMovementAcceleration))
    {
        cTransform->m_velocity.x += cMovement->movementAcceleration;
    }
    if (cAction->isJumping)
    {
        cMovement->isRising = !cMovement->isAirborne;
    }
}

void TransformSystem::reduceVelocity(std::shared_ptr<CTransform>& cTransform, std::shared_ptr<CMovement>& cMovement)
{
    cTransform->m_velocity.x -= cTransform->m_velocity.x > 0.0f
            ? std::abs(cMovement->movementDecelerationPerFrame)
            : 0;
    cTransform->m_velocity.x += cTransform->m_velocity.x < 0.0f
            ? std::abs(cMovement->movementDecelerationPerFrame)
            : 0;
}
