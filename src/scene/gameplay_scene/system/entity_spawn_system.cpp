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

    for (std::shared_ptr<Entity>& player: players)
    {
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                player->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CWeapon> cWeapon = std::static_pointer_cast<CWeapon>(
                player->getComponentByType(Component::Type::WEAPON));
        if (cAction->isShooting && cWeapon->hasBulletsInRound())
        {
            std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(
                    player->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(
                    player->getComponentByType(Component::Type::SPRITE_GROUP));
            // Spawn bullet at player location or slightly in-front of sprite
            // moving to mouse destination (use cos X, sin Y)
            std::shared_ptr<sf::Sprite>& weaponSprite = cSpriteGroup->sprites.at(1);
            float shotAngleRadians = cWeapon->getArmPointAngleRadians(weaponSprite->getPosition());
            float shotAngleX = std::cos(shotAngleRadians);
            float shotAngleY = std::sin(shotAngleRadians);
            float shotSpeed = PLAYER_BULLET_SPEED * DT;

            // Spawn bullet at end of player's weaponSprite
            float armLength = weaponSprite->getGlobalBounds().width > weaponSprite->getGlobalBounds().height
                    ? weaponSprite->getGlobalBounds().width
                    : weaponSprite->getGlobalBounds().height;
            sf::Vector2f bulletPosition{cTransform->m_position.x + (shotAngleX * armLength),
                                        cTransform->m_position.y + (shotAngleY * armLength)};

            sf::Vector2f velocity = sf::Vector2f(shotAngleX * shotSpeed, shotAngleY * shotSpeed);

            float shotAngleDegrees = cWeapon->getArmPointAngleDegrees(weaponSprite->getPosition());
            createBullet(bulletPosition, velocity, shotAngleDegrees - 90);
        }
    }
}

void EntitySpawnSystem::createPlayer(sf::Vector2f position)
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    std::shared_ptr<CSpriteGroup> spriteGroup = std::make_shared<CSpriteGroup>();

    const std::string bodyTextureFilePath = "resources/assets/texture/small_mario_sunglasses.png";
    sf::IntRect body = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);
    const sf::Vector2<float>& bodyOrigin = sf::Vector2f(body.width / 2, body.height / 2);
    addAnimationTextureComponent(spriteGroup, position, bodyTextureFilePath, body, bodyOrigin, 4, TILE_SIZE, 1.0f/10.0f, {1, 1}, 0);

    // Weapon
    const std::string shotgunTextureFilePath = "resources/assets/texture/shotgun.png";
    sf::IntRect armRectBounds = sf::IntRect(0, 0, 81, TILE_SIZE * 2);
    const sf::Vector2<float>& gunOrigin = sf::Vector2f(0, TILE_SIZE);
    addAnimationTextureComponent(spriteGroup, position, shotgunTextureFilePath, armRectBounds, gunOrigin, TILE_SIZE, 0, 0, {1, 1}, 0);

    player->addComponent(Component::Type::SPRITE_GROUP, spriteGroup);
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(
            500.0f, 250.0f, 350.0f, // movement
            1500.0f, -300.0f, // jump velocity
            400.0f, 600.0f)); // gravity

    uint16_t totalAmmoRounds = 3;
    player->addComponent(Component::Type::WEAPON, std::make_shared<CWeapon>(WeaponType::SHOTGUN,
            totalAmmoRounds));
    player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
}

void EntitySpawnSystem::createEnemy(sf::Vector2f position, Entity::Type entityType, const std::string& animationTextureFilePath)
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(entityType);

    sf::Vector2f velocity = sf::Vector2f(60.0f, 0);

    std::shared_ptr<CSpriteGroup> spriteGroup = std::make_shared<CSpriteGroup>();

    sf::IntRect boundingRect = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);
    const sf::Vector2<float>& origin = sf::Vector2f(boundingRect.width / 2, boundingRect.height / 2);
    addAnimationTextureComponent(spriteGroup, position, animationTextureFilePath, boundingRect, origin, 2, TILE_SIZE, 1.0f/3.0f, {1, 1}, 0);

    player->addComponent(Component::Type::SPRITE_GROUP, spriteGroup);
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(
            500.0f, 250.0f, 350.0f, // movement
            0.0f, 0.0f, // jump velocity
            400.0f, 600.0f)); // gravity
    player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
}

void EntitySpawnSystem::createBricks(sf::Vector2f position)
{
    std::shared_ptr<Entity> brick = m_entityManager.addEntity(Entity::Type::BRICK);

    sf::Vector2f velocity = sf::Vector2f(0, 0);
    brick->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    brick->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/blocks.png";
    sf::Rect<int> rectBounds = sf::IntRect(TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, TILE_SIZE, 0, {1, 1}, 0);
    brick->addComponent(Component::Type::SPRITE_GROUP, animationComponent);
    brick->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
}

