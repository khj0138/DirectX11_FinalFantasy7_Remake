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

        // Esc 눌리면 비활성화
        if (KEY_TAP(KEY::ESC))
        {
            SetActive(false);

            // 모든 UI 포커스 해제
            ImGui::SetWindowFocus(nullptr);
        }
    }

    int ListUI::RenderUpdate()
    {
        // 최근 UI 의 작업영역 사이즈를 알아낸다.
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

                // 리스트 아이템 출력, 반환값은 클릭 True/ False
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

                // 특정 아이템(인덱스 i) 위에서 더블클릭이 발생하면
                if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    if (ImGui::IsItemHovered())
                    {
                        SetActive(false);
                        ImGui::SetWindowFocus(nullptr);

                        // Select Delegate 가 등록되어있다면 호출해 준다.
                        if (select_inst && select_delegate)
                        {
                            (select_inst->*select_delegate)((DWORD_PTR)str_data[i].c_str());
                        }
                    }
                    else if (is_popup && !_is_filter_hovered) // popup인 경우, List UI 영역 외 클릭 시 비활성화
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

        // 위치 조정
        HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();

        RECT rt = {};
        //GetClientRect(hWnd, &rt);
        GetWindowRect(hWnd, &rt);

        float CenterX = (float)((rt.left + rt.right) / 2);
        float CenterY = (float)((rt.bottom + rt.top) / 2);

        SetPopupPos(ImVec2(CenterX - _vSize.x / 2.f, CenterY - _vSize.y / 2.f));
    }
}