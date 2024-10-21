#pragma once
#include "UIScript.h"

namespace ff7r
{
    class ButtonUIScript : public UIScript
    {
    public:
        ButtonUIScript();
        ~ButtonUIScript();

        CLONE(ButtonUIScript);

        virtual void Awake() override;
        virtual void Tick() override;

        virtual void EnableFunc() override;
        virtual void DisableFunc() override;
        virtual void TickFunc() override;
        
        void SetKeyType(KEY _key) { select_key = _key; }
        void SetFocused();
        bool IsFocused() { return is_tap; }

    private:
        void TapButton();
        void ChangeButtonTexture(bool _is_tap);

        GameObject* effect;

        vec2        origin_pos;
        vec2        tap_offset_pos;

        float       focus_time;
        float       highlight_time;
        float       cur_time;
        bool        is_tap;

        KEY         select_key;
    };
}

