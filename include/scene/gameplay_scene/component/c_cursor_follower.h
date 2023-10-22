#ifndef MERCILESS_MARIO_C_CURSOR_FOLLOWER_H
#define MERCILESS_MARIO_C_CURSOR_FOLLOWER_H

#include <SFML/System/Vector2.hpp>
#include <valarray>
#include <cmath>
#include "component.h"

class CCursorFollower : public Component
{
    public:
        CCursorFollower();
        float getArmPointAngleRadians(sf::Vector2f originVec);
        float getArmPointAngleDegrees(sf::Vector2f originVec);

    public:
        sf::Vector2f armPointLocation;

};


#endif //MERCILESS_MARIO_C_CURSOR_FOLLOWER_H
