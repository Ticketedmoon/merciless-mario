#pragma once

#ifndef MERCILESS_MARIO_C_SPRITE_GROUP_H
#define MERCILESS_MARIO_C_SPRITE_GROUP_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <SFML/System/Time.hpp>
#include "component.h"
#include "common_constants.h"

class CSpriteGroup : public Component
{
    public:
        struct SpriteAnimationTicker
        {
            float timeBeforeAnimationUpdate;
            float animationUpdateTime;
        };
        struct SpriteAnimation {
            const std::string textureSheetFilePath;
            // TODO make me const
            sf::IntRect animationRectStartBounds;
            sf::IntRect animationRectBounds;
            const uint16_t animationIncrement;

            uint16_t  currentFrame;
            uint16_t totalAnimationFrames;

            SpriteAnimationTicker animationTicker{};
            const sf::Vector2f origin;
            float rotation{};
        };

        CSpriteGroup() = default;

        std::vector<std::shared_ptr<SpriteAnimation>> animations;
        std::vector<std::shared_ptr<sf::Texture>> textures;
        std::vector<std::shared_ptr<sf::Sprite>> sprites;
};


#endif //MERCILESS_MARIO_C_SPRITE_GROUP_H
