#pragma once

#ifndef MERCILESS_MARIO_VIEW_SYSTEM_H
#define MERCILESS_MARIO_VIEW_SYSTEM_H

#include <SFML/Graphics/View.hpp>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include "system.h"
#include "entity.h"
#include "common_constants.h"

class ViewSystem : public System
{
    public:
        explicit ViewSystem(std::shared_ptr<Entity>& entityToView, sf::RenderTexture& renderTexture);

        void execute() override;

    private:
        float getViewCentreForCoordinate(float playerCoordinatePosition, float worldDimension,
                float windowDimensionValue, float playerDimensionValue);
    private:
        static constexpr float VIEW_ZOOM_FACTOR = 1.25f;

        std::shared_ptr<Entity> m_entityToView;
        sf::RenderTexture& m_renderTexture;
};


#endif //MERCILESS_MARIO_VIEW_SYSTEM_H
