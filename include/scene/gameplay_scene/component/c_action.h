#pragma once

#ifndef MERCILESS_MARIO_C_ACTION_H
#define MERCILESS_MARIO_C_ACTION_H

#include "component.h"

class CAction : public Component
{
    public:
        bool isMovingLeft;
        bool isMovingRight;
        bool isMovingUp;
        bool isMovingDown;
        bool isJumping;
        bool isRising;
        bool isAirborne;
};


#endif //MERCILESS_MARIO_C_ACTION_H
