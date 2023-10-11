#pragma once

#ifndef MERCILESS_MARIO_C_ACTION_H
#define MERCILESS_MARIO_C_ACTION_H

#include "component.h"

class CAction : public Component
{
    public:
        bool isMovingLeft;
        bool isMovingRight;
        bool isLookingUp;
        bool isCrouching;
        bool isJumping;

        // Should this be here?
        sf::Vector2f armPointLocation;
};


#endif //MERCILESS_MARIO_C_ACTION_H
