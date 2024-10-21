#pragma once
#include "UIScript.h"

namespace ff7r
{
    class ButtonUIScript;
    class TextUIScript : public UIScript
    {
    public:
        TextUIScript();
        ~TextUIScript();

        CLONE(TextUIScript);

        virtual void Awake() override;

        void CreateDynamicMtrl();

        void SetText(string _text);
        void SetColor(vec4 _color);
        void SetDamageText(string _text);
        void SetAlphaFactor(float _factor);

        void RegisterText(string _sentence, vec2 pos = vec2(0.0f,0.0f));

    private:
        void UpdateChilds();
        
        bool is_dynamic_mtrl;
    };
}

