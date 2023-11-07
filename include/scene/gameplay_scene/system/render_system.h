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
#include "c_weapon.h"
#include "entity/entity_manager.h"
#include "scene/gameplay_scene/system/render_system.h"
#include "c_sprite_group.h"

class RenderSystem : public System
{
    public:
        explicit RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager);

        void execute() override;

    private:
        void drawEntities();
        void drawGuiData();
        void configureTextRendering();
        void drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position);

    private:
        static const inline std::string FONT_PATH = "resources/fonts/mario_256.ttf";
        static constexpr int TEXT_POSITION_OFFSET = 64;

        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;

        static inline sf::Text m_ammoText;
        static inline sf::Font m_font;
};


#endif //MERCILESS_MARIO_RENDER_SYSTEM_H
