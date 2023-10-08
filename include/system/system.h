#pragma once

#ifndef MERCILESS_MARIO_SYSTEM_H
#define MERCILESS_MARIO_SYSTEM_H

class System
{
    public:
        virtual ~System() = default;
        virtual void execute() = 0;
};

#endif //MERCILESS_MARIO_SYSTEM_H
