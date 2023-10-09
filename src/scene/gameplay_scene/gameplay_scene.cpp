#include "scene/gameplay_scene/gameplay_scene.h"
#include "view_system.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
    m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    m_renderSprite.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    registerActions();
    createPlayer();
    registerSystems();

    createPlatform(sf::Vector2f(500, 100), sf::Vector2f(300, 1300), sf::Color::Green);
    createPlatform(sf::Vector2f(200, 20), sf::Vector2f(500, 1100), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(900, 1420), sf::Color::Green);

    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(1200, 1520), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(1500, 1320), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2000, 1220), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2400, 1020), sf::Color::Green);

    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2700, 950), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2500, 750), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2800, 650), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(3100, 630), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2600, 620), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(2200, 610), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(1800, 580), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(1200, 560), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(700, 560), sf::Color::Green);
    createPlatform(sf::Vector2f(100, 50), sf::Vector2f(300, 500), sf::Color::Blue);
    createPlatform(sf::Vector2f(50, MAX_LEVEL_HEIGHT), sf::Vector2f(0, 1300), sf::Color::Green);
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update();

    // FIXME Should this be here? seems related to the transform system but we need to recreate the player.
    //       Would a fn ptr / callback be a good approach?
    if (player->hasComponent(Component::Type::TRANSFORM))
    {
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
        if (cTransform->m_position.y > LEVEL_DEPTH_KILL_ZONE)
        {
            createPlayer();
        }
    }
}

void GameplayScene::render()
{
    gameEngine.window.clear();

    m_renderTexture.clear(sf::Color::Black);
    m_renderTexture.setView(m_renderTexture.getDefaultView());

    m_systemManager.render();

    m_renderTexture.display();
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    gameEngine.window.draw(m_renderSprite, sf::RenderStates(sf::BlendAdd));

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

void GameplayScene::registerSystems()
{
    m_systemManager.registerSystem(
            std::make_shared<TransformSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<CollisionSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(
            std::make_shared<ViewSystem>(player, m_renderTexture), SystemManager::SystemType::RENDER);
    m_systemManager.registerSystem(
            std::make_shared<RenderSystem>(m_renderTexture, m_entityManager), SystemManager::SystemType::RENDER);
}

void GameplayScene::createPlayer()
{
    if (player == nullptr)
    {
        player = m_entityManager.addEntity(Entity::Type::PLAYER);
    }

    sf::Vector2f position = sf::Vector2f(100, 1000);
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    sf::RectangleShape playerShape{sf::Vector2f(50, 50)};
    playerShape.setOrigin(25, 25);
    playerShape.setPosition(position);
    playerShape.setFillColor(sf::Color::Yellow);
    playerShape.setOutlineColor(sf::Color::White);
    playerShape.setOutlineThickness(2.0f);


    player->addComponent(Component::Type::SPRITE, std::make_shared<CSprite>(playerShape));
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(0.1f, 0.025f, 1.25f, -10.0f, 0.3f, 5.0f));
}

void GameplayScene::createPlatform(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor)
{
    std::shared_ptr<Entity> platformA = m_entityManager.addEntity(Entity::Type::PLATFORM);

    sf::RectangleShape platformShape(size);
    platformShape.setOrigin(size.x/2, size.y/2);
    platformShape.setPosition(position);
    platformShape.setFillColor(fillColor);
    platformShape.setOutlineColor(sf::Color::White);
    platformShape.setOutlineThickness(3.0f);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    platformA->addComponent(Component::Type::SPRITE, std::make_shared<CSprite>(platformShape));
    platformA->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    platformA->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platformA->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());
}