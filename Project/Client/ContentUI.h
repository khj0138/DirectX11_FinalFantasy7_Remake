#pragma once
#include "UI.h"

namespace ff7r
{
    class TreeUI;
    class ContentUI : public UI
    {
    private:
        TreeUI* m_Tree;
        vector<wstring>     m_vecResPath;
        wstring root_path;
        wstring exclude;

        bool is_right_click;
        bool is_item_right_click;
        bool is_first;

    public:
        virtual void Init() override;
        virtual void Tick() override;
        virtual int RenderUpdate() override;

    public:
        void Reload(); // Content 폴더에 있는 리소스를 로딩
        void ResetContent();
        void SetTargetToInspector(DWORD_PTR _SelectedNode);
        void RightClickMenu();
        DWORD_PTR GetSelectedResource(int type);
        void SetExcludeFile(wstring _name) { exclude = _name; }

    private:
        void FindFileName(const wstring& _FolderPath, wstring exclude, wstring exclude2);
        RES_TYPE GetResTypeByExt(const wstring& _relativepath);

    public:
        ContentUI();
        ~ContentUI();
    };
}
