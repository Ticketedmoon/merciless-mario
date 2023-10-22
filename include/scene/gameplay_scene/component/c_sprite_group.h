#pragma once

#ifndef MERCILESS_MARIO_C_SPRITE_GROUP_H
#define MERCILESS_MARIO_C_SPRITE_GROUP_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "component.h"

class CSpriteGroup : public Component
{
    public:
        CSpriteGroup() = default;
        explicit CSpriteGroup(sf::RectangleShape& shape);
        explicit CSpriteGroup(std::vector<sf::RectangleShape>& shapes);

        sf::RectangleShape& getSprite();
        std::vector<sf::RectangleShape>& getSprites();

    public:
        sf::IntRect animationRectBounds;
        sf::Texture animationTexture;
        sf::Sprite animationSprite;

    private:
        std::vector<sf::RectangleShape> m_shapes;
};


#endif //MERCILESS_MARIO_C_SPRITE_GROUP_H
