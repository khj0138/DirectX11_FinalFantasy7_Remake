#pragma once
#include "ComponentUI.h"

namespace ff7r
{
    class Light3DUI : public ComponentUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        Light3DUI();
        ~Light3DUI();
    };
}
