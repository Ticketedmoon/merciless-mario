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
            SPRITE_GROUP = 0,
            TRANSFORM = 1,
            COLLISION = 2,
            USER_INPUT = 3,
            DYNAMIC_MOVEMENT = 4,
            STATIC_MOVEMENT = 5,
            LIFESPAN = 6,
            WEAPON = 7,
            INTERACTABLE = 8,
            COUNT = 9
        };
};

#endif //MERCILESS_MARIO_COMPONENT_H
