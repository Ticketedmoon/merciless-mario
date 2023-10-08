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
#include "collision_system.h"

#include "c_sprite.h"
#include "c_collision.h"
#include "c_action.h"
#include "c_transform.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void createPlayer();
        void createPlatform(sf::Vector2f size, sf::Vector2f position);
        void registerSystems(GameEngine& engine);

    private:
        EntityManager m_entityManager;
        SystemManager m_systemManager;
        void registerActions();
};


#endif //MERCILESS_MARIO_GAMEPLAY_SCENE_H
