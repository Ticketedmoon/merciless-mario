#include "c_cursor_follower.h"

CCursorFollower::CCursorFollower() = default;

float CCursorFollower::getArmPointAngleRadians(sf::Vector2f originVec)
{
    float dx = armPointLocation.x - originVec.x;
    float dy = armPointLocation.y - originVec.y;
    return std::atan2(dy, dx);
}

float CCursorFollower::getArmPointAngleDegrees(sf::Vector2f originVec)
{
    float angle_radians = getArmPointAngleRadians(originVec);
    float angle_degrees = angle_radians * (180 / M_PI);
    return 180 + angle_degrees;
}
