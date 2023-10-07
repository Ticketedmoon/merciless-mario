#include "scene/gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
}

void GameplayScene::update()
{
}

void GameplayScene::render()
{
    gameEngine.window.clear();

    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    Action::Type actionType = action.getType();
}

void GameplayScene::registerSystems(GameEngine& engine)
{

}