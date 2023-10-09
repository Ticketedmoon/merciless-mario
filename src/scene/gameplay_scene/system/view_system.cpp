#include "view_system.h"
#include "c_transform.h"
#include "c_sprite.h"

ViewSystem::ViewSystem(std::shared_ptr<Entity>& entityToView, sf::RenderTexture& renderTexture) : m_entityToView(entityToView), m_renderTexture(renderTexture)
{

}

void ViewSystem::execute()
{
    if (!m_entityToView->hasComponents({Component::Type::TRANSFORM, Component::Type::SPRITE}))
    {
        return;
    }

    // keep view centred/centered on entity
    std::shared_ptr<CTransform> cTransform = std::static_pointer_cast<CTransform>(m_entityToView->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CSprite> cSprite = std::static_pointer_cast<CSprite>(m_entityToView->getComponentByType(Component::Type::SPRITE));

    sf::Vector2f entityPos = cTransform->m_position;

    float centreX = getViewCentreForCoordinate(entityPos.x, MAX_LEVEL_WIDTH, WINDOW_WIDTH, cSprite->m_shape.getSize().x);
    float centreY = getViewCentreForCoordinate(entityPos.y, WINDOW_HEIGHT, WINDOW_HEIGHT, cSprite->m_shape.getSize().y);

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
