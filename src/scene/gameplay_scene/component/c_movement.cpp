#include "c_movement.h"

CMovement::CMovement(float movementAcceleration, float movementDecelerationPerFrame, float maxMovementAcceleration,
        float jumpAcceleration, float maxJumpVelocity, float gravityRate, float maxGravityAcceleration)
        : movementAcceleration(movementAcceleration), movementDecelerationPerFrame(movementDecelerationPerFrame),
          maxMovementAcceleration(maxMovementAcceleration),
          jumpAcceleration(jumpAcceleration), maxJumpVelocity(maxJumpVelocity), gravityRate(gravityRate),
          maxGravityAcceleration(maxGravityAcceleration)
{

}
