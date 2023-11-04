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

        for (std::shared_ptr<Entity>& staticEntity: staticEntities)
        {
            checkForEntityCollision(dynamicEntity, staticEntity);
        }

        for (std::shared_ptr<Entity>& otherDynamicEntity: dynamicEntities)
        {
            if (dynamicEntity->getId() != otherDynamicEntity->getId())
            {
                checkForEntityCollision(dynamicEntity, otherDynamicEntity);
            }
        }
    }
}

void CollisionSystem::checkForEntityCollision(std::shared_ptr<Entity>& dynamicEntity,std::shared_ptr<Entity>& otherEntity)
{
    std::shared_ptr<CSpriteGroup> staticEntitySpriteGroup = std::static_pointer_cast<CSpriteGroup>(
            otherEntity->getComponentByType(Component::Type::SPRITE_GROUP));
    std::shared_ptr<CSpriteGroup> dynamicEntitySpriteGroup = std::static_pointer_cast<CSpriteGroup>(
            dynamicEntity->getComponentByType(Component::Type::SPRITE_GROUP));

    sf::FloatRect overlap;
    if (isCollidingAABB(dynamicEntitySpriteGroup, staticEntitySpriteGroup, overlap))
    {
        if (dynamicEntity->getType() == Entity::Type::BULLET)
        {
            dynamicEntity->destroy();
            otherEntity->destroy();
        }

        auto collisionNormal = dynamicEntitySpriteGroup->sprites.at(0)->getPosition() -
                staticEntitySpriteGroup->sprites.at(0)->getPosition();
        auto manifold = getManifold(overlap, collisionNormal);
        resolve(dynamicEntity, otherEntity, manifold);
    }
}

void CollisionSystem::checkForWindowCollision(const std::shared_ptr<Entity>& e, std::shared_ptr<CMovement>& cMovement)
{
    std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> collisionComponentForEntity = std::static_pointer_cast<CCollision>(e->getComponentByType(Component::Type::COLLISION));
    if (collisionComponentForEntity != nullptr)
    {
        std::shared_ptr<CSpriteGroup> spriteGroup = std::static_pointer_cast<CSpriteGroup>(e->getComponentByType(Component::Type::SPRITE_GROUP));

        int spriteHalfWidth = spriteGroup->sprites.at(0)->getTextureRect().width / 2;
        int spriteHalfHeight = spriteGroup->sprites.at(0)->getTextureRect().height / 2;

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
    std::shared_ptr<sf::Sprite>& spriteA = entitySpriteGroup->sprites.at(0);
    std::shared_ptr<sf::Sprite>& spriteB = otherEntitySpriteGroup->sprites.at(0);
    return spriteA->getGlobalBounds().intersects(spriteB->getGlobalBounds(), overlap);
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

void CollisionSystem::resolve(std::shared_ptr<Entity>& entity, std::shared_ptr<Entity>& otherEntity,
        const sf::Vector3f& manifold)
{
    if (!entity->hasComponents({Component::Type::TRANSFORM, Component::Type::DYNAMIC_MOVEMENT}))
    {
        return;
    }

    std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(entity->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(entity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));

    if (entity->hasComponent(Component::Type::USER_INPUT))
    {
        // Ground
        if (manifold.y == 1)
        {
            std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                    entity->getComponentByType(Component::Type::USER_INPUT));
            cMovement->isAirborne = false;
            cMovement->hasTouchedCeiling = false;
            cTransform->m_velocity.y = 0;
        }

        // Ceiling
        if (manifold.y == -1 && !cMovement->hasTouchedCeiling)
        {
            cTransform->m_velocity.y = 0;
            cMovement->hasTouchedCeiling = true;


            if (otherEntity->getType() == Entity::Type::BRICK)
            {
                m_audioManager->playSound(AudioManager::AudioType::BREAK_BRICK, DEFAULT_SFX_VOLUME);
                otherEntity->destroy();
            }

            // If question-block, turn off and get coin
            if (otherEntity->getType() == Entity::Type::QUESTION_BLOCK)
            {
                std::shared_ptr<CInteractable> otherEntityInteractableState = std::static_pointer_cast<CInteractable>(
                        otherEntity->getComponentByType(Component::Type::INTERACTABLE));
                if (otherEntityInteractableState->isInteractable())
                {
                    m_audioManager->playSound(AudioManager::AudioType::POWER_UP_APPEARS, DEFAULT_SFX_VOLUME);

                    std::shared_ptr<CSpriteGroup> otherEntitySpriteGroup = std::static_pointer_cast<CSpriteGroup>(
                            otherEntity->getComponentByType(Component::Type::SPRITE_GROUP));
                    otherEntitySpriteGroup->animations.at(0)->animationRectStartBounds = sf::IntRect(96, 0, TILE_SIZE, TILE_SIZE);
                    otherEntitySpriteGroup->animations.at(0)->animationRectBounds = sf::IntRect(96, 0, TILE_SIZE, TILE_SIZE);
                    otherEntitySpriteGroup->animations.at(0)->currentFrame = 0;
                    otherEntitySpriteGroup->animations.at(0)->totalAnimationFrames = 1;

                    otherEntityInteractableState->deactivate();
                }
                else
                {
                    m_audioManager->playSound(AudioManager::AudioType::BUMP, DEFAULT_SFX_VOLUME);
                }
            }
        }

        // Left/Right Wall
        if (manifold.x == 1 || manifold.x == -1)
        {
            if (!cMovement->hasTouchedWall)
            {
                if (cMovement->isAirborne)
                {
                    cTransform->m_velocity.x = 0;
                }
                cMovement->hasTouchedWall = true;
            }
            cTransform->m_velocity.x += (cTransform->m_velocity.x > 0.0f
                    ? cTransform->m_velocity.x * -0.1f
                    : cTransform->m_velocity.x * 0.1f) * DT;
        }
        else
        {
            cMovement->hasTouchedWall = false;
        }
    }

    // move the shape out of the solid object by the penetration amount
    sf::Vector2f normal(manifold.x, manifold.y);
    const sf::Vector2<float>& offset = normal * manifold.z;
    cTransform->m_position -= offset;
}