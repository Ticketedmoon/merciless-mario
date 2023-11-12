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
            SHOOT = 5,
            RELOAD = 6,
            CURSOR_MOVE = 7,
            CURSOR_SELECT = 8,
            MENU_SELECT = 9,

            MENU_OPTION_MOVE_UP = 10,
            MENU_OPTION_MOVE_DOWN = 11,
            MENU_OPTION_MOVE_LEFT = 12,
            MENU_OPTION_MOVE_RIGHT = 13,

            SCENE_EXIT = 14,
            START_OR_STOP_MUSIC = 15 // DEBUG
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