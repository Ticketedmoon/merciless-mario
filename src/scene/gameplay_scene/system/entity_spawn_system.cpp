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
        if (cAction->isShooting)
        {
            std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(
                    player->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(
                    player->getComponentByType(Component::Type::SPRITE_GROUP));
            std::shared_ptr<CCursorFollower> cCursorFollower = std::static_pointer_cast<CCursorFollower>(
                    player->getComponentByType(Component::Type::CURSOR_FOLLOWER));

            // Spawn bullet at player location or slightly in-front of sprite
            // moving to mouse destination (use cos X, sin Y)
            std::shared_ptr<sf::Sprite>& arm = cSpriteGroup->sprites.at(1);
            float shotAngleRadians = cCursorFollower->getArmPointAngleRadians(arm->getPosition());
            float shotAngleX = std::cos(shotAngleRadians);
            float shotAngleY = std::sin(shotAngleRadians);
            float shotSpeed = PLAYER_BULLET_SPEED * DT;

            // Spawn bullet at end of player's arm
            float armLength = arm->getGlobalBounds().width > arm->getGlobalBounds().height
                    ? arm->getGlobalBounds().width
                    : arm->getGlobalBounds().height;
            sf::Vector2f bulletPosition{cTransform->m_position.x + (shotAngleX * armLength),
                                        cTransform->m_position.y + (shotAngleY * armLength)};

            sf::Vector2f velocity = sf::Vector2f(shotAngleX * shotSpeed, shotAngleY * shotSpeed);

            float shotAngleDegrees = cCursorFollower->getArmPointAngleDegrees(arm->getPosition());
            createBullet(bulletPosition, velocity, shotAngleDegrees - 90);

            // Reset shooting flag
            cAction->isShooting = false;
        }
    }
}

void EntitySpawnSystem::createPlayer(sf::Vector2f position, bool isCollidable)
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    std::shared_ptr<CSpriteGroup> spriteGroup = std::make_shared<CSpriteGroup>();

    // Body
    const std::string bodyTextureFilePath = "resources/assets/texture/mario_spritesheet_smb3.png";
    sf::IntRect bodyRectBounds = sf::IntRect(35, 0, 35, 30);
    const sf::Vector2<float>& origin = sf::Vector2f(bodyRectBounds.width / 2, bodyRectBounds.height / 2);
    addAnimationTextureComponent(spriteGroup, position, bodyTextureFilePath, bodyRectBounds, origin, 3, 35, 1.0f/12.0f, {1, 1}, 0);

    // Weapon
    const std::string shotgunTextureFilePath = "resources/assets/texture/shotgun.png";
    sf::IntRect armRectBounds = sf::IntRect(0, 0, 81, TILE_SIZE * 2);
    const sf::Vector2<float>& gunOrigin = sf::Vector2f(0, TILE_SIZE);
    addAnimationTextureComponent(spriteGroup, position, shotgunTextureFilePath, armRectBounds, gunOrigin, TILE_SIZE, 0, 0, {1, 1}, 0);

    player->addComponent(Component::Type::SPRITE_GROUP, spriteGroup);
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(
            500.0f, 150.0f, 400.0f, // movement
            1500.0f, -300.0f, // jump velocity
            400.0f, 600.0f)); // gravity
    player->addComponent(Component::Type::CURSOR_FOLLOWER, std::make_shared<CCursorFollower>());

    if (isCollidable)
    {
        player->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::createPlatform(sf::Vector2f position, bool isCollidable)
{
    std::shared_ptr<Entity> platform = m_entityManager.addEntity(Entity::Type::PLATFORM);

    sf::Vector2f velocity = sf::Vector2f(0, 0);
    platform->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platform->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/blocks.png";
    sf::Rect<int> rectBounds = sf::IntRect(64, 64, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, TILE_SIZE, 0, {1, 1}, 0);
    platform->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    if (isCollidable)
    {
        platform->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::createQuestionBlock(sf::Vector2f position, bool isCollidable)
{
    std::shared_ptr<Entity> platform = m_entityManager.addEntity(Entity::Type::QUESTION_BLOCK);

    sf::Vector2f velocity = sf::Vector2f(0, 0);
    platform->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platform->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/blocks.png";
    sf::Rect<int> rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 3, TILE_SIZE, 1.0f/3.5f, {1, 1}, 0);
    platform->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    if (isCollidable)
    {
        platform->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::createBullet(sf::Vector2f bulletPosition, sf::Vector2f velocity, float gunAngle)
{
    std::shared_ptr<Entity>& bullet = m_entityManager.addEntity(Entity::Type::BULLET);

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/bullet_32x32.png";
    sf::Rect<int> rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);

    addAnimationTextureComponent(animationComponent, bulletPosition, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, TILE_SIZE, 0, {1, 1}, gunAngle);
    bullet->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    bullet->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(bulletPosition, velocity));
    bullet->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    bullet->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>());
    bullet->addComponent(Component::Type::LIFESPAN, std::make_shared<CLifespan>(255));
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
        sf::Vector2f position = sf::Vector2f(WINDOW_WIDTH + (row.locationX * TILE_SIZE),
                WINDOW_HEIGHT - (row.locationY * TILE_SIZE));
        if (row.entityType == "PLAYER")
        {
            createPlayer(position, row.isCollidable);
        }
        else if (row.entityType == "TILE")
        {
            if (row.animation == "GROUND")
            {
                // Apply 'GROUND' Animation
                // ---
                // Add animation component, keep sf::Texture and sf::Sprite managed in here.
                // Add texture to IntRect
                //  > contains appropriate texture with animation.
                //  > Remember we can have single-frame animations, e.g., so basically an unchanging texture.
                // create animation system.
                // For all entities with an animation component, update animation frame by 1.
                // If reach end, reset the animation.
                // Allow looping and non-looping animations.
                createPlatform(position, row.isCollidable);
            }
            if (row.animation == "QUESTION_BLOCK")
            {
                createQuestionBlock(position, row.isCollidable);
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
        file >> row.entityType >> row.animation >> row.locationX >> row.locationY >> row.isCollidable;
        rows.push_back(row);
    }
    return rows;
}