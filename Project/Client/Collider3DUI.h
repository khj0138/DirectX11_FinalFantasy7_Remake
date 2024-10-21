#pragma once
#include "ComponentUI.h"

namespace ff7r
{
    class Collider3DUI : public ComponentUI
    {
    public:
        virtual int RenderUpdate() override;

    public:
        Collider3DUI();
        ~Collider3DUI();
    };
}
