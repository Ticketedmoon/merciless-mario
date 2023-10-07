#include "scene/scene.h"

Scene::Scene(GameEngine& gameEngine) : gameEngine(gameEngine)
{

}

Scene::~Scene()
{

}

void Scene::registerActionType(int inputCode, Action::Type actionType)
{
    m_actionMap[inputCode] = actionType;
}

void Scene::registerCursorActionType(sf::Event::EventType eventType, Action::Type type)
{
    m_cursorActionMap[eventType] = type;
}

bool Scene::hasActionType(int inputCode) const
{
    return m_actionMap.contains(inputCode);
}

bool Scene::hasEventType(sf::Event::EventType event) const
{
    return m_cursorActionMap.contains(event);
}

Action::Type Scene::getActionType(int inputCode)
{
    return m_actionMap[inputCode];
}

Action::Type Scene::getCursorActionType(sf::Event::EventType eventType)
{
    return m_cursorActionMap[eventType];
}
