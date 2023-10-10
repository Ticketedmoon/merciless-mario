#include "view_system.h"
#include "c_transform.h"
#include "c_sprite.h"

ViewSystem::ViewSystem(EntityManager& entityManager, sf::RenderTexture& renderTexture) : m_entityManager(entityManager),
    m_renderTexture(renderTexture)
{

}

void ViewSystem::execute()
{

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.size() != 1)
    {
        // We don't support multiplayer as of now.
        return;
    }

    std::shared_ptr<Entity>& player = players.front();

    // keep view centred/centered on entity
    std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(player->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CSprite> cSprite = std::static_pointer_cast<CSprite>(player->getComponentByType(Component::Type::SPRITE));

    sf::Vector2f entityPos = cTransform->m_position;

    float centreX = getViewCentreForCoordinate(entityPos.x, MAX_LEVEL_WIDTH, WINDOW_WIDTH, cSprite->m_shape.getSize().x);
    float centreY = getViewCentreForCoordinate(entityPos.y, MAX_LEVEL_HEIGHT, WINDOW_HEIGHT, cSprite->m_shape.getSize().y);

    // Reset View
    m_renderTexture.setView(m_renderTexture.getDefaultView());

    // Update to New View
    sf::View newView = m_renderTexture.getView();
    newView.zoom(VIEW_ZOOM_FACTOR);
    newView.setCenter(centreX, centreY);
    m_renderTexture.setView(newView);
}

float ViewSystem::getViewCentreForCoordinate(const float playerCoordinatePosition, const float worldDimension,
        const float windowDimensionValue, const float playerDimensionValue) {
    if (playerCoordinatePosition <= windowDimensionValue / 4)
    {
        return windowDimensionValue / 4;
    }

    return playerCoordinatePosition < (playerDimensionValue * worldDimension)
            ? playerCoordinatePosition
            : windowDimensionValue - (windowDimensionValue / 3);
}
