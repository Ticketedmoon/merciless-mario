#pragma once

#ifndef MERCILESS_MARIO_COMPONENT_H
#define MERCILESS_MARIO_COMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <utility>

class Component
{
    public:
        enum Type
        {
            SPRITE = 0,
            TRANSFORM = 1,
            COLLISION = 2,
            USER_INPUT = 3,
            COUNT = 4
        };
};

#endif //MERCILESS_MARIO_COMPONENT_H
