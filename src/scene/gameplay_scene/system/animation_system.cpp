#include "animation_system.h"
#include "c_movement.h"

AnimationSystem::AnimationSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void AnimationSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP});
    for (std::shared_ptr<Entity>& entity: entities)
    {
        std::shared_ptr<CSpriteGroup> spriteGroup = std::static_pointer_cast<CSpriteGroup>(
                entity->getComponentByType(Component::Type::SPRITE_GROUP));
        if (entity->hasComponent(Component::USER_INPUT))
        {
            std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                    entity->getComponentByType(Component::Type::USER_INPUT));
            std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(
                    entity->getComponentByType(Component::Type::DYNAMIC_MOVEMENT));
            if (cMovement->isRising)
            {
                tryUpdateSpriteAnimation(spriteGroup);
            }
            if (cAction->isMovingRight)
            {
                spriteGroup->animations.at(0)->animationRectStartBounds.top = 0;
                spriteGroup->animations.at(0)->animationRectBounds.top = 0;
                tryUpdateSpriteAnimation(spriteGroup);
            }
            else if (cAction->isMovingLeft)
            {
                spriteGroup->animations.at(0)->animationRectStartBounds.top = TILE_SIZE;
                spriteGroup->animations.at(0)->animationRectBounds.top = TILE_SIZE;
                tryUpdateSpriteAnimation(spriteGroup);
            }
        }
        else
        {
            tryUpdateSpriteAnimation(spriteGroup);
        }
    }
}

void AnimationSystem::tryUpdateSpriteAnimation(std::shared_ptr<CSpriteGroup>& spriteGroup)
{
    for (size_t spriteIndex = 0; spriteIndex < spriteGroup->sprites.size(); spriteIndex++)
    {
        std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimation = spriteGroup->animations.at(spriteIndex);
        float& spriteAnimationTime = spriteAnimation->animationTicker.timeBeforeAnimationUpdate;
        float spriteAnimationCompletionTime = spriteAnimation->animationTicker.animationUpdateTime;

        spriteAnimationTime += DT;

        if (spriteAnimationTime >= spriteAnimationCompletionTime)
        {
            std::shared_ptr<sf::Sprite>& sprite = spriteGroup->sprites.at(spriteIndex);
            resolveAnimation(sprite, spriteAnimation);
            spriteAnimationTime = 0;
        }
    }
}

void AnimationSystem::resolveAnimation(std::shared_ptr<sf::Sprite>& sprite,
        std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimation)
{
    if (spriteAnimation->currentFrame >= spriteAnimation->totalAnimationFrames - 1)
    {
        // Reset animation
        spriteAnimation->animationRectBounds = spriteAnimation->animationRectStartBounds;
        spriteAnimation->currentFrame = 0;
    }
    else
    {
        // Update animation frame
        spriteAnimation->currentFrame++;
        spriteAnimation->animationRectBounds.left = (spriteAnimation->animationIncrement * spriteAnimation->currentFrame);
    }

    sprite->setTextureRect(spriteAnimation->animationRectBounds);
}
