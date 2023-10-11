#include "entity_spawn_system.h"
#include "common_constants.h"

EntitySpawnSystem::EntitySpawnSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
    createPlayer();
    createPlatform(sf::Vector2f(10, 100), sf::Vector2f(500, 1200), sf::Color::Green);
    createPlatform(sf::Vector2f(500, 100), sf::Vector2f(300, 1250), sf::Color::Green);
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

void EntitySpawnSystem::execute()
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        createPlayer();
    }
}

void EntitySpawnSystem::createPlayer()
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::Vector2f position = sf::Vector2f(100, 1000);
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    sf::RectangleShape playerBody{sf::Vector2f(50, 50)};
    playerBody.setOrigin(25, 25);
    playerBody.setFillColor(sf::Color::Yellow);
    playerBody.setOutlineColor(sf::Color::White);
    playerBody.setOutlineThickness(2.0f);

    sf::RectangleShape playerArm{sf::Vector2f(150, 10)};
    playerArm.setOrigin(75, 5);
    playerArm.setFillColor(sf::Color::Yellow);
    playerArm.setOutlineColor(sf::Color::White);
    playerArm.setOutlineThickness(2.0f);

    std::vector<sf::RectangleShape> playerComponents = {playerBody, playerArm};
    player->addComponent(Component::Type::SPRITE_GROUP, std::make_shared<CSpriteGroup>(playerComponents));
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(0.125f, 0.01f, 5.95f, 1.25f, -10.0f, 0.3f, 25.0f));
}

void EntitySpawnSystem::createPlatform(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor)
{
    std::shared_ptr<Entity> platformA = m_entityManager.addEntity(Entity::Type::PLATFORM);

    sf::RectangleShape platformShape(size);
    platformShape.setOrigin(size.x/2, size.y/2);
    platformShape.setPosition(position);
    platformShape.setFillColor(fillColor);
    platformShape.setOutlineColor(sf::Color::White);
    platformShape.setOutlineThickness(3.0f);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    platformA->addComponent(Component::Type::SPRITE_GROUP, std::make_shared<CSpriteGroup>(platformShape));
    platformA->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    platformA->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platformA->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());
}
