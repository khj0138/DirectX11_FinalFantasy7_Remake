#pragma once
#include "UI.h"

namespace ff7r
{
    class ListUI : public UI
    {
    private:
        vector<string>          str_data;
        int                     selected_index;
        bool                    is_popup; // popup 여부
        bool                    is_filter; // is_filter 여부

        UI* select_inst;
        UI_DELEGATE_1           select_delegate;



    public:
        virtual void FinalTick() override;
        virtual int RenderUpdate() override;

    public:
        void Reset(const string& _strName, ImVec2 _vSize);
        void AddItem(const string& _strItem) { str_data.push_back(_strItem); }
        void Clear() { str_data.clear(); selected_index = -1; }
        void SetPopup(bool _popup) { is_popup = _popup; }
        void SetFilter(bool _filter) { is_filter = _filter; }

        void AddDynamic_Select(UI* _Inst, UI_DELEGATE_1 _Func)
        {
            select_inst = _Inst;
            select_delegate = _Func;
        }


    public:
        ListUI();
        ~ListUI();
    };

}