#pragma once

#ifndef MERCILESS_MARIO_C_SPRITE_GROUP_H
#define MERCILESS_MARIO_C_SPRITE_GROUP_H

#include "component.h"

class CSpriteGroup : public Component
{
    public:
        explicit CSpriteGroup(sf::RectangleShape& shape);
        explicit CSpriteGroup(std::vector<sf::RectangleShape>& shapes);

        sf::RectangleShape& getSprite();
        std::vector<sf::RectangleShape>& getSprites();

    private:
        std::vector<sf::RectangleShape> m_shapes;
};


#endif //MERCILESS_MARIO_C_SPRITE_GROUP_H
