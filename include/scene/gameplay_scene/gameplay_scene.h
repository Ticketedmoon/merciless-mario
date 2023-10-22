#pragma once

#ifndef MERCILESS_MARIO_GAMEPLAY_SCENE_H
#define MERCILESS_MARIO_GAMEPLAY_SCENE_H

#include <SFML/Graphics.hpp>

#include "game_engine.h"
#include "scene.h"
#include "entity_manager.h"
#include "system_manager.h"

#include "render_system.h"
#include "transform_system.h"
#include "animation_system.h"
#include "collision_system.h"
#include "view_system.h"
#include "lifespan_system.h"
#include "entity_spawn_system.h"

#include "c_sprite_group.h"
#include "c_movement.h"
#include "c_collision.h"
#include "c_action.h"
#include "c_transform.h"
#include "texture_manager.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems();
        void registerActions();

    private:
        EntityManager m_entityManager;
        SystemManager m_systemManager;

        sf::RenderTexture m_renderTexture;
        sf::Sprite m_renderSprite;
};


#endif //MERCILESS_MARIO_GAMEPLAY_SCENE_H
