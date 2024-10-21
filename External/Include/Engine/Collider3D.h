#pragma once
#include "Component.h"

namespace ff7r
{
    class Collider3D : public Component
    {
    public:
        Collider3D();
        ~Collider3D();

        CLONE(Collider3D);

        virtual void Awake() override;
        virtual void Start() override;
        virtual void FinalTick() override;

        void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);
        void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);
        void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;
        virtual void OnDisable() override;

        void SetOffsetPos(vec3 _vOffsetPos) { offset_pos = vec3(_vOffsetPos.x, _vOffsetPos.y, _vOffsetPos.z); }
        void SetOffsetScale(vec3 _vOffsetScale) { offset_scale = vec3(_vOffsetScale.x, _vOffsetScale.y, _vOffsetScale.z); }
        void SetAbsolute(bool _bSet) { is_absolute = _bSet; }
        void SetCollider3DType(COLLIDER3D_TYPE _Type) { type = _Type; }
        void SetTrigger(bool _trigger) { is_trigger = _trigger; }
        void SetDynamic(bool _dynamic) { is_dynamic = _dynamic; }
        void SetMove(bool _move) { is_move = _move; }
        void SetGravity(bool _gravity);
        void AddForce(vec3 _force);
        void SetMass(float _mass) { mass = _mass; }

        bool IsCollision() { return is_collision; }
        bool IsMove() { return is_move; }
        bool IsAbsolute() { return is_absolute; }
        bool IsTrigger() { return is_trigger; }
        bool IsDynamic() { return is_dynamic; }
        bool GetMove() { return is_move; }

        vec3 GetOffsetPos() { return offset_pos; }
        vec3 GetOffsetScale() { return offset_scale; }
        vec3 GetWorldPos();
        COLLIDER3D_TYPE GetType() { return type; }
        Matrix GetColWorld() { return mat_colli_world; }

        const Matrix& GetColliderWorldMat() { return mat_colli_world; }

        float GetMass() { return mass; }

        void SetRootBone();
        void SetCollision(bool _able_collision);
        bool GetCollision() { return able_collision; }
        void SceneDestroy() { scene_destroy = true; }

        void Disable();
    private:
        vec3            offset_pos;
        vec3            offset_scale;

        float           mass;

        bool            able_collision;
        bool            is_absolute;
        bool            is_trigger;
        bool            is_dynamic;
        bool            is_collision;
        bool            is_move;
        bool            scene_destroy;

        COLLIDER3D_TYPE type;
        Matrix          mat_colli_world;    // Collider 의 월드행렬

        int             collision_cnt;  // 충돌 횟수
    };

}