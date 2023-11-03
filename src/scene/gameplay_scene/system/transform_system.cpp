#include "scene/gameplay_scene/system/transform_system.h"
#include "common_constants.h"

TransformSystem::TransformSystem(EntityManager& entityManager, sf::RenderWindow& window, sf::RenderTexture& renderTexture)
    : m_entityManager(entityManager), m_window(window), m_renderTexture(renderTexture)
{

}

void TransformSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> players = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::TRANSFORM, Component::Type::USER_INPUT});
    if (players.empty())
    {
        return;
    }

    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(player->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(player->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
        updatePlayerVelocityOnAction(cTransform, cAction, cMovement);
        updatePlayerArmPositionByMousePosition(player);
    }

    std::vector<std::shared_ptr<Entity>> collisionEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::TRANSFORM, Component::Type::COLLISION});
    for (std::shared_ptr<Entity>& entity : collisionEntities)
    {
        std::shared_ptr<CCollision> cCollision = std::static_pointer_cast<CCollision>(entity->getComponentByType(Component::Type::COLLISION));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(entity->getComponentByType(Component::Type::TRANSFORM));
        checkForWindowCollision(cTransform, cCollision);
    }

    std::vector<std::shared_ptr<Entity>> transformEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::TRANSFORM, Component::Type::SPRITE_GROUP, Component::Type::DYNAMIC_MOVEMENT});
    for (std::shared_ptr<Entity>& entity : transformEntities)
    {
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(entity->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(entity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
        std::shared_ptr<CSpriteGroup> spriteGroup = std::static_pointer_cast<CSpriteGroup>(
                entity->getComponentByType(Component::Type::SPRITE_GROUP));
        applyGravity(cTransform, cMovement, spriteGroup);
        updatePosition(cTransform);
        if (entity->hasComponent(Component::Type::DYNAMIC_MOVEMENT))
        {
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
        cTransform->m_position.x += cCollision->isCollidingLeft ? std::abs(cTransform->m_velocity.x*2*DT) : 0;
        cTransform->m_position.x -= cCollision->isCollidingRight ? std::abs(cTransform->m_velocity.x*2*DT) : 0;
        cTransform->m_position.y += cCollision->isCollidingUp ? std::abs(cTransform->m_velocity.y*2*DT) : 0;
        cTransform->m_position.y -= cCollision->isCollidingDown ? std::abs(cTransform->m_velocity.y*2*DT) : 0;
        cTransform->m_velocity = sf::Vector2f(
                cCollision->isCollidingLeft || cCollision->isCollidingRight ? 0 : cTransform->m_velocity.x,
                cCollision->isCollidingUp || cCollision->isCollidingDown ? 0 : cTransform->m_velocity.y);
    }
}

void TransformSystem::updatePosition(std::shared_ptr<CTransform>& cTransform)
{
    cTransform->m_position.x += cTransform->m_velocity.x * DT;
    cTransform->m_position.y += cTransform->m_velocity.y * DT;
}

void TransformSystem::applyGravity(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CMovement>& cMovement,
        std::shared_ptr<CSpriteGroup>& spriteGroup)
{
    if (cTransform->m_velocity.y >= 20)
    {
        cMovement->isAirborne = true;
    }

    if (cMovement->isRising && !cMovement->hasTouchedCeiling && cTransform->m_velocity.y > cMovement->maxJumpVelocity)
    {
        m_audioManger->playSound(AudioManager::AudioType::JUMP, 5.0f);
        cTransform->m_velocity.y -= cMovement->jumpAcceleration * DT;
        cMovement->isAirborne = true;

        spriteGroup->animations.at(0)->animationRectStartBounds = {160, 0, TILE_SIZE, TILE_SIZE};
        spriteGroup->animations.at(0)->animationRectBounds = {160, 0, TILE_SIZE, TILE_SIZE};
        spriteGroup->animations.at(0)->currentFrame = 0;
        spriteGroup->animations.at(0)->totalAnimationFrames = 1;
    }
    else
    {
        cMovement->isRising = false;
    }

    if (cTransform->m_velocity.y < cMovement->maxGravityAcceleration)
    {
        cTransform->m_velocity.y += cMovement->gravityRate * DT;
    }
}

void TransformSystem::updatePlayerVelocityOnAction(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CAction>& cAction,
        std::shared_ptr<CMovement>& cMovement)
{
    if (cAction->isMovingLeft && cTransform->m_velocity.x > -cMovement->maxMovementAcceleration)
    {
        cTransform->m_velocity.x -= cMovement->movementAcceleration * DT;
    }
    if (cAction->isMovingRight && (cTransform->m_velocity.x < cMovement->maxMovementAcceleration))
    {
        cTransform->m_velocity.x += cMovement->movementAcceleration * DT;
    }
    if (cAction->isJumping)
    {
        cAction->isJumping = false;
        cMovement->isRising = !cMovement->isAirborne;
    }
}

void TransformSystem::reduceVelocity(std::shared_ptr<CTransform>& cTransform, std::shared_ptr<CMovement>& cMovement)
{
    float movementDeceleration = std::abs(cMovement->movementDecelerationPerFrame) * DT;

    if (cTransform->m_velocity.x > movementDeceleration)
    {
        cTransform->m_velocity.x -= movementDeceleration;
    }
    else if (cTransform->m_velocity.x < -std::abs(cMovement->movementDecelerationPerFrame) * DT)
    {
        cTransform->m_velocity.x += movementDeceleration;
    }
    else
    {
        cTransform->m_velocity.x = 0;
    }
}

void TransformSystem::updatePlayerArmPositionByMousePosition(std::shared_ptr<Entity>& player)
{
    std::shared_ptr<CCursorFollower> cursorFollower = std::static_pointer_cast<CCursorFollower>(
            player->getComponentByType(Component::CURSOR_FOLLOWER));
    const sf::Vector2i& mousePos = sf::Mouse::getPosition(m_window);
    cursorFollower->armPointLocation = m_window.mapPixelToCoords(mousePos, m_renderTexture.getView());

    std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(player->getComponentByType(Component::Type::SPRITE_GROUP));
    std::shared_ptr<sf::Sprite>& arm = cSpriteGroup->sprites.at(1);
    float armRotationDegrees = cursorFollower->getArmPointAngleDegrees(arm->getPosition()) + SPRITE_TEXTURE_OFFSET_DEGREES;
    arm->setRotation(armRotationDegrees);
}
