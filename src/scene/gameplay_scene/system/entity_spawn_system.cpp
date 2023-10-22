#include "entity_spawn_system.h"

EntitySpawnSystem::EntitySpawnSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
    createLevel();
}

void EntitySpawnSystem::execute()
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        m_entityManager.destroyAllEntities();
        createLevel();
        return;
    }

    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(player->getComponentByType(Component::Type::USER_INPUT));
        if (cAction->isShooting)
        {
            std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(player->getComponentByType(Component::Type::SPRITE_GROUP));
            std::shared_ptr<CCursorFollower> cCursorFollower = std::static_pointer_cast<CCursorFollower>(player->getComponentByType(Component::Type::CURSOR_FOLLOWER));

            // Spawn bullet at player location or slightly in-front of sprite
            // moving to mouse destination (use cos X, sin Y)
            sf::RectangleShape arm = cSpriteGroup->getSprites().at(1);
            float shotAngle = cCursorFollower->getArmPointAngleRadians(arm.getPosition());
            float shotAngleX = std::cos(shotAngle);
            float shotAngleY = std::sin(shotAngle);
            float shotSpeed = 20.0f;

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

void EntitySpawnSystem::createBullet(sf::Vector2f bulletPosition, sf::Vector2f velocity)
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

void EntitySpawnSystem::createPlayer(sf::Vector2f size, sf::Vector2f position, bool isCollidable)
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    sf::RectangleShape playerBody(size);
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
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(0.125f, 0.01f, 10.95f, 1.25f, -12.5f, 0.5f, 7.5f));
    player->addComponent(Component::Type::CURSOR_FOLLOWER, std::make_shared<CCursorFollower>());

    if (isCollidable)
    {
        player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::createPlatform(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor, bool isCollidable)
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
    platform->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platform->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    if (isCollidable)
    {
        platform->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::createLevel()
{
    std::vector<Row> levelRows = LoadLevelData(1);
    for (const auto& row : levelRows)
    {
        sf::Vector2f position = sf::Vector2f(row.locationX, row.locationY);
        if (row.entityType == "PLAYER")
        {
            createPlayer(ENTITY_SIZE, position, row.isCollidable);
        }
        else if (row.entityType == "PLATFORM")
        {
            createPlatform(ENTITY_SIZE, position, sf::Color::Green, row.isCollidable);
        }
    }
}

std::vector<EntitySpawnSystem::Row> EntitySpawnSystem::LoadLevelData(uint8_t levelNumber)
{
    const std::string path = "resources/level/level_" + std::to_string(levelNumber) + ".txt";
    std::ifstream file(path, std::ifstream::in);
    assert(file.is_open());

    std::vector<Row> rows;
    std::string line;

    while (getline(file, line))
    {
        Row row{};
        file >> row.entityType >> row.locationX >> row.locationY >> row.isCollidable;
        rows.push_back(row);
    }
    return rows;
}