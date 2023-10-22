#pragma once

#ifndef MERCILESS_MARIO_RENDER_SYSTEM_H
#define MERCILESS_MARIO_RENDER_SYSTEM_H

#include <cmath>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "c_transform.h"
#include "system.h"
#include "entity/entity_manager.h"
#include "scene/gameplay_scene/system/render_system.h"
#include "c_sprite_group.h"

class RenderSystem : public System
{
    public:
        explicit RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager);

        void execute() override;

    private:
        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;
};


#endif //MERCILESS_MARIO_RENDER_SYSTEM_H
