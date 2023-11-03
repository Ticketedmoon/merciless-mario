#pragma once

#ifndef MERCILESS_MARIO_C_INTERACTABLE_H
#define MERCILESS_MARIO_C_INTERACTABLE_H


#include "component.h"

class CInteractable : public Component
{
    public:
        CInteractable();

        [[nodiscard]] bool isInteractable() const;
        void deactivate();

    private:
        bool m_canInteract = true;
};


#endif //MERCILESS_MARIO_C_INTERACTABLE_H
