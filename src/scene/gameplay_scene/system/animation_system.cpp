#include <iostream>
#include "animation_system.h"

AnimationSystem::AnimationSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void AnimationSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::SPRITE_GROUP});
    for (std::shared_ptr<Entity>& entity : entities)
    {
        std::shared_ptr<CSpriteGroup> spriteGroup = std::static_pointer_cast<CSpriteGroup>(entity->getComponentByType(Component::Type::SPRITE_GROUP));
        
        for (size_t i = 0; i < spriteGroup->animationSprites.size(); i++)
        {
            sf::IntRect rectBounds = spriteGroup->animationSprites.at(i).getTextureRect();
            if (spriteGroup->currentFrameGroup.at(i) >= spriteGroup->totalAnimationFramesGroup.at(i)-1)
            {
                // Reset
                spriteGroup->animationSprites.at(i).setTextureRect({0, rectBounds.top, rectBounds.width, rectBounds.height});
                spriteGroup->currentFrameGroup.at(i) = 0;
            }
            else
            {
                // Update animation frame
                spriteGroup->currentFrameGroup.at(i) += 1;
                int leftPos = TILE_SIZE * spriteGroup->currentFrameGroup.at(i);
                spriteGroup->animationSprites.at(i).setTextureRect({leftPos, rectBounds.top, rectBounds.width, rectBounds.height});
            }
        }
    }
}
