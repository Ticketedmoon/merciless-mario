#include "projectile_system.h"

ProjectileSystem::ProjectileSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void ProjectileSystem::execute()
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        return;
    }

    for (std::shared_ptr<Entity>& player: players)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                player->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CWeapon> cWeapon = std::static_pointer_cast<CWeapon>(
                player->getComponentByType(Component::Type::WEAPON));
        if (cAction->isShooting)
        {
            cAction->isShooting = false;
            if (cWeapon->hasBulletsInRound())
            {
                // play fire sfx
                m_audioManager->playSound(AudioManager::AudioType::SHOTGUN_FIRE, 20.0f);

                // decrement ammo
                cWeapon->reduceAmmoInCurrentWeaponRound();
            }
        }
        if (cAction->isReloading)
        {
            cAction->isReloading = false;
            if (cWeapon->hasMagazinesAvailable() && !cWeapon->hasBulletsInRound())
            {
                // play reload sfx
                m_audioManager->playSound(AudioManager::AudioType::SHOTGUN_RELOAD, 20.0f);

                // Update round
                cWeapon->addMagazineToWeapon();
            }
        }
    }
}