#ifndef MERCILESS_MARIO_C_SPRITE_H
#define MERCILESS_MARIO_C_SPRITE_H

#include "component.h"

class CSprite : public Component
{
    public:
        explicit CSprite(sf::RectangleShape& shape);

    public:
        sf::RectangleShape m_shape;
};


#endif //MERCILESS_MARIO_C_SPRITE_H
