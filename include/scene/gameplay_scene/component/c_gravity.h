#pragma once

#ifndef MERCILESS_MARIO_C_GRAVITY_H
#define MERCILESS_MARIO_C_GRAVITY_H

#include "component.h"

class CGravity : public Component
{
    public:
        explicit CGravity(float gravity);
        float gravity;
};


#endif //MERCILESS_MARIO_C_GRAVITY_H
