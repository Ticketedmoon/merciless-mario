#include <iostream>
#include "entity_spawn_system.h"

struct Row
{
    Entity::Type entityType;
    std::string locationX;
    std::string locationY;
    std::string isCollidable;
};

EntitySpawnSystem::EntitySpawnSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
    // Load Csv
    // Iterate over all rows
    // Extract out properties of each row
    // Based on ENTITY_TYPE
    // Spawn appropriate entity at appropriate location
    // Future properties will determine if the entity can collide, or how at what velocity the entity moves.
    std::ifstream file("resources/level/level_1.txt", std::ifstream::in);
    assert(file.is_open());

    int rowNo = 0;
    std::vector<Row> rows;
    while (!file.eof())
    {
        Row row{};
        file >> row.entityType >> row.locationX >> row.locationY >> row.isCollidable;

        if (rowNo > 0)
        {
            std::cout << row.entityType << ", " << row.locationX << ", " << row.locationY << ", " << row.isCollidable << '\n';
            rows.push_back(row);
        }
        rowNo++;
    }

    createPlayer();
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(96, 1200), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(500, 1200), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(300, 1250), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(500, 1100), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(900, 1420), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(1200, 1520), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(1500, 1320), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2000, 1220), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2400, 1020), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2700, 950), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2500, 750), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2800, 650), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(3100, 630), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2600, 620), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(2200, 610), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(1800, 580), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(1200, 560), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(700, 560), sf::Color::Green);
    createPlatform(sf::Vector2f(32, 32), sf::Vector2f(300, 500), sf::Color::Blue);
}

void EntitySpawnSystem::execute()
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        createPlayer();
    }

    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(player->getComponentByType(Component::Type::USER_INPUT));
        if (cAction->isShooting)
        {
            std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(player->getComponentByType(Component::Type::SPRITE_GROUP));

            // Spawn bullet at player location or slightly in-front of sprite
            // moving to mouse destination (use cos X, sin Y)
            sf::RectangleShape arm = cSpriteGroup->getSprites().at(1);
            float shotAngle = cAction->getArmPointAngleRadians(arm.getPosition());
            float shotAngleX = std::cos(shotAngle);
            float shotAngleY = std::sin(shotAngle);
            float shotSpeed = 10.0f;

            // Spawn bullet at end of player's arm
            float armLength = arm.getGlobalBounds().width > arm.getGlobalBounds().height
                    ? arm.getGlobalBounds().width
                    : arm.getGlobalBounds().height;
            sf::Vector2f bulletPosition{cTransform->m_position.x + (shotAngleX * armLength),
                                        cTransform->m_position.y + (shotAngleY * armLength)};
            sf::Vector2f velocity = sf::Vector2f(shotAngleX * shotSpeed, shotAngleY * shotSpeed);
            createBullet(bulletPosition, velocity);

            // Reset shooting flag
            cAction->isShooting = false;
        }
    }
}

void EntitySpawnSystem::createBullet(sf::Vector2f bulletPosition, sf::Vector2f velocity) const
{
    std::shared_ptr<Entity>& bullet = m_entityManager.addEntity(Entity::Type::BULLET);

    sf::RectangleShape bulletShape{sf::Vector2f(10, 10)};
    bulletShape.setOrigin(bulletShape.getLocalBounds().width/2, bulletShape.getLocalBounds().height/2);
    bulletShape.setFillColor(sf::Color::White);
    bulletShape.setOutlineColor(sf::Color::Blue);
    bulletShape.setOutlineThickness(2.0f);

    bullet->addComponent(Component::Type::SPRITE_GROUP, std::make_shared<CSpriteGroup>(bulletShape));
    bullet->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(bulletPosition, velocity));
    //bullet->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    bullet->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>());
    bullet->addComponent(Component::Type::LIFESPAN, std::make_shared<CLifespan>(255));
}

void EntitySpawnSystem::createPlayer()
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::Vector2f position = sf::Vector2f(100, 1000);
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    sf::RectangleShape playerBody{sf::Vector2f(32, 32)};
    playerBody.setOrigin(playerBody.getLocalBounds().width/2, playerBody.getLocalBounds().height/2);
    playerBody.setFillColor(sf::Color::Yellow);
    playerBody.setOutlineColor(sf::Color::White);
    playerBody.setOutlineThickness(2.0f);

    sf::RectangleShape playerArm{sf::Vector2f(60, 10)};
    playerArm.setOrigin(playerArm.getLocalBounds().width, playerArm.getLocalBounds().height/2);
    playerArm.setFillColor(sf::Color::Black);
    playerArm.setOutlineColor(sf::Color::White);
    playerArm.setOutlineThickness(2.0f);

    std::vector<sf::RectangleShape> playerComponents = {playerBody, playerArm};
    player->addComponent(Component::Type::SPRITE_GROUP, std::make_shared<CSpriteGroup>(playerComponents));
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(0.125f, 0.01f, 10.95f, 1.25f, -10.0f, 0.3f, 25.0f));
}

void EntitySpawnSystem::createPlatform(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor)
{
    std::shared_ptr<Entity> platform = m_entityManager.addEntity(Entity::Type::PLATFORM);

    sf::RectangleShape platformShape(size);
    platformShape.setOrigin(size.x/2, size.y/2);
    platformShape.setPosition(position);
    platformShape.setFillColor(fillColor);
    platformShape.setOutlineColor(sf::Color::White);
    platformShape.setOutlineThickness(3.0f);
    platformShape.scale(2.0f, 2.0f);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    platform->addComponent(Component::Type::SPRITE_GROUP, std::make_shared<CSpriteGroup>(platformShape));
    platform->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    platform->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platform->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());
}
