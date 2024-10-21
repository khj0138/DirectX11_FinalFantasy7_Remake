#pragma once
#include "UI.h"


#include <Engine\ptr.h>
#include <Engine\Resource.h>

namespace ff7r
{
    class GameObject;
    class ComponentUI;
    class ResourceUI;
    class ScriptUI;
    class ListUI;
    class ButtonUI;

    class InspectorUI : public UI
    {
    public:
        InspectorUI();
        ~InspectorUI();

        virtual void Init() override;
        virtual void Tick() override;
        virtual int RenderUpdate() override;

        void SetTargetObject(GameObject* _Target);
        void SetTargetResource(Ptr<Resource> _Res);

        void AddComponent(COMPONENT_TYPE _type);
        void AddComponent(DWORD_PTR _type);
        void AddScript(const wstring& _script_name);
        void ShowCompList(); // 추가 가능한 component 리스트 활성화
        void ClearTargetObject();
        void ClearTargetResource();


    private:
        GameObject* target_object;
        ComponentUI* component_UIs[(UINT)COMPONENT_TYPE::END];
        vector<ScriptUI*>       script_UIs;
        Ptr<Resource>           target_resource;
        ResourceUI* resource_UIs[(UINT)RES_TYPE::END];
        ButtonUI* add_comp_button;
        ListUI* component_list;
    };

}