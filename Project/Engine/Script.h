#pragma once
#include "Component.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"

#include "ResourceMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

#include "GameObject.h"
#include "components.h"

namespace ff7r
{
    enum class SCRIPT_PARAM
    {
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
    };

    struct ScriptParam
    {
        SCRIPT_PARAM    type;
        void* data;
        string          desc;
    };

    class Collider2D;

    class Script :
        public Component
    {
    private:
        UINT                    script_type;
        vector<ScriptParam>    parameters;


    public:
        void Destroy() { DestroyObject(GetOwner()); }
        void SetLifeSpan(float _Time) { GetOwner()->SetLifeSpan(_Time); }
        UINT GetScriptType() { return script_type; }
        const vector<ScriptParam>& GetScritpParam() { return parameters; }

    public:
        virtual void Awake() {}
        virtual void Start() {}
        virtual void FinalTick() final {};

        virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) {}
        virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) {}
        virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) {}

    public:
        virtual void SaveToLevelFile(FILE* _File) override {}
        virtual void LoadFromLevelFile(FILE* _FILE) override {}

    protected:
        void AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc);

    public:
        Script(UINT _iScriptType);
        ~Script();

        friend class GameObject;
    };
}
