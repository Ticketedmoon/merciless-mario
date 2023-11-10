#include "c_weapon.h"

// We minus against the `totalRoundsOfAmmo` since the currentRoundBullets will also be full.
CWeapon::CWeapon(WeaponType weaponType, uint16_t totalRoundsOfAmmo)
        : weaponType(weaponType),
          currentRoundBullets(static_cast<int>(weaponType)),
          totalMagazinesAvailable(totalRoundsOfAmmo - 1),
          hasWeaponBeenCollidedWith(true)
{

}

float CWeapon::getArmPointAngleRadians(sf::Vector2f originVec)
{
    float dx = weaponPointLocation.x - originVec.x;
    float dy = weaponPointLocation.y - originVec.y;
    return std::atan2(dy, dx);
}

float CWeapon::getArmPointAngleDegrees(sf::Vector2f originVec)
{
    float angle_radians = getArmPointAngleRadians(originVec);
    float angle_degrees = angle_radians * (180 / M_PI);
    return 180 + angle_degrees;
}

bool CWeapon::hasBulletsInRound()
{
    return currentRoundBullets > 0;
}

bool CWeapon::hasMagazinesAvailable()
{
    return totalMagazinesAvailable > 0;
}

void CWeapon::reduceAmmoInCurrentWeaponRound()
{
    if (hasBulletsInRound())
    {
        currentRoundBullets--;
    }
}

void CWeapon::addMagazineToWeapon()
{
    if (totalMagazinesAvailable > 0)
    {
        // Reduce rounds
        totalMagazinesAvailable--;

        // Reset current bullets in round
        currentRoundBullets = static_cast<uint16_t>(weaponType);
    }
}