void EntitySpawnSystem::createLevelDecoration(sf::Vector2f position, Entity::Type entityType, const LevelSprite& levelSprite)
{
    std::shared_ptr<Entity> decoration = m_entityManager.addEntity(entityType);
    applySpriteGroupForEntity(position, levelSprite, decoration);
}

void EntitySpawnSystem::createLevelCollidableSprite(sf::Vector2f position, Entity::Type entityType, const LevelSprite& levelSprite)
{
    std::shared_ptr<Entity> collidableEntity = m_entityManager.addEntity(entityType);
    applySpriteGroupForEntity(position, levelSprite, collidableEntity);
    collidableEntity->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    collidableEntity->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());
}

void EntitySpawnSystem::createQuestionBlock(sf::Vector2f position)
{
    std::shared_ptr<Entity> questionBlock = m_entityManager.addEntity(Entity::Type::QUESTION_BLOCK);

    sf::Vector2f velocity = sf::Vector2f(0, 0);
    questionBlock->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    questionBlock->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/blocks.png";
    sf::Rect<int> rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 3, TILE_SIZE, 1.0f/3.5f, {1, 1}, 0);
    questionBlock->addComponent(Component::Type::SPRITE_GROUP, animationComponent);
    questionBlock->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    questionBlock->addComponent(Component::Type::INTERACTABLE, std::make_shared<CInteractable>());
}

void EntitySpawnSystem::createGroundBlock(sf::Vector2f position)
{
    std::shared_ptr<Entity> ground = m_entityManager.addEntity(Entity::Type::GROUND);

    sf::Vector2f velocity = sf::Vector2f(0, 0);
    ground->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    ground->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/blocks.png";
    sf::Rect<int> rectBounds = sf::IntRect(0, 32, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, TILE_SIZE, 0, {1, 1}, 0);
    ground->addComponent(Component::Type::SPRITE_GROUP, animationComponent);
    ground->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
}

void EntitySpawnSystem::createBullet(sf::Vector2f bulletPosition, sf::Vector2f velocity, float gunAngle)
{
    std::shared_ptr<Entity>& bullet = m_entityManager.addEntity(Entity::Type::BULLET);

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/bullet_32x32.png";
    sf::Rect<int> rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, bulletPosition, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, TILE_SIZE, 0, {0.5, 0.5}, gunAngle);
    bullet->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    bullet->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(bulletPosition, velocity));
    bullet->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    bullet->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>());
    bullet->addComponent(Component::Type::LIFESPAN, std::make_shared<CLifespan>(255));
}

void EntitySpawnSystem::applySpriteGroupForEntity(const sf::Vector2f& position,
        const EntitySpawnSystem::LevelSprite& levelSprite, std::shared_ptr<Entity>& decoration)
{
    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    std::string decorationSizeString = levelSprite.sizeType == LevelSpriteSizeType::SMALL
            ? "small"
            : levelSprite.sizeType == LevelSpriteSizeType::MEDIUM
                    ? "mid"
                    : "large";
    const std::string& animationTextureFilePath = "resources/assets/texture/mario_1_1_" + levelSprite.type + "_" +
            decorationSizeString + ".png";

    sf::Rect<int> rectBounds = sf::IntRect(0, 0, levelSprite.sizeValueX * TILE_SIZE, levelSprite.sizeValueY * TILE_SIZE);

    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, 0, 0, {1, 1}, 0);
    decoration->addComponent(Component::SPRITE_GROUP, animationComponent);
}

void EntitySpawnSystem::addAnimationTextureComponent(std::shared_ptr<CSpriteGroup>& spriteGroup,
        const sf::Vector2f& position, const std::string& animationTextureFilePath, sf::IntRect& rectBounds,
        const sf::Vector2f origin, const uint32_t totalAnimationFrames, const uint32_t animationIncrement,
        float spriteAnimationCompletionTime, sf::Vector2f scale, float rotation)
{
    CSpriteGroup::SpriteAnimation spriteAnimation{rectBounds, rectBounds, animationIncrement, totalAnimationFrames, 0, {0, spriteAnimationCompletionTime}};
    spriteGroup->animations.emplace_back(std::make_shared<CSpriteGroup::SpriteAnimation>(spriteAnimation));

    std::shared_ptr<sf::Texture> spriteTexture = buildSpriteTexture(spriteGroup, animationTextureFilePath);
    spriteGroup->textures.emplace_back(spriteTexture);

    sf::Sprite sprite;
    sf::Texture& texture = *m_textureManager.getTexture(animationTextureFilePath);
    sprite.setTexture(texture);
    sprite.setTextureRect(spriteGroup->animations.back()->animationRectBounds);
    sprite.setPosition(position);
    sprite.setOrigin(origin);
    sprite.rotate(rotation);
    sprite.setScale(scale);

    spriteGroup->sprites.emplace_back(std::make_shared<sf::Sprite>(sprite));
}

