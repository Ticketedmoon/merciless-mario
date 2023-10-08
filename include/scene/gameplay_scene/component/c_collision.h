#ifndef MERCILESS_MARIO_C_COLLISION_H
#define MERCILESS_MARIO_C_COLLISION_H

#include "component.h"

class CCollision : public Component
{
    public:
        CCollision();

    public:
        bool isCollidingLeft = false;
        bool isCollidingRight = false;
        bool isCollidingUp = false;
        bool isCollidingDown = false;
};


#endif //MERCILESS_MARIO_C_COLLISION_H
