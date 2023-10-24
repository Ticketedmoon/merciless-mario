#pragma once

#ifndef MERCILESS_MARIO_C_SPRITE_GROUP_H
#define MERCILESS_MARIO_C_SPRITE_GROUP_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <SFML/System/Time.hpp>
#include "component.h"

class CSpriteGroup : public Component
{
    public:
        struct AnimationTicker
        {
            float currentTime;
            float completionTime;
        };

        CSpriteGroup() = default;

        std::vector<sf::IntRect> animationRectBoundsGroup;
        std::vector<uint32_t> totalAnimationFramesGroup;
        std::vector<uint32_t> currentFrameGroup;
        std::vector<AnimationTicker> spriteAnimationTickerGroup;

        std::vector<std::shared_ptr<sf::Texture>> animationTextures;
        std::vector<sf::Sprite> animationSprites;
};


#endif //MERCILESS_MARIO_C_SPRITE_GROUP_H
