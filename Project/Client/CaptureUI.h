#pragma once
#include "ComponentUI.h"

namespace ff7r
{
    class CaptureUI : public ComponentUI
    {
    public:
        CaptureUI();
        ~CaptureUI();

    public:
        virtual int RenderUpdate() override;
    };
}
