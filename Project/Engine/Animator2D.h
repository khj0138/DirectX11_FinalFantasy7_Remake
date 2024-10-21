#pragma once
#include "Component.h"

#include "ptr.h"
#include "Texture.h"

namespace ff7r
{
    class Animation2D;
    class Animator2D : public Component
    {
    public:
        Animator2D();
        Animator2D(const Animator2D& _other);
        ~Animator2D();

        CLONE(Animator2D);

        virtual void FinalTick() override;
        void UpdateData();
        void Clear();   
        void SetRepeat(bool _repeat) { is_repeat = _repeat; }
        void SetManagementActive(bool _is) { is_management_active = _is; }
        void Reset();

        void Play(const wstring& _strName, bool _bRepeat, int _frame = 0);
        Animation2D* FindAnim(const wstring& _strName);

        void CreateAnimation(const wstring& _strAnimName, Ptr<Texture> _AtlasTex, vec2 _vLeftTop, vec2 _vSlice, vec2 _vBackSize, int _FrameCount, int _FPS);
        void CreateAnimation(const wstring& _ani_name, Ptr<Texture> _atlas_tex, vec2 _left_top, vec2 _slice_cnt, float _fps);

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;

    private:
        map<wstring, Animation2D*>  map_anim;  // Animation 목록
        Animation2D* cur_anim; // 현재 재생중인 Animation

        bool                    is_repeat;  // 반복
        bool                    is_management_active;
    };
}

