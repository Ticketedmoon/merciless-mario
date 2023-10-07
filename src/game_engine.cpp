#include "game_engine.h"

GameEngine::GameEngine()
{
    createGameWindow();
    gameScenes[currentScene] = std::make_shared<GameplayScene>(*this);
}

GameEngine::~GameEngine()
{
    delete m_audioManager;
}

void GameEngine::startGameLoop()
{
    while (window.isOpen())
    {
        deltaClock.restart();

        handleInput();
        update();
        render();
    }
}

void GameEngine::update()
{
    gameScenes[currentScene]->update();
}

void GameEngine::render()
{
    gameScenes[currentScene]->render();
}

void GameEngine::changeScene(Scene::Type sceneType, const std::shared_ptr<Scene>& scene)
{
    currentScene = sceneType;
    gameScenes[currentScene] = scene;
}

/* Get the keyboard input
 * if keyboard input is not part of scene, skip!
 * pass it down to scenes internal mapping of keys to ACTION
 * let scene do what it needs to do in performAction();
 */
void GameEngine::handleInput()
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            std::shared_ptr<Scene>& scene = gameScenes[currentScene];
            if (!scene->hasActionType(event.key.code))
            {
                continue;
            }
            const Action::Mode actionMode = event.type == sf::Event::KeyPressed ? Action::Mode::PRESS : Action::Mode::RELEASE;
            Action action = Action(scene->getActionType(event.key.code), actionMode);
            scene->performAction(action);
        }

        if (event.type == sf::Event::MouseEntered || event.type == sf::Event::MouseMoved)
        {
            std::shared_ptr<Scene>& scene = gameScenes[currentScene];
            if (!scene->hasEventType(event.type))
            {
                continue;
            }

            const Action::Mode actionMode = event.type == sf::Event::MouseButtonPressed ? Action::Mode::PRESS : Action::Mode::RELEASE;
            Action action = Action(scene->getCursorActionType(event.type), actionMode);
            scene->performAction(action);
        }

        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
        {
            std::shared_ptr<Scene>& scene = gameScenes[currentScene];
            int buttonCode = event.mouseButton.button == 0 ? Scene::CursorButton::CURSOR_LEFT : Scene::CursorButton::CURSOR_RIGHT;
            if (!scene->hasActionType(buttonCode))
            {
                continue;
            }

            const Action::Mode actionMode = event.type == sf::Event::MouseButtonPressed
                    ? Action::Mode::PRESS
                    : Action::Mode::RELEASE;
            Action action = Action(scene->getActionType(buttonCode), actionMode);
            scene->performAction(action);
        }
    }
}

void GameEngine::createGameWindow()
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());

    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;
    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else
    {
        window.setFramerateLimit(APP_FRAME_RATE);
    }
}
