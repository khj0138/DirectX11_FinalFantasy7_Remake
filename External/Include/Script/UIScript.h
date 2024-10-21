#pragma once
#include <Engine\Script.h>

namespace ff7r
{
    class UIScript : public Script
    {
    public:
        enum class UI_FUNC
        {
            TICK,
            ENABLE,
            DISABLE,
            CALL,
            SELECT,
            RETURN,
            RESET,
        };
        virtual void Awake() override;
        virtual void Tick() override;

    private:
        virtual void EnableFunc() {};
        virtual void DisableFunc() {};
        virtual void TickFunc() {};
        virtual void CallFunc() {};
        virtual void ReturnFunc() {};
        virtual void ResetFunc() {};
        virtual void SelectFunc() {};

    public:
        UIScript(UINT _ScriptType);
        ~UIScript();

        virtual void OnEnable() override;
        virtual void OnDisable() override;

        void SetPivot(vec2 _pivot) { pivot = _pivot; }
        vec2 GetPivot() { return pivot ; }
        void SetPos(vec2 _pos) { pos = _pos; }
        vec2 GetPos() { return pos ; }
        void SetScale(vec2 _scale) { scale = _scale; }
        vec2 GetScale() { return scale; }
        void SetActive(bool _active);
        bool GetActive() { return is_enable; }

        void SetEnableTime(float _active_time) { enable_time = _active_time; }
        float GetEnableTime() { return enable_time; }

        void SetTarget(wstring _target_name) { target_ui = _target_name; }
        wstring GetTarget() { return target_ui; }

        void SetSize(vec2 _size, vec2 _scale_ratio);
        void SetLeftTop(vec2 _left_top);

        //bool CheckUIFunc(float _condition_val);
        void SetUIFunc(UI_FUNC _func);
        
        void (UIScript::* UI_Func)(void);

        float time_val;
    private:
        vec2 pivot;
        vec2 pos;
        vec2 scale;
        vec3 rot;
        bool is_enable;
        float enable_time;
		
        wstring target_ui;
    };
}

