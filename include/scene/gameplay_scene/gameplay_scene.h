#pragma once

#ifndef MERCILESS_MARIO_GAMEPLAY_SCENE_H
#define MERCILESS_MARIO_GAMEPLAY_SCENE_H

#include "game_engine.h"
#include "scene.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems(GameEngine& engine);
};


#endif //MERCILESS_MARIO_GAMEPLAY_SCENE_H
