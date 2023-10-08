#include "scene/gameplay_scene/system/collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::ranges::filter_view playersFiltered = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::SPRITE);
    });
    std::ranges::filter_view platformsFiltered = m_entityManager.getEntitiesByType(Entity::Type::PLATFORM) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::SPRITE);
    });

    std::vector<std::shared_ptr<Entity>> playersToUpdate = std::vector(playersFiltered.begin(), playersFiltered.end());
    std::vector<std::shared_ptr<Entity>> platformsToUpdate = std::vector(platformsFiltered.begin(), platformsFiltered.end());

    for (const std::shared_ptr<Entity>& player: playersToUpdate)
    {
        checkForWindowCollision(player);
        for (const std::shared_ptr<Entity>& platform: platformsToUpdate)
        {
            std::shared_ptr<CSprite> playerSprite = std::static_pointer_cast<CSprite>(player->getComponentByType(Component::Type::SPRITE));
            std::shared_ptr<CTransform> playerTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CSprite> platformSprite = std::static_pointer_cast<CSprite>(platform->getComponentByType(Component::Type::SPRITE));

            sf::FloatRect overlap;
            if (isCollidingAABB(playerSprite, platformSprite, overlap))
            {
                auto collisionNormal = platformSprite->m_shape.getPosition() - playerSprite->m_shape.getPosition();
                auto manifold = getManifold(overlap, collisionNormal);
                resolve(playerTransform, manifold);
            }
        }
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

bool CollisionSystem::isCollidingAABB(
        const std::shared_ptr<CSprite>& renderComponentForEntity,
        const std::shared_ptr<CSprite>& renderComponentForEnemy,
        sf::FloatRect& overlap)
{
    return renderComponentForEntity->m_shape.getGlobalBounds()
        .intersects(renderComponentForEnemy->m_shape.getGlobalBounds(), overlap);
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

void CollisionSystem::resolve(std::shared_ptr<CTransform>& cTransform, const sf::Vector3f& manifold)
{
    // move the shape out of the solid object by the penetration amount
    sf::Vector2f normal(manifold.x, manifold.y);
    const sf::Vector2<float>& offset = normal * manifold.z;
    cTransform->m_position += offset;
}