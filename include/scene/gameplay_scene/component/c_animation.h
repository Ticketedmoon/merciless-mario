#ifndef MERCILESS_MARIO_C_ANIMATION_H
#define MERCILESS_MARIO_C_ANIMATION_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "component.h"

class CAnimation : public Component
{
    public:
        CAnimation() = default;

        sf::IntRect animationRectBounds;
        sf::Texture animationTexture;
        sf::Sprite animationSprite;
};


#endif //MERCILESS_MARIO_C_ANIMATION_H
