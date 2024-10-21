#include "pch.h"
#include "ButtonUI.h"

#include <Engine\EventMgr.h>

namespace ff7r
{
    ButtonUI::ButtonUI(const string& _name)
        : UI(_name)
    {
    }

    ButtonUI::~ButtonUI()
    {
    }

    int ButtonUI::RenderUpdate()
    {
        if (ImGui::Button(GetName().c_str(), ImVec2(100, 18)))
        {
            if (select_delegate)
            {
                (select_inst->*select_delegate)();
            }
            //ImGui::Separator();
        }
        return TRUE;
    }
}