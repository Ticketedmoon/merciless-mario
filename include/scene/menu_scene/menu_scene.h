#pragma once

#ifndef PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H
#define PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

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
        void onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover);

        void createTextElements();

        std::pair<sf::Color, sf::Text> createTextElementPair(const std::string& value, uint16_t characterSize,
                sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position);
        void registerActions();

    private:
        static inline const std::string FONT_PATH = "resources/fonts/mario_256.ttf";
        static inline const sf::Color& BACKGROUND_COLOR = sf::Color{4, 154, 199, 255};
        // Have this based on the amount of level files in directory.
        static inline const uint8_t TOTAL_TEXT_BUTTONS = 3;
        static inline const sf::Color& TITLE_TEXT_COLOUR = sf::Color::Red;
        static inline const sf::Color& LEVEL_BUTTON_TEXT_COLOUR = sf::Color::Blue;
        static inline const uint8_t TITLE_FONT_SIZE = 96;
        static inline const uint8_t BUTTON_FONT_SIZE = 64;

        sf::Font m_font;
        uint8_t currentSelectItem = 0;

        std::pair<sf::Color, sf::Text> titleText;

        std::pair<sf::Color, sf::Text> levelOneTextButton;
        std::pair<sf::Color, sf::Text> levelTwoTextButton;
        std::pair<sf::Color, sf::Text> levelThreeTextButton;

        sf::Cursor cursor;
};

#endif //PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H
