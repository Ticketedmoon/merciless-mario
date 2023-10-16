#pragma once

#ifndef MERCILESS_MARIO_C_ACTION_H
#define MERCILESS_MARIO_C_ACTION_H

#include <cmath>
#include "component.h"

class CAction : public Component
{
    public:
        float getArmPointAngleRadians(sf::Vector2f originVec);
        float getArmPointAngleDegrees(sf::Vector2f originVec);

    public:
        bool isMovingLeft;
        bool isMovingRight;
        bool isLookingUp;
        bool isCrouching;
        bool isJumping;
        bool isShooting;

        // Should this be here?
        sf::Vector2f armPointLocation;
};

#endif //MERCILESS_MARIO_C_ACTION_H
