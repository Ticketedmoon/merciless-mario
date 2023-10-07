#include "action.h"

Action::Action(Action::Type type, Action::Mode mode) : m_type(type), m_mode(mode)
{

}

Action::Type Action::getType()
{
    return m_type;
}

Action::Mode Action::getMode()
{
    return m_mode;
}
