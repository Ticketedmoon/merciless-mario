#pragma once

#ifndef MERCILESS_MARIO_VIEW_SYSTEM_H
#define MERCILESS_MARIO_VIEW_SYSTEM_H

#include <SFML/Graphics/View.hpp>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include "system.h"
#include "entity.h"
#include "common_constants.h"
#include "entity_manager.h"

class ViewSystem : public System
{
    public:
        explicit ViewSystem(EntityManager& entityManager, sf::RenderTexture& renderTexture);

        void execute() override;

    private:
        float getViewCentreForCoordinate(float playerCoordinatePosition, float worldDimension,
                float windowDimensionValue, float playerDimensionValue);
    private:
        static constexpr float VIEW_ZOOM_FACTOR = 1.25f;

        EntityManager& m_entityManager;
        sf::RenderTexture& m_renderTexture;
};


#endif //MERCILESS_MARIO_VIEW_SYSTEM_H
