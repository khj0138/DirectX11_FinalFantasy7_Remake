#pragma once
#include "ResourceUI.h"

namespace ff7r
{
    class MeshUI : public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        MeshUI();
        ~MeshUI();
    };
}
