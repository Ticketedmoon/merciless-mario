#include "render_system.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
    configureTextRendering();
}

void RenderSystem::execute()
{
    drawEntities();
    drawGuiData();
}

void RenderSystem::drawEntities()
{
    std::vector<std::shared_ptr<Entity>> transformEntities = m_entityManager
            .getEntitiesByComponentTypes({Component::SPRITE_GROUP});
    for (const std::shared_ptr<Entity>& e: transformEntities)
    {
        std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(
                e->getComponentByType(Component::TRANSFORM));
        std::shared_ptr<CSpriteGroup> cSpriteGroup = std::static_pointer_cast<CSpriteGroup>(
                e->getComponentByType(Component::SPRITE_GROUP));

        for (std::shared_ptr<sf::Sprite>& sprite: cSpriteGroup->sprites)
        {
            if (cTransform != nullptr)
            {
                sprite->setPosition(cTransform->m_position);
            }
            m_renderTarget.draw(*sprite);
        }
    }
}

void RenderSystem::drawGuiData()
{
    std::vector<std::shared_ptr<Entity>> players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        return;
    }

    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CWeapon> cWeapon = std::static_pointer_cast<CWeapon>(player->getComponentByType(Component::WEAPON));

        m_ammoText.setString("Round: " + std::to_string(cWeapon->currentRoundBullets) + "\nMag: " +
                std::to_string(cWeapon->totalMagazinesAvailable));

        const sf::Vector2u& windowSize = m_renderTarget.getSize();
        float viewCentreX = m_renderTarget.getView().getCenter().x;
        const sf::Vector2<float>& position = sf::Vector2f(viewCentreX - windowSize.x/4 - TEXT_POSITION_OFFSET, windowSize.y / 2 -
                TEXT_POSITION_OFFSET);

        drawText(m_ammoText, sf::Color::Red, 20, position);
    }
}

// TODO move to engine for inter-scene text drawing?
void RenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.6f);
    text.setLetterSpacing(3.0f);

    m_renderTarget.draw(text);
}

void RenderSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    m_ammoText = sf::Text("", m_font);
}