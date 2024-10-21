#pragma once
#include "ComponentUI.h"

namespace ff7r
{
    class GameObject;
    class TransformUI : public ComponentUI
    {
    public:
        virtual int RenderUpdate() override;

    public:
        TransformUI();
        ~TransformUI();
    };

}