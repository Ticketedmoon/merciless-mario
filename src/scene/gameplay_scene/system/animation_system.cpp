#include "animation_system.h"

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
            handlePlayerAnimation(entity, spriteGroup);
        }

        tryUpdateSpriteAnimation(spriteGroup);
    }
}

void AnimationSystem::handlePlayerAnimation(std::shared_ptr<Entity>& entity, std::shared_ptr<CSpriteGroup>& spriteGroup)
{
    std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
            entity->getComponentByType(Component::USER_INPUT));
    std::shared_ptr<CMovement> cMovement = std::static_pointer_cast<CMovement>(
            entity->getComponentByType(Component::DYNAMIC_MOVEMENT));

    std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimation = spriteGroup->animations.at(0);

    if (cMovement->isRising)
    {
        updateSpriteAnimationFrame(spriteAnimation, {160, spriteAnimation->animationRectStartBounds.top, TILE_SIZE, TILE_SIZE}, 0, 1);
    }
    else if (!cMovement->isAirborne)
    {
        handlePlayerGroundAnimations(cAction, spriteAnimation);
    }
}

void AnimationSystem::handlePlayerGroundAnimations(const std::shared_ptr<CAction>& cAction,
        std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimation)
{
    int top = spriteAnimation->animationRectStartBounds.top;
    updateSpriteAnimationFrame(spriteAnimation, {0, top, TILE_SIZE, TILE_SIZE}, spriteAnimation->currentFrame, 4);

    if (cAction->isMovingRight)
    {
        sf::IntRect rightAnimBounds = {spriteAnimation->animationRectStartBounds.left, 0, TILE_SIZE, TILE_SIZE};
        updateSpriteAnimationFrame(spriteAnimation, rightAnimBounds, spriteAnimation->currentFrame, spriteAnimation->totalAnimationFrames);
    }
    else if (cAction->isMovingLeft)
    {
        sf::IntRect leftAnimBounds = {spriteAnimation->animationRectStartBounds.left, TILE_SIZE, TILE_SIZE, TILE_SIZE};
        updateSpriteAnimationFrame(spriteAnimation, leftAnimBounds, spriteAnimation->currentFrame, spriteAnimation->totalAnimationFrames);
    }
    else
    {
        // Idle
        sf::IntRect idleAnimBounds = {spriteAnimation->animationRectStartBounds.left, spriteAnimation->animationRectStartBounds.top, TILE_SIZE, TILE_SIZE};
        updateSpriteAnimationFrame(spriteAnimation, idleAnimBounds, 0, 1);
    }
}

void AnimationSystem::updateSpriteAnimationFrame(std::shared_ptr<CSpriteGroup::SpriteAnimation>& spriteAnimationToUpdate,
        sf::IntRect rectBounds, uint32_t currentFrame, uint32_t totalAnimationFrames)
{
    spriteAnimationToUpdate->animationRectStartBounds = rectBounds;
    spriteAnimationToUpdate->animationRectBounds = rectBounds;
    spriteAnimationToUpdate->currentFrame = currentFrame;
    spriteAnimationToUpdate->totalAnimationFrames = totalAnimationFrames;
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
