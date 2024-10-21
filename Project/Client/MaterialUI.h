#pragma once
#include "ResourceUI.h"

namespace ff7r
{
    class MaterialUI : public ResourceUI
    {
    private:
        TEX_PARAM       m_eSelected;

    public:
        virtual int RenderUpdate() override;


    public:
        void SelectTexture(DWORD_PTR _Key);

    private:
        void SelectShader(DWORD_PTR _data);

    public:
        MaterialUI();
        ~MaterialUI();
    };
}
