#include "entity_spawn_system.h"

EntitySpawnSystem::EntitySpawnSystem(EntityManager& entityManager, const std::string& levelName)
    : m_entityManager(entityManager), m_levelName(levelName)
{
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

    std::vector<std::shared_ptr<Entity>>& questionBlocks = m_entityManager.getEntitiesByType(Entity::Type::QUESTION_BLOCK);
    for (std::shared_ptr<Entity>& questionBlock: questionBlocks)
    {
        std::shared_ptr<CWeapon> cWeapon = std::static_pointer_cast<CWeapon>(
                questionBlock->getComponentByType(Component::Type::WEAPON));

        if (cWeapon != nullptr && cWeapon->shouldSpawnWeapon)
        {
            if (cWeapon->weaponType == WeaponType::PISTOL)
            {
                sf::IntRect entityRect{0, 0, TILE_SIZE, TILE_SIZE};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{0.0f, 0.0f};
                const EntityProperties& properties{Entity::Type::WEAPON_PISTOL, cWeapon->weaponSpawnLocation, velocity, {
                        {"resources/assets/texture/pistol_32x32.png", entityRect, entityRect, TILE_SIZE, 0, 0, {0, 0}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()}
                };

                createEntity(properties, components);

            }
            else if (cWeapon->weaponType == WeaponType::SHOTGUN)
            {
                sf::IntRect entityRect{0, 0, 81, TILE_SIZE*2};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{0.0f, 0.0f};
                const EntityProperties& properties{Entity::Type::WEAPON_SHOTGUN, cWeapon->weaponSpawnLocation, velocity, {
                        {"resources/assets/texture/shotgun.png", entityRect, entityRect, TILE_SIZE, 0, 0, {0, 0}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()}
                };

                createEntity(properties, components);
            }

            cWeapon->shouldSpawnWeapon = false;
        }
    }


    for (std::shared_ptr<Entity>& player: players)
    {
        std::shared_ptr<CWeapon> cWeapon = std::static_pointer_cast<CWeapon>(
                player->getComponentByType(Component::Type::WEAPON));
        std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(
                player->getComponentByType(Component::Type::SPRITE_GROUP));
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(
                player->getComponentByType(Component::Type::TRANSFORM));
        if (cWeapon != nullptr)
        {
            // Collided with
            if (cWeapon->hasWeaponBeenCollidedWith)
            {

                if (cSpriteGroup->animations.size() > 1)
                {
                    removeLastAnimationTexture(cSpriteGroup);
                }

                m_audioManager->playSound(AudioManager::AudioType::POWER_UP_APPEARS, DEFAULT_SFX_VOLUME);

                // Pick up weapon.
                if (cWeapon->weaponType == WeaponType::PISTOL)
                {
                    sf::IntRect entityGunRect{0, 0, TILE_SIZE, TILE_SIZE};
                    sf::Vector2f gunOrigin{0, TILE_SIZE / 2};
                    const std::string weaponTexturePath = "resources/assets/texture/pistol_32x32.png";
                    addAnimationTextureComponent(cSpriteGroup, weaponTexturePath, cTransform->m_position, entityGunRect,
                            gunOrigin, 1, TILE_SIZE, 0, 0);
                }
                else if (cWeapon->weaponType == WeaponType::SHOTGUN)
                {
                    sf::IntRect entityGunRect{0, 0, 81, TILE_SIZE * 2};
                    sf::Vector2f gunOrigin{0, TILE_SIZE};
                    const std::string weaponTexturePath = "resources/assets/texture/shotgun.png";
                    addAnimationTextureComponent(cSpriteGroup, weaponTexturePath, cTransform->m_position, entityGunRect,
                            gunOrigin, 1, TILE_SIZE, 0, 0);
                }
                else if (cWeapon->weaponType == WeaponType::SNIPER_RIFLE)
                {
                    // NOT IMPLEMENTED
                }
                cWeapon->hasWeaponBeenCollidedWith = false;
            }
        }

        // Other Actions
        std::shared_ptr<CAction> cAction = std::static_pointer_cast<CAction>(
                player->getComponentByType(Component::Type::USER_INPUT));
        if (cAction->isShooting && cWeapon != nullptr && cWeapon->hasBulletsInRound())
        {
            // Spawn bullet at player location or slightly in-front of sprite
            // moving to mouse destination (use cos X, sin Y)
            std::shared_ptr<sf::Sprite>& weaponSprite = cSpriteGroup->sprites.at(1);
            float shotAngleRadians = cWeapon->getArmPointAngleRadians(weaponSprite->getPosition());
            float shotAngleX = std::cos(shotAngleRadians);
            float shotAngleY = std::sin(shotAngleRadians);

            // Spawn bullet at end of player's weaponSprite
            float armLength = weaponSprite->getGlobalBounds().width > weaponSprite->getGlobalBounds().height
                    ? weaponSprite->getGlobalBounds().width
                    : weaponSprite->getGlobalBounds().height;

            sf::Vector2f bulletPosition{cTransform->m_position.x + (shotAngleX * armLength),
                                        cTransform->m_position.y + (shotAngleY * armLength)};

            float shotAngleDegrees = cWeapon->getArmPointAngleDegrees(weaponSprite->getPosition());
            float gunAngle = shotAngleDegrees - 90;

            sf::IntRect entityRect{0, 0, TILE_SIZE/2, TILE_SIZE/2};
            sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};

            float shotSpeed = PLAYER_BULLET_SPEED * DT;
            sf::Vector2f velocity = sf::Vector2f(shotAngleX * shotSpeed, shotAngleY * shotSpeed);
            EntityProperties properties{Entity::Type::BULLET, bulletPosition, velocity, {
                    {"resources/assets/texture/bullet_16x16.png", entityRect, entityRect, TILE_SIZE, 0, 1, {0, 1.0f/3.5f}, origin, gunAngle},
            }};

            if (cWeapon->weaponType == WeaponType::PISTOL || cWeapon->weaponType == WeaponType::SHOTGUN)
            {
                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()},
                        {Component::Type::LIFESPAN, std::make_shared<CLifespan>(255)}
                };
                createEntity(properties, components);
            }
        }
    }
}

