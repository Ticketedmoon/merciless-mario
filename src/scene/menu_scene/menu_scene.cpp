#include "scene/menu_scene/menu_scene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    registerActions();
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    buildLevelButtonSelection();
}

void MenuScene::update()
{
    for (std::pair<sf::Color, sf::Text>& buttonPair: levelSelectButtons)
    {
        sf::Color& levelButtonColour = buttonPair.first;
        buttonPair.second.setFillColor({levelButtonColour.r, levelButtonColour.g, levelButtonColour.b, 48});
        buttonPair.second.setOutlineColor(LEVEL_BUTTON_TEXT_COLOUR);
        buttonPair.second.setOutlineThickness(1.0f);
    }

    std::pair<sf::Color, sf::Text>& currentSelectedButtonPair = levelSelectButtons.at(currentSelectItem);
    currentSelectedButtonPair.second.setFillColor(LEVEL_BUTTON_TEXT_COLOUR);
    currentSelectedButtonPair.second.setOutlineColor(sf::Color::Black);
}

void MenuScene::render()
{
    gameEngine.window.clear(BACKGROUND_COLOR);

    gameEngine.window.draw(titleText.second);

    for (const std::pair<sf::Color, sf::Text>& buttonPair: levelSelectButtons)
    {
        gameEngine.window.draw(buttonPair.second);
    }

    gameEngine.window.display();
}

void MenuScene::registerActions()
{
    registerCursorActionType(sf::Event::MouseEntered, Action::Type::CURSOR_MOVE);
    registerCursorActionType(sf::Event::MouseMoved, Action::Type::CURSOR_MOVE);
    registerActionType(CURSOR_LEFT, Action::Type::CURSOR_SELECT);

    registerActionType(sf::Keyboard::Left, Action::Type::MENU_OPTION_MOVE_LEFT);
    registerActionType(sf::Keyboard::Right, Action::Type::MENU_OPTION_MOVE_RIGHT);
    registerActionType(sf::Keyboard::Up, Action::Type::MENU_OPTION_MOVE_UP);
    registerActionType(sf::Keyboard::Down, Action::Type::MENU_OPTION_MOVE_DOWN);

    registerActionType(sf::Keyboard::A, Action::Type::MENU_OPTION_MOVE_LEFT);
    registerActionType(sf::Keyboard::D, Action::Type::MENU_OPTION_MOVE_RIGHT);
    registerActionType(sf::Keyboard::W, Action::Type::MENU_OPTION_MOVE_UP);
    registerActionType(sf::Keyboard::S, Action::Type::MENU_OPTION_MOVE_DOWN);

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
        case Action::Type::MENU_OPTION_MOVE_LEFT:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem == 0
                        ? totalLevelSelectButtons - 1
                        : currentSelectItem - 1) % totalLevelSelectButtons;
            }
            break;
        case Action::Type::MENU_OPTION_MOVE_RIGHT:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem + 1) % totalLevelSelectButtons;
            }
            break;
        case Action::Type::MENU_OPTION_MOVE_DOWN:
            if (action.getMode() == Action::Mode::PRESS)
            {
                uint8_t x = currentSelectItem - TOTAL_BUTTONS_IN_ROW;
                uint8_t y = currentSelectItem + TOTAL_BUTTONS_IN_ROW;
                currentSelectItem = (y > totalLevelSelectButtons ? x : y) % totalLevelSelectButtons;
            }

            break;
        case Action::Type::MENU_OPTION_MOVE_UP:
            if (action.getMode() == Action::Mode::PRESS)
            {
                uint8_t x = totalLevelSelectButtons - TOTAL_BUTTONS_IN_ROW;
                uint8_t y = currentSelectItem - TOTAL_BUTTONS_IN_ROW;
                currentSelectItem = (y < 0 ? x : y) % totalLevelSelectButtons;
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
    // Get level name
    std::string levelFileName = levelSelectButtons.at(currentSelectItem).second.getString();

    // Replace the space with underscore since that is our level file naming structure.
    // @cleanup: This is a little hacky, we could just store the file names alongside the level names.
    std::ranges::replace(levelFileName, ' ', '_');

    // Update the scene.
    gameEngine.changeScene(Scene::Type::LEVEL_ONE_GAMEPLAY_SCENE, std::make_shared<GameplayScene>(gameEngine, levelFileName));
}

void MenuScene::handleMouseClick()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));
    bool clickedOnMenuItem;
    
    for (int i = 0; i < levelSelectButtons.size(); i++)
    {
        const sf::Text& button = levelSelectButtons.at(i).second;
        if (button.getGlobalBounds().contains(mousePosF))
        {
            currentSelectItem = i;
            clickedOnMenuItem = true;
        }
    }

    if (clickedOnMenuItem)
    {
        handleButtonSelect();
    }
}

void MenuScene::handleMouseHover()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    bool isHoveringOverButton = false;
    for (int i = 0; i < levelSelectButtons.size(); i++)
    {
        const sf::Text& button = levelSelectButtons.at(i).second;
        if (button.getGlobalBounds().contains(mousePosF))
        {
            currentSelectItem = i;
            onHoverModifyCursorDesign(sf::Cursor::Hand);
            isHoveringOverButton = true;
        }
    }

    if (!isHoveringOverButton)
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

void MenuScene::buildLevelButtonSelection()
{
    titleText = createTextElementPair("Merciless Mario", TITLE_FONT_SIZE, TITLE_TEXT_COLOUR, sf::Color::Black, 2.0f,
            sf::Vector2f(TITLE_TEXT_POSITION_X, TITLE_TEXT_POSITION_Y));

    // Open directory, read each file, all with name structure: `level_X.txt`
    // For each file, create a new button element using the file name.
    std::set<std::filesystem::path> sortedLevels;

    std::string directory_path = "resources/level";
    for (const auto & file: std::filesystem::directory_iterator(directory_path))
    {
        std::string fileNameStr = file.path().filename().string();
        std::ranges::replace(fileNameStr, '_', ' ');
        const auto& levelName = fileNameStr.substr(0, fileNameStr.size() - 4);
        sortedLevels.insert(levelName);
    }

    uint16_t yOffset = 1;
    for (const std::filesystem::path& levelName : sortedLevels)
    {
        if (totalLevelSelectButtons != 0 && totalLevelSelectButtons % TOTAL_BUTTONS_IN_ROW == 0)
        {
            yOffset += 1;
        }

        uint16_t screenPositionOffsetX = 400 * (totalLevelSelectButtons % TOTAL_BUTTONS_IN_ROW);
        uint16_t screenPositionOffsetY = 100 * yOffset;
        uint32_t LEVEL_BUTTON_POSITION_X = WINDOW_WIDTH / (TOTAL_BUTTONS_IN_ROW+2) + screenPositionOffsetX;
        uint32_t LEVEL_BUTTON_POSITION_Y = WINDOW_HEIGHT / TOTAL_BUTTONS_IN_ROW + screenPositionOffsetY;

        const sf::Vector2f& position = sf::Vector2f(LEVEL_BUTTON_POSITION_X, LEVEL_BUTTON_POSITION_Y);
        std::pair<sf::Color, sf::Text> levelButton = createTextElementPair(levelName, BUTTON_FONT_SIZE,
                LEVEL_BUTTON_TEXT_COLOUR, sf::Color::Black, 2.0f, position);
        levelSelectButtons.emplace_back(levelButton);
        totalLevelSelectButtons++;
    }

    std::cout << "Total levels found: " << std::to_string(totalLevelSelectButtons) << '\n';
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
