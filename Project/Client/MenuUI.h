#pragma once
#include "UI.h"

namespace ff7r
{
    class MenuUI : public UI
    {
    private:


    public:
        virtual void FinalTick() override;
        virtual int RenderUpdate() override;


    private:
        void CreateEmptyObject();
        void CreateEmptyMaterial();
        void AddComponent(COMPONENT_TYPE _type);
        void AddScript(const wstring& _strScriptName);
        void LoadFileList(const wstring& _folder_path, vector<wstring>& _level_list);

        void LoadLevel(const DWORD_PTR _level_name);

    public:
        MenuUI();
        ~MenuUI();
    };
}
