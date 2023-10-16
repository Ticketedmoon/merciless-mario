#include "c_action.h"

float CAction::getArmPointAngleRadians(sf::Vector2f originVec)
{
    double dx = armPointLocation.x - originVec.x;
    double dy = armPointLocation.y - originVec.y;
    return std::atan2(dy, dx);
}

float CAction::getArmPointAngleDegrees(sf::Vector2f originVec)
{
    float angle_radians = getArmPointAngleRadians(originVec);
    float angle_degrees = angle_radians * (180 / M_PI);
    return 180 + angle_degrees;
}
