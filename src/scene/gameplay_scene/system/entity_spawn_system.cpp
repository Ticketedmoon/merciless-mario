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
            sf::Sprite& arm = cSpriteGroup->animationSprites.at(1);
            float shotAngle = cCursorFollower->getArmPointAngleRadians(arm.getPosition());
            float shotAngleX = std::cos(shotAngle);
            float shotAngleY = std::sin(shotAngle);
            float shotSpeed = PLAYER_BULLET_SPEED * DT;

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

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/brick.png";
    const sf::Rect<int>& rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);
    addAnimationTextureComponent(animationComponent, bulletPosition, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width/2, rectBounds.height/2), 1);
    bullet->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    bullet->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(bulletPosition, velocity));
    //bullet->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    bullet->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>());
    bullet->addComponent(Component::Type::LIFESPAN, std::make_shared<CLifespan>(255));
}

void EntitySpawnSystem::createPlayer(sf::Vector2f position, bool isCollidable)
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    sf::Vector2f velocity = sf::Vector2f(0, 0);

    std::shared_ptr<CSpriteGroup> spriteGroup = std::make_shared<CSpriteGroup>();

    // Body
    const std::string bodyTextureFilePath = "resources/assets/texture/brick.png";
    const sf::Rect<int>& bodyRectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);
    addAnimationTextureComponent(spriteGroup, position, bodyTextureFilePath, bodyRectBounds,
            sf::Vector2f(bodyRectBounds.width/2, bodyRectBounds.height/2), 1);

    // Arm
    const std::string shotgunTextureFilePath = "resources/assets/texture/shotgun.png";
    const sf::Rect<int>& armRectBounds = sf::IntRect(0, 0, 81, TILE_SIZE * 2);
    const sf::Vector2<float>& gunOrigin = sf::Vector2f(0, TILE_SIZE);
    addAnimationTextureComponent(spriteGroup, position, shotgunTextureFilePath, armRectBounds, gunOrigin, 1);

    player->addComponent(Component::Type::SPRITE_GROUP, spriteGroup);
    player->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    player->addComponent(Component::Type::USER_INPUT, std::make_shared<CAction>());
    player->addComponent(Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(
            500.0f, 150.0f, 1000.0f, // movement
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
    const std::string animationTextureFilePath = "resources/assets/texture/brick.png";
    const sf::Rect<int>& rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);
    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width/2, rectBounds.height/2), 1);
    platform->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    if (isCollidable)
    {
        platform->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::createQuestionBlock(sf::Vector2f position, bool isCollidable)
{
    std::shared_ptr<Entity> platform = m_entityManager.addEntity(Entity::Type::PLATFORM);

    sf::Vector2f velocity = sf::Vector2f(0, 0);
    platform->addComponent(Component::Type::TRANSFORM, std::make_shared<CTransform>(position, velocity));
    platform->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());

    std::shared_ptr<CSpriteGroup> animationComponent = std::make_shared<CSpriteGroup>();
    const std::string animationTextureFilePath = "resources/assets/texture/question_block.png";
    const sf::Rect<int>& rectBounds = sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE);
    addAnimationTextureComponent(animationComponent, position, animationTextureFilePath, rectBounds,
            sf::Vector2f(rectBounds.width/2, rectBounds.height/2), 3);
    platform->addComponent(Component::Type::SPRITE_GROUP, animationComponent);

    if (isCollidable)
    {
        platform->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    }
}

void EntitySpawnSystem::addAnimationTextureComponent(std::shared_ptr<CSpriteGroup>& spriteGroup,
        const sf::Vector2f& position, const std::string& animationTextureFilePath, const sf::IntRect rectBounds,
        sf::Vector2f origin, uint32_t totalAnimationFrames)
{
    spriteGroup->animationRectBoundsGroup.emplace_back(rectBounds);
    spriteGroup->totalAnimationFramesGroup.emplace_back(totalAnimationFrames);
    spriteGroup->currentFrameGroup.emplace_back(0);

    std::shared_ptr<sf::Texture> animationTexture = buildSpriteTexture(spriteGroup, animationTextureFilePath);
    spriteGroup->animationTextures.emplace_back(animationTexture);

    sf::Sprite animationSprite;
    sf::Texture& texture = *m_textureManager.getTexture(animationTextureFilePath);
    animationSprite.setTexture(texture);
    animationSprite.setTextureRect(spriteGroup->animationRectBoundsGroup.back());
    animationSprite.setPosition(position);
    animationSprite.setOrigin(origin);

    spriteGroup->animationSprites.emplace_back(animationSprite);
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
        bool isTextureLoaded = animationTexture->loadFromFile(animationTextureFilePath, spriteGroup->animationRectBoundsGroup.back());
        assert(isTextureLoaded);
        m_textureManager.addTexture(animationTextureFilePath, animationTextureFilePath);
    }

    animationTexture->setSmooth(true); // TODO Experiment with this
    return animationTexture;
}

void EntitySpawnSystem::createLevel()
{
    std::vector<Row> levelRows = LoadLevelData(1);
    for (const auto& row : levelRows)
    {
        sf::Vector2f position = sf::Vector2f(WINDOW_WIDTH + (row.locationX * TILE_SIZE), WINDOW_HEIGHT - (row.locationY * TILE_SIZE));
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