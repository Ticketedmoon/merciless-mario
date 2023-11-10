#pragma once

#ifndef MERCILESS_MARIO_C_WEAPON_H
#define MERCILESS_MARIO_C_WEAPON_H

#include <cmath>

#include "component.h"

enum class WeaponType
{
    PISTOL = 7,
    SHOTGUN = 3,
    SNIPER_RIFLE = 1
};

class CWeapon : public Component
{
    public:
        CWeapon(WeaponType weaponType, uint16_t totalRoundsOfAmmo);
        float getArmPointAngleRadians(sf::Vector2f originVec);
        float getArmPointAngleDegrees(sf::Vector2f originVec);

        void reduceAmmoInCurrentWeaponRound();
        void addMagazineToWeapon();
        bool hasBulletsInRound();
        bool hasMagazinesAvailable();

    public:
        WeaponType weaponType;
        uint8_t currentRoundBullets;
        uint8_t totalMagazinesAvailable;
        sf::Vector2f weaponPointLocation;
        bool hasWeaponBeenCollidedWith;
};

#endif //MERCILESS_MARIO_C_WEAPON_H
