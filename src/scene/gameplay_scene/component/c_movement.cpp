#include "c_movement.h"

CMovement::CMovement(float movementAcceleration, float maxMovementAcceleration, float jumpAcceleration,
        float maxJumpHeight, float gravityRate, float maxGravityAcceleration)
        : movementAcceleration(movementAcceleration), maxMovementAcceleration(maxMovementAcceleration),
        jumpAcceleration(jumpAcceleration), maxJumpHeight(maxJumpHeight), gravityRate(gravityRate),
        maxGravityAcceleration(maxGravityAcceleration)
{

}
