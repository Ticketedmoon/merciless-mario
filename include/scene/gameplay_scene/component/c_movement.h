#pragma once

#ifndef MERCILESS_MARIO_C_MOVEMENT_H
#define MERCILESS_MARIO_C_MOVEMENT_H

#include "component.h"

class CMovement : public Component
{
    public:
        CMovement() = default;
        CMovement(float movementAcceleration, float movementDecelerationPerFrame, float maxMovementAcceleration,
                float jumpAcceleration, float maxJumpHeight,
                float gravityRate, float maxGravityAcceleration);

    public:
        float movementAcceleration{0.0f};
        float movementDecelerationPerFrame{0.0f};
        float maxMovementAcceleration{0.0f};
        float jumpAcceleration{0.0f};
        float maxJumpVelocity{0.0f};
        float gravityRate{0.0f};
        float maxGravityAcceleration{0.0f};

        bool isRising{false};
        bool isAirborne{false};
        bool hasTouchedCeiling{false};
};


#endif //MERCILESS_MARIO_C_MOVEMENT_H
