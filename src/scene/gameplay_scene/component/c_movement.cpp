#include "c_movement.h"

CMovement::CMovement(float movementAcceleration, float movementDecelerationPerFrame, float maxMovementAcceleration,
        float jumpAcceleration, float maxJumpHeight, float gravityRate, float maxGravityAcceleration)
        : movementAcceleration(movementAcceleration), movementDecelerationPerFrame(movementDecelerationPerFrame),
          maxMovementAcceleration(maxMovementAcceleration),
          jumpAcceleration(jumpAcceleration), maxJumpVelocity(maxJumpHeight), gravityRate(gravityRate),
          maxGravityAcceleration(maxGravityAcceleration)
{

}
