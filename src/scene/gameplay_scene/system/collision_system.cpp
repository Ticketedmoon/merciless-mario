#include "scene/gameplay_scene/system/collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::shared_ptr<Entity>& player = m_entityManager.getEntityByType(Entity::Type::PLAYER);
    if (player == nullptr)
    {
        return;
    }

    checkForWindowCollision(player);

    std::ranges::filter_view platformsFiltered = m_entityManager.getEntitiesByType(Entity::Type::PLATFORM) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::SPRITE);
    });
    std::vector<std::shared_ptr<Entity>> platformsToUpdate = std::vector(platformsFiltered.begin(), platformsFiltered.end());

    std::shared_ptr<CSprite> playerSprite = std::static_pointer_cast<CSprite>(player->getComponentByType(Component::Type::SPRITE));
    std::shared_ptr<CAction> playerAction = std::static_pointer_cast<CAction>(player->getComponentByType(Component::Type::USER_INPUT));
    std::shared_ptr<CTransform> playerTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));

    for (const std::shared_ptr<Entity>& platform: platformsToUpdate)
    {
        std::shared_ptr<CSprite> platformSprite = std::static_pointer_cast<CSprite>(platform->getComponentByType(Component::Type::SPRITE));
        checkForPlatformCollision(playerSprite, playerTransform, playerAction, platformSprite);
    }
}

void CollisionSystem::checkForPlatformCollision(const std::shared_ptr<CSprite>& playerSprite,
        std::shared_ptr<CTransform>& playerTransform, const std::shared_ptr<CAction>& playerAction,
        const std::shared_ptr<CSprite>& platformSprite)
{
    sf::FloatRect overlap;
    if (isCollidingAABB(playerSprite, platformSprite, overlap))
    {
        auto collisionNormal = platformSprite->m_shape.getPosition() - playerSprite->m_shape.getPosition();
        auto manifold = getManifold(overlap, collisionNormal);
        resolve(playerTransform, playerAction, manifold);
    }
}

void CollisionSystem::checkForWindowCollision(const std::shared_ptr<Entity>& e)
{
    std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> collisionComponentForEntity = std::static_pointer_cast<CCollision>(e->getComponentByType(Component::Type::COLLISION));
    if (collisionComponentForEntity != nullptr)
    {
        std::shared_ptr<CSprite> renderComponentForEntity = std::static_pointer_cast<CSprite>(e->getComponentByType(Component::Type::SPRITE));
        collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->m_position.x <= renderComponentForEntity->m_shape.getSize().x/2;
        collisionComponentForEntity->isCollidingRight = transformComponentForEntity->m_position.x >= WINDOW_WIDTH - renderComponentForEntity->m_shape.getSize().x/2;
        collisionComponentForEntity->isCollidingUp = transformComponentForEntity->m_position.y <= renderComponentForEntity->m_shape.getSize().y/2;
        collisionComponentForEntity->isCollidingDown = transformComponentForEntity->m_position.y >= WINDOW_HEIGHT - renderComponentForEntity->m_shape.getSize().y/2;
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

void CollisionSystem::resolve(std::shared_ptr<CTransform>& cTransform, const std::shared_ptr<CAction>& cAction, const sf::Vector3f& manifold)
{
    if (manifold.y < 0)
    {
        cTransform->m_velocity.y = 0.0f;
        cAction->isJumping = false;
        cAction->isAirborne = false;
    }

    // move the shape out of the solid object by the penetration amount
    sf::Vector2f normal(manifold.x, manifold.y);
    const sf::Vector2<float>& offset = normal * manifold.z;
    cTransform->m_position += offset;
}