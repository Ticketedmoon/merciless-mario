#include "c_interactable.h"

CInteractable::CInteractable() = default;

bool CInteractable::isInteractable() const
{
    return m_canInteract;
}

void CInteractable::deactivate()
{
    m_canInteract = false;
}
