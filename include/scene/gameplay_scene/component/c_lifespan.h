#pragma once

#ifndef MERCILESS_MARIO_C_LIFESPAN_H
#define MERCILESS_MARIO_C_LIFESPAN_H

#include <cstdint>
#include "component.h"

class CLifespan : public Component
{
    public:
        explicit CLifespan(uint8_t timeToLive);

        [[nodiscard]] bool isAlive() const;
        void decreaseTimeToLive();

    private:
        uint8_t m_timeToLive;
};


#endif //MERCILESS_MARIO_C_LIFESPAN_H
