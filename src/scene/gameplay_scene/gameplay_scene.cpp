#include "scene/gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
    registerActions();
    registerSystems(engine);

    createPlayer();
    createPlatform(sf::Vector2f(500, 100), sf::Vector2f(300, 500));
    createPlatform(sf::Vector2f(200, 20), sf::Vector2f(500, 300));
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(900, 620));
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update();
}

void GameplayScene::render()
{
    gameEngine.window.clear(sf::Color::Black);

    m_systemManager.render();

    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    std::shared_ptr<Entity>& player = m_entityManager.getEntityByType(Entity::Type::PLAYER);
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
    if (action.getType() == Action::Type::MOVE_UP)
    {
        actionComponent->isMovingUp = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::MOVE_DOWN)
    {
        actionComponent->isMovingDown = action.getMode() == Action::Mode::PRESS;
    }
    if (action.getType() == Action::Type::JUMP)
    {
        actionComponent->isJumping = action.getMode() == Action::Mode::PRESS;
    }
}

void GameplayScene::registerActions()
{
    registerActionType(sf::Keyboard::Left, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Right, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Down, Action::Type::MOVE_DOWN);
    registerActionType(sf::Keyboard::Space, Action::Type::JUMP);

    registerActionType(sf::Keyboard::A, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::D, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::W, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::S, Action::Type::MOVE_DOWN);
}

void GameplayScene::registerSystems(GameEngine& engine)
{
    m_systemManager.registerSystem(
            std::make_shared<TransformSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<CollisionSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<RenderSystem>(engine.window, m_entityManager), SystemManager::SystemType::RENDER);
}

void GameplayScene::createPlayer()
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::RectangleShape playerShape{sf::Vector2f(50, 50)};
    playerShape.setOrigin(25, 25);
    playerShape.setFillColor(sf::Color::Yellow);
    playerShape.setOutlineColor(sf::Color::White);
    playerShape.setOutlineThickness(2.0f);

    sf::Vector2f position = sf::Vector2f(100, 100);
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    player->addComponent(Component::Type::SPRITE, std::make_shared<CSprite>(playerShape));
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::GRAVITY, std::make_shared<CGravity>(3.0f));
}

void GameplayScene::createPlatform(sf::Vector2f size, sf::Vector2f position)
{
    std::shared_ptr<Entity> platformA = m_entityManager.addEntity(Entity::Type::PLATFORM);

    sf::RectangleShape platformShape(size);
    platformShape.setOrigin(size.x/2, size.y/2);
    platformShape.setFillColor(sf::Color::Green);
    platformShape.setOutlineColor(sf::Color::White);
    platformShape.setOutlineThickness(3.0f);

    sf::Vector2f velocity = sf::Vector2f(3, 3);

    platformA->addComponent(Component::Type::SPRITE, std::make_shared<CSprite>(platformShape));
    platformA->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    platformA->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
}