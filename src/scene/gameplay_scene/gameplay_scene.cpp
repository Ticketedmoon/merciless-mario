#include "scene/gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
    m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    m_renderSprite.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    registerActions();
    registerSystems();
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update();
}

void GameplayScene::render()
{
    gameEngine.window.clear();
    m_renderTexture.clear(LEVEL_BACKGROUND_COLOR);

    m_systemManager.render();

    m_renderTexture.display();
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    gameEngine.window.draw(m_renderSprite, sf::RenderStates(sf::BlendAdd));

    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        return;
    }
    std::shared_ptr<Entity>& player = players.front();
    std::shared_ptr<CAction> actionComponent = std::static_pointer_cast<CAction>(
            player->getComponentByType(Component::Type::USER_INPUT));

    if (action.getType() == Action::Type::MOVE_LEFT)
    {
        actionComponent->isMovingLeft = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::MOVE_RIGHT)
    {
        actionComponent->isMovingRight = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::LOOK_UP)
    {
        actionComponent->isLookingUp = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::CROUCH)
    {
        actionComponent->isCrouching = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::JUMP)
    {
        if (action.getMode() == Action::Mode::RELEASE)
        {
            return;
        }

        actionComponent->isJumping = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::SHOOT)
    {
        actionComponent->isShooting = action.getMode() == Action::Mode::PRESS;
    }
}

void GameplayScene::registerActions()
{
    // Keys
    registerActionType(sf::Keyboard::Left, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Right, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Up, Action::Type::LOOK_UP);
    registerActionType(sf::Keyboard::Down, Action::Type::CROUCH);
    registerActionType(sf::Keyboard::Space, Action::Type::JUMP);

    registerActionType(sf::Keyboard::A, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::D, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::W, Action::Type::LOOK_UP);
    registerActionType(sf::Keyboard::S, Action::Type::CROUCH);

    // Mouse
    registerActionType(CursorButton::CURSOR_LEFT, Action::Type::SHOOT);
}

void GameplayScene::registerSystems()
{
    m_systemManager.registerSystem(
            std::make_shared<EntitySpawnSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<TransformSystem>(m_entityManager, gameEngine.window, m_renderTexture), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<CollisionSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<LifespanSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<AnimationSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(
            std::make_shared<ViewSystem>(m_entityManager, m_renderTexture), SystemManager::SystemType::RENDER);
    m_systemManager.registerSystem(
            std::make_shared<RenderSystem>(m_renderTexture, m_entityManager), SystemManager::SystemType::RENDER);
}