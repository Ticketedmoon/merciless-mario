#pragma once

#ifndef MERCILESS_MARIO_C_TRANSFORM_H
#define MERCILESS_MARIO_C_TRANSFORM_H

#include <cstdint>

#include "component.h"

class CTransform : public Component
{
    public:
        CTransform(sf::Vector2f position, sf::Vector2f velocity);

    public:
        sf::Vector2f m_position;
        sf::Vector2f m_velocity;
};

#endif //MERCILESS_MARIO_C_TRANSFORM_H