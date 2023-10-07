#pragma once

#ifndef MERCILESS_MARIO_GAME_ENGINE_H
#define MERCILESS_MARIO_GAME_ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <string_view>
#include <memory>
#include <iostream>

#include "scene.h"
#include "gameplay_scene.h"

static constexpr std::string_view WINDOW_TITLE = "merciless-mario";
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;
static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;

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