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
        for (size_t spriteIndex = 0; spriteIndex < spriteGroup->animationSprites.size(); spriteIndex++)
        {
            tryUpdateSpriteAnimation(spriteGroup, spriteIndex);
        }
    }
}

void AnimationSystem::tryUpdateSpriteAnimation(std::shared_ptr<CSpriteGroup>& spriteGroup, size_t spriteIndex) const
{
    float& spriteAnimationTime = spriteGroup->spriteAnimationTickerGroup.at(spriteIndex).currentTime;
    float spriteAnimationCompletionTime = spriteGroup->spriteAnimationTickerGroup.at(spriteIndex).completionTime;

    spriteAnimationTime += DT;

    if (spriteAnimationTime >= spriteAnimationCompletionTime)
    {
        sf::Sprite& currentSprite = spriteGroup->animationSprites.at(spriteIndex);
        unsigned int& spriteAnimationFrameNo = spriteGroup->currentFrameGroup.at(spriteIndex);
        unsigned int& spriteAnimationFramesTotal = spriteGroup->totalAnimationFramesGroup.at(spriteIndex);

        resolveAnimation(currentSprite, spriteAnimationFrameNo, spriteAnimationFramesTotal);
        spriteAnimationTime = 0;
    }
}

void AnimationSystem::resolveAnimation(sf::Sprite& currentSprite, unsigned int& spriteAnimationFrameNo,
        const unsigned int& spriteAnimationFramesTotal) const
{
    const sf::IntRect& spriteRectBounds = currentSprite.getTextureRect();
    if (spriteAnimationFrameNo >= spriteAnimationFramesTotal - 1)
    {
        // Reset animation
        currentSprite.setTextureRect({0, spriteRectBounds.top, spriteRectBounds.width, spriteRectBounds.height});
        spriteAnimationFrameNo = 0;
    }
    else
    {
        // Update animation frame
        spriteAnimationFrameNo += 1;
        int leftPos = TILE_SIZE * spriteAnimationFrameNo;
        currentSprite.setTextureRect({leftPos, spriteRectBounds.top, spriteRectBounds.width, spriteRectBounds.height});
    }
}
