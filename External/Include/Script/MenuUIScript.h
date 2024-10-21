#pragma once

#include <stack>
#include "UIScript.h"

namespace ff7r
{
    class SelectUIScript;
    class ButtonUIScript;
    class MenuUIScript : public UIScript
    {
    public:
        enum class MENU_TYPE
        {
            COMMAND,
            ABILITY,
            MAGIC,
            ITEM,
            LIMIT,
            TARGET,
            END
        };

        MenuUIScript();
        ~MenuUIScript();

        CLONE(MenuUIScript);
        
        virtual void Awake() override;
        virtual void Tick() override;

        virtual void OnEnable() override;
        virtual void OnDisable() override;

    private:
        void CreateChild();
        void ChangePlayer();
        void ChangeType(MENU_TYPE _type);
        void PushNextType();

        void DeliverCommandData();
        void AddChildTarget(GameObject* _obj, int _layer_idx);

        MENU_TYPE StrToType(const string& _str);

        virtual void EnableFunc() override;
        virtual void DisableFunc() override;
        virtual void TickFunc() override;
        virtual void CallFunc() override;
        virtual void ReturnFunc() override;
        virtual void SelectFunc() override;
        virtual void ResetFunc() override;

        void UpdateChilds();

    private:
        vector<SelectUIScript*> select_ui;
        std::stack<MENU_TYPE>   ui_stack;

        GameObject* player;
        GameObject* portrait;

        MENU_TYPE   type;

        string      prev_choice;

        int active_select_cnt;
        int focused_idx;
    };
}

