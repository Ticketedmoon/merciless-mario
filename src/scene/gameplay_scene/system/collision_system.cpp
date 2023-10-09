#include "scene/gameplay_scene/system/collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> dynamicEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE, Component::Type::COLLISION, Component::Type::DYNAMIC_MOVEMENT});
    std::vector<std::shared_ptr<Entity>> staticEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE, Component::Type::COLLISION, Component::Type::STATIC_MOVEMENT});
    if (dynamicEntities.empty() || staticEntities.empty())
    {
        return;
    }

    for (std::shared_ptr<Entity>& dynamicEntity : dynamicEntities)
    {
        std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(dynamicEntity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
        checkForWindowCollision(dynamicEntity, cMovement);

        for (const std::shared_ptr<Entity>& staticEntity: staticEntities)
        {
            checkForPlatformCollision(dynamicEntity, staticEntity);
        }
    }
}

void CollisionSystem::checkForPlatformCollision(std::shared_ptr<Entity>& dynamicEntity, const std::shared_ptr<Entity>& staticEntity)
{
    std::shared_ptr<CSprite> staticEntitySprite = std::static_pointer_cast<CSprite>(staticEntity->getComponentByType(Component::Type::SPRITE));
    std::shared_ptr<CSprite> dynamicEntitySprite = std::static_pointer_cast<CSprite>(dynamicEntity->getComponentByType(Component::Type::SPRITE));

    sf::FloatRect overlap;
    if (isCollidingAABB(dynamicEntitySprite, staticEntitySprite, overlap))
    {
        auto collisionNormal = staticEntitySprite->m_shape.getPosition() - dynamicEntitySprite->m_shape.getPosition();
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
        std::shared_ptr<CSprite> renderComponentForEntity = std::static_pointer_cast<CSprite>(e->getComponentByType(Component::Type::SPRITE));
        collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->m_position.x <= renderComponentForEntity->m_shape.getSize().x/2;
        collisionComponentForEntity->isCollidingRight = transformComponentForEntity->m_position.x >= MAX_LEVEL_WIDTH - renderComponentForEntity->m_shape.getSize().x/2;
        collisionComponentForEntity->isCollidingUp = transformComponentForEntity->m_position.y <= renderComponentForEntity->m_shape.getSize().y/2;
        collisionComponentForEntity->isCollidingDown = transformComponentForEntity->m_position.y >= MAX_LEVEL_WIDTH - renderComponentForEntity->m_shape.getSize().y/2;

        if (collisionComponentForEntity->isCollidingUp)
        {
            cMovement->isRising = false;
        }
    }
}

bool CollisionSystem::isCollidingAABB(const std::shared_ptr<CSprite>& playerSprite,
        const std::shared_ptr<CSprite>& platformSprite, sf::FloatRect& overlap)
{
    return playerSprite->m_shape.getGlobalBounds().intersects(platformSprite->m_shape.getGlobalBounds(), overlap);
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
    if (!dynamicEntity->hasComponent(Component::TRANSFORM))
    {
        return;
    }

    std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(dynamicEntity->getComponentByType(Component::Type::TRANSFORM));
    if (manifold.y < 0)
    {
        if (dynamicEntity->hasComponents({Component::Type::USER_INPUT, Component::Type::DYNAMIC_MOVEMENT}))
        {
            std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(dynamicEntity->getComponentByType(Component::Type::USER_INPUT));
            std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(dynamicEntity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
            cTransform->m_velocity.y = 0.0f;
            cAction->isJumping = false;
            cMovement->isAirborne = false;
        }
    }

    // move the shape out of the solid object by the penetration amount
    sf::Vector2f normal(manifold.x, manifold.y);
    const sf::Vector2<float>& offset = normal * manifold.z;
    cTransform->m_position += offset;
}