#pragma once
#include "ResourceUI.h"

namespace ff7r
{
    class PrefabUI : public ResourceUI
    {
    private:
        bool is_right_click;
    public:
        virtual int RenderUpdate() override;

    public:
        PrefabUI();
        ~PrefabUI();
    };
}