void EntitySpawnSystem::createLevel()
{
    std::vector<Row> levelRows = LoadLevelData();

    for (const auto& row: levelRows)
    {
        sf::Vector2f position = sf::Vector2f(row.locationX * TILE_SIZE, WINDOW_HEIGHT - (row.locationY * TILE_SIZE));
        if (row.entityType == "PLAYER")
        {
            // TODO could be issue here we are not passing by value.
            sf::IntRect entityBodyRect{0, 0, TILE_SIZE, TILE_SIZE};
            sf::Vector2f bodyOrigin{entityBodyRect.width/2.0f, entityBodyRect.height/2.0f};

            const EntityProperties& properties{Entity::Type::PLAYER, position, {0, 0}, {
                    {"resources/assets/texture/small_mario_sunglasses.png", entityBodyRect, entityBodyRect, TILE_SIZE, 0, 4, {0, 1.0f/10.0f}, bodyOrigin, 0}
            }};

            std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                    {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                    {Component::Type::USER_INPUT, std::make_shared<CAction>()},
                    {Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(
                            500.0f, 250.0f, 350.0f, // movement
                            1500.0f, -300.0f, // jump velocity
                            400.0f, 600.0f) // gravity
                    },
                    {Component::Type::COLLISION, std::make_shared<CCollision>()}
            };

            createEntity(properties, components);
        }
        else if (row.entityType == "ENEMY")
        {
            if (row.animation == "GOOMBA")
            {
                sf::IntRect entityRect{0, 0, TILE_SIZE, TILE_SIZE};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{60.0f, 0};
                const EntityProperties& properties{Entity::Type::ENEMY_GOOMBA, position, velocity, {
                        {"resources/assets/texture/goomba_spritesheet.png", entityRect, entityRect, TILE_SIZE, 0, 2, {0, 1.0f/3.0f}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::DYNAMIC_MOVEMENT, std::make_shared<CMovement>(
                                500.0f, 250.0f, 350.0f, // movement
                                0.0f, 0.0f, // jump velocity
                                400.0f, 600.0f) // gravity
                        },
                        {Component::Type::COLLISION, std::make_shared<CCollision>()}
                };

                createEntity(properties, components);
            }
        }
        else if (row.entityType == "TILE")
        {
            if (row.animation == "GROUND")
            {
                sf::IntRect entityRect{0, TILE_SIZE, TILE_SIZE, TILE_SIZE};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{0.0f, 0};
                const EntityProperties& properties{Entity::Type::GROUND, position, velocity, {
                        {"resources/assets/texture/blocks.png", entityRect, entityRect, TILE_SIZE, 0, 1, {0, 0}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()}
                };

                createEntity(properties, components);
            }
            if (row.animation == "BRICK")
            {
                sf::IntRect entityRect{TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{0.0f, 0};
                const EntityProperties& properties{Entity::Type::BRICK, position, velocity, {
                        {"resources/assets/texture/blocks.png", entityRect, entityRect, TILE_SIZE, 0, 1, {0, 0}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()}
                };

                createEntity(properties, components);
            }
            if (row.animation == "MARBLE")
            {
                sf::IntRect entityRect{TILE_SIZE*2, TILE_SIZE, TILE_SIZE, TILE_SIZE};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{0.0f, 0};
                const EntityProperties& properties{Entity::Type::BRICK, position, velocity, {
                        {"resources/assets/texture/blocks.png", entityRect, entityRect, TILE_SIZE, 0, 1, {0, 0}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()}
                };

                createEntity(properties, components);
            }
            if (row.animation == "QUESTION_BLOCK")
            {
                sf::IntRect entityRect{0, 0, TILE_SIZE, TILE_SIZE};
                sf::Vector2f origin{entityRect.width/2.0f, entityRect.height/2.0f};
                sf::Vector2f velocity{0.0f, 0};
                const EntityProperties& properties{Entity::Type::QUESTION_BLOCK, position, velocity, {
                        {"resources/assets/texture/blocks.png", entityRect, entityRect, TILE_SIZE, 0, 3, {0, 1.0f/3.5f}, origin, 0},
                }};

                std::unordered_map<Component::Type, std::shared_ptr<Component>> components{
                        {Component::Type::TRANSFORM, std::make_shared<CTransform>(properties.position, properties.velocity)},
                        {Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>()},
                        {Component::Type::COLLISION, std::make_shared<CCollision>()},
                        {Component::Type::INTERACTABLE, std::make_shared<CInteractable>()}
                };

                if (row.item == "WEAPON_PISTOL")
                {
                    components.insert({
                        Component::Type::WEAPON, std::make_shared<CWeapon>(WeaponType::PISTOL, 5)
                    });
                }
                else if (row.item == "WEAPON_SHOTGUN")
                {
                    components.insert({
                            Component::Type::WEAPON, std::make_shared<CWeapon>(WeaponType::SHOTGUN, 3)
                    });
                }
                else if (row.item == "COIN")
                {
                    // NOT IMPLEMENTED
                }

                createEntity(properties, components);
            }
            if (row.animation.find("PIPE_") != std::string::npos)
            {
                const LevelSpriteSizeType decorationSizeInfo =
                        row.animation.find("_SMALL") != std::string::npos ? LevelSpriteSizeType::SMALL
                                : row.animation.find("_MID") != std::string::npos ? LevelSpriteSizeType::MEDIUM
                                : row.animation.find("_LARGE") != std::string::npos ? LevelSpriteSizeType::LARGE
                                        : LevelSpriteSizeType::NONE;

                const EntityProperties& decorationProperties{Entity::Type::PIPE, position};
                createLevelCollidableSprite(decorationProperties, {"pipe", decorationSizeInfo, row.totalTilesInWidth, row.totalTilesInHeight});
            }
        }
        else if (row.entityType == "DECORATION")
        {
            const std::pair<Entity::Type, const std::string&> decorationEntityInfo =
                    row.animation.find("BUSH_") != std::string::npos ? std::make_pair<Entity::Type, const std::string&>(Entity::Type::BUSH, "bush")
                    : row.animation.find("HILL_") != std::string::npos ? std::make_pair<Entity::Type, const std::string&>(Entity::Type::HILL, "hill")
                    : row.animation.find("CLOUD_") != std::string::npos ? std::make_pair<Entity::Type, const std::string&>(Entity::Type::CLOUD, "cloud")
                    : std::make_pair(Entity::Type::NONE, nullptr);

            const LevelSpriteSizeType decorationSizeInfo =
                    row.animation.find("_SMALL") != std::string::npos ? LevelSpriteSizeType::SMALL
                            : row.animation.find("_MID") != std::string::npos ? LevelSpriteSizeType::MEDIUM
                            : row.animation.find("_LARGE") != std::string::npos ? LevelSpriteSizeType::LARGE
                            : LevelSpriteSizeType::NONE;

            const EntityProperties& decorationProperties{decorationEntityInfo.first, position};
            createLevelDecoration(decorationProperties, {decorationEntityInfo.second, decorationSizeInfo, row.totalTilesInWidth, row.totalTilesInHeight});
        }
    }
}

void EntitySpawnSystem::createEntity(const EntityProperties& entityProperties,
        std::unordered_map<Component::Type, std::shared_ptr<Component>> componentGroup)
{
    std::shared_ptr<Entity>& entity = m_entityManager.addEntity(entityProperties.entityType);

    std::shared_ptr<CSpriteGroup> spriteGroup = componentGroup.contains(Component::Type::SPRITE_GROUP)
            ? std::static_pointer_cast<CSpriteGroup>(componentGroup.at(Component::Type::SPRITE_GROUP))
            : std::make_shared<CSpriteGroup>();

    if (!componentGroup.contains(Component::Type::SPRITE_GROUP))
    {
        for (CSpriteGroup::SpriteAnimation properties: entityProperties.animationProperties)
        {
            addAnimationTextureComponent(spriteGroup, properties.textureSheetFilePath, entityProperties.position,
                    properties.animationRectBounds, properties.origin,
                    properties.totalAnimationFrames, TILE_SIZE, properties.animationTicker.animationUpdateTime,
                    properties.rotation);
        }

        componentGroup[Component::Type::SPRITE_GROUP] = spriteGroup;
    }

    for (const auto& entry : componentGroup)
    {
        entity->addComponent(entry.first, entry.second);
    }
}

void EntitySpawnSystem::createLevelDecoration(const EntityProperties& entityProperties, const LevelSprite& levelSprite)
{
    std::shared_ptr<Entity> decoration = m_entityManager.addEntity(entityProperties.entityType);
    applySpriteGroupForEntity(entityProperties.position, levelSprite, decoration);
}

void EntitySpawnSystem::createLevelCollidableSprite(const EntityProperties& entityProperties, const LevelSprite& levelSprite)
{
    std::shared_ptr<Entity> collidableEntity = m_entityManager.addEntity(entityProperties.entityType);
    applySpriteGroupForEntity(entityProperties.position, levelSprite, collidableEntity);
    collidableEntity->addComponent(Component::Type::COLLISION, std::make_shared<CCollision>());
    collidableEntity->addComponent(Component::Type::STATIC_MOVEMENT, std::make_shared<CMovement>());
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

    sf::Rect<int> rectBounds = sf::IntRect(0, 0, levelSprite.totalTilesInWidth * TILE_SIZE, levelSprite.totalTilesInHeight * TILE_SIZE);

    addAnimationTextureComponent(animationComponent, animationTextureFilePath, position, rectBounds,
            sf::Vector2f(rectBounds.width / 2, rectBounds.height / 2), 1, 0, 0, 0);
    decoration->addComponent(Component::SPRITE_GROUP, animationComponent);
}

// TODO Refactor to not take so many params, but an aggregation dto
void EntitySpawnSystem::addAnimationTextureComponent(std::shared_ptr<CSpriteGroup>& spriteGroup,
        const std::string& animationTextureFilePath,
        const sf::Vector2f& position, sf::IntRect& rectBounds,
        const sf::Vector2f origin, const uint16_t totalAnimationFrames, const uint16_t animationIncrement,
        float spriteAnimationCompletionTime, float rotation)
{
    CSpriteGroup::SpriteAnimation spriteAnimation{animationTextureFilePath,
                                                  rectBounds, rectBounds, animationIncrement,
                                                  0, totalAnimationFrames,
                                                  {0, spriteAnimationCompletionTime}};

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

    spriteGroup->sprites.emplace_back(std::make_shared<sf::Sprite>(sprite));
}

void EntitySpawnSystem::removeLastAnimationTexture(std::shared_ptr<CSpriteGroup>& spriteGroup)
{
    spriteGroup->animations.pop_back();
    spriteGroup->textures.pop_back();
    spriteGroup->sprites.pop_back();
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

std::vector<EntitySpawnSystem::Row> EntitySpawnSystem::LoadLevelData()
{
    const std::string path = "resources/level/" + m_levelName + ".txt";
    std::ifstream file(path, std::ifstream::in);
    assert(file.is_open());

    std::vector<Row> rows;
    std::string line;

    while (getline(file, line))
    {
        Row row{};
        file >> row.entityType >> row.animation >> row.locationX >> row.locationY
             >> row.totalTilesInWidth >> row.totalTilesInHeight >> row.item;
        rows.push_back(row);
    }

    file.close();
    return rows;
}