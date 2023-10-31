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
        struct SpriteAnimationTicker
        {
            float timeBeforeAnimationUpdate;
            float animationUpdateTime;
        };
        struct SpriteAnimation {
            sf::IntRect animationRectStartBounds;
            sf::IntRect animationRectBounds;
            uint32_t animationIncrement{};

            uint32_t totalAnimationFrames{};
            uint32_t currentFrame{};
            SpriteAnimationTicker animationTicker{};
        };

        CSpriteGroup() = default;

        std::vector<std::shared_ptr<SpriteAnimation>> animations;
        std::vector<std::shared_ptr<sf::Texture>> textures;
        std::vector<std::shared_ptr<sf::Sprite>> sprites;
};


#endif //MERCILESS_MARIO_C_SPRITE_GROUP_H
