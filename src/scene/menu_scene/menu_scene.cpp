#include "scene/menu_scene/menu_scene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    registerActions();
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    createTextElements();
}

void MenuScene::update()
{
    sf::Color& levelOneButtonColour = levelOneTextButton.first;
    sf::Color& levelTwoButtonColour = levelTwoTextButton.first;
    sf::Color& levelThreeButtonColour = levelThreeTextButton.first;

    levelOneButtonColour.a = 48;
    levelTwoButtonColour.a = 48;
    levelThreeButtonColour.a = 48;

    levelOneTextButton.second.setOutlineColor(levelOneButtonColour);
    levelTwoTextButton.second.setOutlineColor(levelOneButtonColour);
    levelThreeTextButton.second.setOutlineColor(levelOneButtonColour);

    if (currentSelectItem == 0)
    {
        levelOneTextButton.second.setFillColor(LEVEL_BUTTON_TEXT_COLOUR);
        levelOneTextButton.second.setOutlineColor(sf::Color::Black);

        levelTwoTextButton.second.setFillColor(levelTwoButtonColour);
        levelThreeTextButton.second.setFillColor(levelThreeButtonColour);
    }
    else if (currentSelectItem == 1)
    {
        levelOneTextButton.second.setFillColor(levelOneButtonColour);
        levelTwoTextButton.second.setFillColor(LEVEL_BUTTON_TEXT_COLOUR);
        levelTwoTextButton.second.setOutlineColor(sf::Color::Black);
        levelThreeTextButton.second.setFillColor(levelThreeButtonColour);
    }
    else if (currentSelectItem == 2)
    {
        levelOneTextButton.second.setFillColor(levelOneButtonColour);
        levelTwoTextButton.second.setFillColor(levelTwoButtonColour);
        levelThreeTextButton.second.setFillColor(LEVEL_BUTTON_TEXT_COLOUR);
        levelThreeTextButton.second.setOutlineColor(sf::Color::Black);
    }
}

void MenuScene::render()
{
    gameEngine.window.clear(BACKGROUND_COLOR);

    gameEngine.window.draw(titleText.second);
    gameEngine.window.draw(levelOneTextButton.second);
    gameEngine.window.draw(levelTwoTextButton.second);
    gameEngine.window.draw(levelThreeTextButton.second);

    gameEngine.window.display();
}

void MenuScene::registerActions()
{
    registerCursorActionType(sf::Event::MouseEntered, Action::Type::CURSOR_MOVE);
    registerCursorActionType(sf::Event::MouseMoved, Action::Type::CURSOR_MOVE);
    registerActionType(CURSOR_LEFT, Action::Type::CURSOR_SELECT);

    registerActionType(sf::Keyboard::Up, Action::Type::MENU_OPTION_MOVE_UP);
    registerActionType(sf::Keyboard::Down, Action::Type::MENU_OPTION_MOVE_DOWN);
    registerActionType(sf::Keyboard::Enter, Action::Type::MENU_SELECT);
    registerActionType(sf::Keyboard::Escape, Action::Type::SCENE_EXIT);
}

void MenuScene::performAction(Action& action)
{
    switch (action.getType())
    {
        case Action::Type::SCENE_EXIT:
        {
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }

            gameEngine.window.close();
            break;
        }
        case Action::Type::MENU_OPTION_MOVE_DOWN:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem + 1) % TOTAL_TEXT_BUTTONS;
            }
            break;
        case Action::Type::MENU_OPTION_MOVE_UP:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem == 0 ? TOTAL_TEXT_BUTTONS-1 : currentSelectItem - 1) % TOTAL_TEXT_BUTTONS;
            }
            break;
        case Action::Type::CURSOR_MOVE:
        {
            handleMouseHover();
            break;
        }
        case Action::Type::MENU_SELECT:
        {
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }
            handleButtonSelect();
            break;
        }
        case Action::Type::CURSOR_SELECT:
        {
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }
            handleMouseClick();
            break;
        }
        default:
            break;
    }
}

void MenuScene::handleButtonSelect()
{
    if (currentSelectItem == 0)
    {
        gameEngine.changeScene(Scene::Type::LEVEL_ONE_GAMEPLAY_SCENE, std::make_shared<GameplayScene>(gameEngine));
    }
    else if (currentSelectItem == 1)
    {
        // TODO Update me to different level
        gameEngine.changeScene(Scene::Type::LEVEL_TWO_GAMEPLAY_SCENE, std::make_shared<GameplayScene>(gameEngine));
    }
    else if (currentSelectItem == 2)
    {
        // TODO Update me to different level
        gameEngine.changeScene(Scene::Type::LEVEL_THREE_GAMEPLAY_SCENE, std::make_shared<GameplayScene>(gameEngine));
    }
}

void MenuScene::handleMouseClick()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    if (levelOneTextButton.second.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 0;
        handleButtonSelect();
        return;
    }

    if (levelTwoTextButton.second.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 1;
        handleButtonSelect();
        return;
    }

    if (levelThreeTextButton.second.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 2;
        handleButtonSelect();
        return;
    }

}

void MenuScene::handleMouseHover()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    auto& [originalLevelOneTextButtonColor, levelOneButton] = levelOneTextButton;
    auto& [originalLevelTwoTextButtonColor, levelTwoButton] = levelTwoTextButton;
    auto& [originalLevelThreeTextButtonColor, levelThreeButton] = levelThreeTextButton;

    if (levelOneButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 0;
        onHoverModifyCursorDesign(sf::Cursor::Hand);
        return;
    }
    else if (levelTwoButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 1;
        onHoverModifyCursorDesign(sf::Cursor::Hand);
        return;
    }
    else if (levelThreeButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 2;
        onHoverModifyCursorDesign(sf::Cursor::Hand);
        return;
    }
    else
    {
        onHoverModifyCursorDesign(sf::Cursor::Arrow);
    }
}

void MenuScene::onHoverModifyCursorDesign(sf::Cursor::Type cursorTypeOnHover)
{
    if (cursor.loadFromSystem(cursorTypeOnHover))
    {
        gameEngine.window.setMouseCursor(cursor);
    }
}

void MenuScene::createTextElements()
{
    titleText = createTextElementPair("Merciless Mario", TITLE_FONT_SIZE, TITLE_TEXT_COLOUR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 250));

    levelOneTextButton = createTextElementPair("Level 1", BUTTON_FONT_SIZE, LEVEL_BUTTON_TEXT_COLOUR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));

    levelTwoTextButton = createTextElementPair("Level 2", BUTTON_FONT_SIZE, LEVEL_BUTTON_TEXT_COLOUR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100));

    levelThreeTextButton = createTextElementPair("Level 3", BUTTON_FONT_SIZE, LEVEL_BUTTON_TEXT_COLOUR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 250));
}

/**
 * Creates a text element pairing from the original text color to the sf::Text object.
 * This is useful when we want to update the colour on hover and then back to the original colour afterwards.
 *
 * @param value The text value
 * @param characterSize The character size
 * @param fillColor The fill Color
 * @param outlineColor The outline Color
 * @param outlineThickness The outline thickness
 * @param position The position of the text
 * @return A text element pairing from the original text color to the sf::Text object.
 */
std::pair<sf::Color, sf::Text> MenuScene::createTextElementPair(const std::string& value, uint16_t characterSize,
        sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position)
{
    sf::Text text;
    text.setPosition(position);
    text.setFont(m_font);
    text.setString(value);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(characterSize);
    text.setFillColor(fillColor);
    text.setOutlineColor(outlineColor);
    text.setOutlineThickness(outlineThickness);
    text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
    return std::make_pair(text.getFillColor(), text);
}
