#include "scene/gameplay_scene/system/collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> dynamicEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP, Component::Type::TRANSFORM, Component::Type::COLLISION, Component::Type::DYNAMIC_MOVEMENT});
    std::vector<std::shared_ptr<Entity>> staticEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP, Component::Type::COLLISION, Component::Type::STATIC_MOVEMENT});
    if (dynamicEntities.empty() || staticEntities.empty())
    {
        return;
    }

    for (std::shared_ptr<Entity>& dynamicEntity : dynamicEntities)
    {
        std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(dynamicEntity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
        checkForWindowCollision(dynamicEntity, cMovement);

        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(dynamicEntity->getComponentByType(Component::Type::TRANSFORM));
        if (cTransform->m_position.y > LEVEL_DEPTH_KILL_ZONE)
        {
            dynamicEntity->destroy();
        }

        for (const std::shared_ptr<Entity>& staticEntity: staticEntities)
        {
            checkForEntityCollision(dynamicEntity, staticEntity);
        }

        for (const std::shared_ptr<Entity>& otherDynamicEntity: dynamicEntities)
        {
            if (dynamicEntity->getId() != otherDynamicEntity->getId())
            {
                checkForEntityCollision(dynamicEntity, otherDynamicEntity);
            }
        }
    }
}

void CollisionSystem::checkForEntityCollision(std::shared_ptr<Entity>& dynamicEntity, const std::shared_ptr<Entity>& staticEntity)
{
    std::shared_ptr<CSpriteGroup> staticEntitySpriteGroup = std::static_pointer_cast<CSpriteGroup>(staticEntity->getComponentByType(Component::Type::SPRITE_GROUP));
    std::shared_ptr<CSpriteGroup> dynamicEntitySpriteGroup = std::static_pointer_cast<CSpriteGroup>(dynamicEntity->getComponentByType(Component::Type::SPRITE_GROUP));

    sf::FloatRect overlap;
    if (isCollidingAABB(dynamicEntitySpriteGroup, staticEntitySpriteGroup, overlap))
    {
        auto collisionNormal = dynamicEntitySpriteGroup->animationSprites.at(0).getPosition() - staticEntitySpriteGroup->animationSprites.at(0).getPosition();
        auto manifold = getManifold(overlap, collisionNormal);
        resolve(dynamicEntity, manifold);
    }
}

void CollisionSystem::checkForWindowCollision(const std::shared_ptr<Entity>& e, std::shared_ptr<CMovement>& cMovement)
{
    std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> collisionComponentForEntity = std::static_pointer_cast<CCollision>(e->getComponentByType(Component::Type::COLLISION));
    if (collisionComponentForEntity != nullptr)
    {
        std::shared_ptr<CSpriteGroup> spriteGroup = std::static_pointer_cast<CSpriteGroup>(e->getComponentByType(Component::Type::SPRITE_GROUP));

        int spriteHalfWidth = spriteGroup->animationSprites.at(0).getTextureRect().width / 2;
        int spriteHalfHeight = spriteGroup->animationSprites.at(0).getTextureRect().height / 2;

        collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->m_position.x <= spriteHalfWidth;
        collisionComponentForEntity->isCollidingRight = transformComponentForEntity->m_position.x >= MAX_LEVEL_WIDTH -spriteHalfWidth;

        int maxLevelHeightCollisionPoint = MAX_LEVEL_HEIGHT - spriteHalfHeight;
        collisionComponentForEntity->isCollidingUp = transformComponentForEntity->m_position.y <= -maxLevelHeightCollisionPoint;
        collisionComponentForEntity->isCollidingDown = transformComponentForEntity->m_position.y >=
                maxLevelHeightCollisionPoint;

        if (collisionComponentForEntity->isCollidingUp)
        {
            cMovement->isRising = false;
        }
    }
}

bool CollisionSystem::isCollidingAABB(const std::shared_ptr<CSpriteGroup>& entitySpriteGroup,
        const std::shared_ptr<CSpriteGroup>& otherEntitySpriteGroup, sf::FloatRect& overlap)
{
    return entitySpriteGroup->animationSprites.at(0).getGlobalBounds().intersects(otherEntitySpriteGroup->animationSprites.at(0).getGlobalBounds(), overlap);
}

sf::Vector3f CollisionSystem::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
    // the collision normal is stored in x and y, with the penetration in z
    sf::Vector3f manifold;

    if (overlap.width < overlap.height)
    {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    }
    else
    {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}

void CollisionSystem::resolve(std::shared_ptr<Entity>& dynamicEntity, const sf::Vector3f& manifold)
{
    if (!dynamicEntity->hasComponents({Component::Type::TRANSFORM, Component::Type::DYNAMIC_MOVEMENT}))
    {
        return;
    }

    std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(dynamicEntity->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(dynamicEntity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));

    if (dynamicEntity->hasComponent(Component::Type::USER_INPUT))
    {
        if (manifold.y == 1 || manifold.y == -1)
        {
            std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                    dynamicEntity->getComponentByType(Component::Type::USER_INPUT));
            cAction->isJumping = false;
            cMovement->isAirborne = false;
            cMovement->hasTouchedCeiling = false;
            cTransform->m_velocity.y = 0;
        }
        if (manifold.x == 1 || manifold.x == -1)
        {
            cTransform->m_velocity.x += (cTransform->m_velocity.x > 0.0f
                    ? cTransform->m_velocity.x * -0.1f
                    : cTransform->m_velocity.x * 0.1f);
        }
    }

    updateVelocityOnCollision(manifold, cTransform, cMovement);

    // move the shape out of the solid object by the penetration amount
    sf::Vector2f normal(manifold.x, manifold.y);
    const sf::Vector2<float>& offset = normal * manifold.z;
    cTransform->m_position -= offset;
}

void CollisionSystem::updateVelocityOnCollision(const sf::Vector3f& manifold, std::shared_ptr<CTransform>& cTransform,
        std::shared_ptr<CMovement>& cMovement)
{

    // Reset velocity if hit collidable entity.
    bool isCollisionOverlapFromTop = manifold.y == -1;
    if (isCollisionOverlapFromTop && !cMovement->hasTouchedCeiling)
    {
        cTransform->m_velocity.y = 0;
        cMovement->hasTouchedCeiling = true;
    }
}
