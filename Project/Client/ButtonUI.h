#pragma once
#include "UI.h"

#include <Engine\Ptr.h>

namespace ff7r
{
    class ButtonUI :
        public UI
    {
    private:
        UI* select_inst;
        UI_DELEGATE             select_delegate;
        UI_DELEGATE_1           select_delegate1;

    private:


    public:
        ButtonUI(const string& _name);
        ~ButtonUI();
        virtual int RenderUpdate() override;

    public:
        void AddDynamic_Button(UI* _inst, UI_DELEGATE _func)
        {
            select_inst = _inst;
            select_delegate = _func;
        }
    };
}

