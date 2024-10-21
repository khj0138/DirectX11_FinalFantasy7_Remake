#include "pch.h"
#include "ListUI.h"

#include <Engine\KeyMgr.h>

namespace ff7r
{
    ListUI::ListUI()
        : UI("##List")
        , selected_index(0)
        , is_popup(false)
    {
    }

    ListUI::~ListUI()
    {
    }

    void ListUI::FinalTick()
    {
        UI::FinalTick();

        // Esc ������ ��Ȱ��ȭ
        if (KEY_TAP(KEY::ESC))
        {
            SetActive(false);

            // ��� UI ��Ŀ�� ����
            ImGui::SetWindowFocus(nullptr);
        }
    }

    int ListUI::RenderUpdate()
    {
        // �ֱ� UI �� �۾����� ����� �˾Ƴ���.
        ImVec2 _contents_size = ImGui::GetContentRegionAvail();

        ImGuiTextFilter _filter;
        bool _is_filter_hovered = false;
        if (is_filter)
        {
            _filter.Draw("##componentsFilter");
            _is_filter_hovered = ImGui::IsItemHovered();
        }
        if (ImGui::BeginListBox("##list", _contents_size))
        {
            for (int i = 0; i < str_data.size(); i++)
            {
                const bool _is_selected = (selected_index == i);

                const char* _item_name = str_data[i].c_str();

                // ����Ʈ ������ ���, ��ȯ���� Ŭ�� True/ False
                if (is_filter && !_filter.PassFilter(_item_name))
                {
                    continue;
                }
                else
                {
                    if (ImGui::Selectable(_item_name, _is_selected))
                    {
                        selected_index = i;
                    }
                }

                // Ư�� ������(�ε��� i) ������ ����Ŭ���� �߻��ϸ�
                if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    if (ImGui::IsItemHovered())
                    {
                        SetActive(false);
                        ImGui::SetWindowFocus(nullptr);

                        // Select Delegate �� ��ϵǾ��ִٸ� ȣ���� �ش�.
                        if (select_inst && select_delegate)
                        {
                            (select_inst->*select_delegate)((DWORD_PTR)str_data[i].c_str());
                        }
                    }
                    else if (is_popup && !_is_filter_hovered) // popup�� ���, List UI ���� �� Ŭ�� �� ��Ȱ��ȭ
                    {
                        SetActive(false);
                    }
                }
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (_is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }


        return TRUE;
    }

    void ListUI::Reset(const string& _strName, ImVec2 _vSize)
    {
        Clear();

        SetName(_strName);

        SetSize(_vSize.x, _vSize.y);

        SetActive(true);

        // ��ġ ����
        HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();

        RECT rt = {};
        //GetClientRect(hWnd, &rt);
        GetWindowRect(hWnd, &rt);

        float CenterX = (float)((rt.left + rt.right) / 2);
        float CenterY = (float)((rt.bottom + rt.top) / 2);

        SetPopupPos(ImVec2(CenterX - _vSize.x / 2.f, CenterY - _vSize.y / 2.f));
    }
}