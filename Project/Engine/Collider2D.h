#pragma once
#include "Component.h"

namespace ff7r
{
    class Collider2D : public Component
    {
    public:
        Collider2D();
        ~Collider2D();

        CLONE(Collider2D);

        virtual void FinalTick() override;

        void BeginOverlap(Collider2D* _Other);
        void OnOverlap(Collider2D* _Other);
        void EndOverlap(Collider2D* _Other);

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;

        void SetOffsetPos(vec2 _vOffsetPos) { offset_pos = vec3(_vOffsetPos.x, _vOffsetPos.y, 0.f); }
        void SetOffsetScale(vec2 _vOffsetScale) { offset_scale = vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
        void SetAbsolute(bool _bSet) { is_absolute = _bSet; }
        void SetCollider2DType(COLLIDER2D_TYPE _Type) { type = _Type; }

        const Matrix& GetColliderWorldMat() { return mat_colli_world; }

    private:
        vec3            offset_pos;
        vec3            offset_scale;
        bool            is_absolute;
        COLLIDER2D_TYPE type;
        Matrix          mat_colli_world;    // Collider 의 월드행렬

        int             collision_cnt;  // 충돌 횟수
    };

}