std::shared_ptr<sf::Texture> EntitySpawnSystem::buildSpriteTexture(std::shared_ptr<CSpriteGroup>& spriteGroup,
        const std::string& animationTextureFilePath)
{
    std::shared_ptr<sf::Texture> animationTexture = std::make_shared<sf::Texture>();
    if (m_textureManager.hasTexture(animationTextureFilePath))
    {
        animationTexture = m_textureManager.getTexture(animationTextureFilePath);
    }
    else
    {
        bool isTextureLoaded = animationTexture->loadFromFile(animationTextureFilePath,
                spriteGroup->animations.back()->animationRectBounds);
        assert(isTextureLoaded);
        m_textureManager.addTexture(animationTextureFilePath, animationTextureFilePath);
    }

    animationTexture->setSmooth(true); // TODO Experiment with this
    return animationTexture;
}

void EntitySpawnSystem::createLevel()
{
    std::vector<Row> levelRows = LoadLevelData(1);
    for (const auto& row: levelRows)
    {
        sf::Vector2f position = sf::Vector2f(row.locationX * TILE_SIZE, WINDOW_HEIGHT - (row.locationY * TILE_SIZE));
        if (row.entityType == "PLAYER")
        {
            createPlayer(position);
        }
        else if (row.entityType == "ENEMY")
        {
            if (row.animation == "GOOMBA")
            {
                const std::string animationTextureFilePath = "resources/assets/texture/goomba_spritesheet.png";
                createEnemy(position, Entity::Type::ENEMY_GOOMBA, animationTextureFilePath);
            }
        }
        else if (row.entityType == "TILE")
        {
            if (row.animation == "GROUND")
            {
                createGroundBlock(position);
            }
            if (row.animation == "BRICK")
            {
                createBricks(position);
            }
            if (row.animation == "QUESTION_BLOCK")
            {
                createQuestionBlock(position);
            }
            if (row.animation == "PIPE_SMALL")
            {
                createLevelCollidableSprite(position, Entity::Type::PIPE, {"pipe", LevelSpriteSizeType::SMALL, 2, 3});
            }
            if (row.animation == "PIPE_MEDIUM")
            {
                createLevelCollidableSprite(position, Entity::Type::PIPE, {"pipe", LevelSpriteSizeType::MEDIUM, 2, 5});
            }
            if (row.animation == "PIPE_LARGE")
            {
                createLevelCollidableSprite(position, Entity::Type::PIPE, {"pipe", LevelSpriteSizeType::LARGE, 2, 7});
            }
        }
        else if (row.entityType == "DECORATION")
        {
            // Bush
            if (row.animation == "BUSH_SMALL")
            {
                createLevelDecoration(position, Entity::Type::BUSH, {"bush", LevelSpriteSizeType::SMALL, 3, 1});
            }
            if (row.animation == "BUSH_MID")
            {
                createLevelDecoration(position, Entity::Type::BUSH, {"bush", LevelSpriteSizeType::MEDIUM, 4, 1});
            }
            if (row.animation == "BUSH_BIG")
            {
                createLevelDecoration(position, Entity::Type::BUSH, {"bush", LevelSpriteSizeType::LARGE, 5, 1});
            }

            // Hill
            if (row.animation == "HILL_SMALL")
            {
                createLevelDecoration(position, Entity::Type::HILL, {"hill", LevelSpriteSizeType::SMALL, 5, 3});
            }
            if (row.animation == "HILL_LARGE")
            {
                createLevelDecoration(position, Entity::Type::HILL, {"hill", LevelSpriteSizeType::LARGE, 5, 3});
            }

            // Cloud
            if (row.animation == "CLOUD_SMALL")
            {
                createLevelDecoration(position, Entity::Type::CLOUD, {"cloud", LevelSpriteSizeType::SMALL, 3, 2});
            }
            if (row.animation == "CLOUD_MID")
            {
                createLevelDecoration(position, Entity::Type::CLOUD, {"cloud", LevelSpriteSizeType::MEDIUM, 4, 2});
            }
            if (row.animation == "CLOUD_LARGE")
            {
                createLevelDecoration(position, Entity::Type::CLOUD, {"cloud", LevelSpriteSizeType::LARGE, 5, 2});
            }
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
        file >> row.entityType >> row.animation >> row.locationX >> row.locationY;
        rows.push_back(row);
    }
    return rows;
}