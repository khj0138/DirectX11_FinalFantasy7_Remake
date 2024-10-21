#pragma once

#include "RenderComponent.h"


namespace ff7r
{
    class StructuredBuffer;
    class FontRenderer : public RenderComponent
    {
    private:
        
        StructuredBuffer* buffer;
        Texts texts;

    public:
        virtual void FinalTick() override;
        virtual void Render() override;
        virtual void Render(UINT _subset) override {}
        void UpdateData();

        void SetTexts(string _texts, bool _is_text = true);


        CLONE(FontRenderer);
    public:
        FontRenderer();
        ~FontRenderer();
    };

}