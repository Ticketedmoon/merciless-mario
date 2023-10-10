#pragma once

#ifndef MERCILESS_MARIO_GAME_ENGINE_H
#define MERCILESS_MARIO_GAME_ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <string_view>
#include <memory>

#include "common_constants.h"
#include "scene.h"
#include "gameplay_scene.h"

class GameEngine
{
    public:
        GameEngine();
        ~GameEngine();
        void startGameLoop();
        void changeScene(Scene::Type sceneType, const std::shared_ptr<Scene>& scene);

    private:
        void handleInput();
        void update();
        void render();

        void createGameWindow();

    public:
        sf::RenderWindow window;
        sf::Clock deltaClock;
        AudioManager* m_audioManager = AudioManager::getInstance();

    private:
        Scene::Type currentScene = Scene::Type::MENU_SCENE;
        std::unordered_map<Scene::Type, std::shared_ptr<Scene>> gameScenes;
};

#endif //MERCILESS_MARIO_GAME_ENGINE_H