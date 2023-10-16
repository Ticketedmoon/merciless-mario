#pragma once

#ifndef MERCILESS_MARIO_ACTION_H
#define MERCILESS_MARIO_ACTION_H

class Action
{
    public:
        enum class Type
        {
            MOVE_LEFT = 0,
            MOVE_RIGHT = 1,
            LOOK_UP = 2,
            CROUCH = 3,
            JUMP = 4,
            POINT = 5,
            SHOOT = 6,
        };
        enum class Mode
        {
            PRESS,
            RELEASE
        };

        Action(Type type, Mode mode);
        Type getType();
        Mode getMode();

    private:
        Type m_type;
        Mode m_mode;

};


#endif //MERCILESS_MARIO_ACTION_H