#pragma once
#include "ResourceUI.h"

namespace ff7r
{
    class TextureUI : public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        TextureUI();
        ~TextureUI();
    };
}
