#pragma once

#ifndef PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H
#define PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <filesystem>

#include "game_engine.h"
#include "gameplay_scene.h"
#include "scene.h"

class MenuScene : public Scene
{
    public:
        explicit MenuScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void handleMouseHover();
        void handleMouseClick();
        void handleButtonSelect();
        void onHoverModifyCursorDesign(sf::Cursor::Type cursorTypeOnHover);

        void buildLevelButtonSelection();

        std::pair<sf::Color, sf::Text> createTextElementPair(const std::string& value, uint16_t characterSize,
                sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position);
        void registerActions();

    private:
        static inline const std::string FONT_PATH = "resources/fonts/mario_256.ttf";
        static inline const sf::Color& BACKGROUND_COLOR = sf::Color{4, 154, 199, 255};
        static inline constexpr uint32_t TITLE_TEXT_POSITION_X = WINDOW_WIDTH / 2;
        static inline constexpr uint32_t TITLE_TEXT_POSITION_Y = WINDOW_HEIGHT / 2 - 250;
        static inline constexpr uint8_t TOTAL_BUTTONS_IN_ROW = 3;

        // Have this based on the amount of level files in directory.

        static inline const sf::Color& TITLE_TEXT_COLOUR{255, 32, 0};
        static inline const sf::Color& LEVEL_BUTTON_TEXT_COLOUR{0, 0, 255};
        static inline constexpr uint8_t TITLE_FONT_SIZE = 96;
        static inline constexpr uint8_t BUTTON_FONT_SIZE = 64;

        sf::Font m_font;
        sf::Cursor cursor;
        uint8_t totalLevelSelectButtons{};
        uint8_t currentSelectItem = 0;

        std::pair<sf::Color, sf::Text> titleText;
        std::vector<std::pair<sf::Color, sf::Text>> levelSelectButtons;

};

#endif //PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H
