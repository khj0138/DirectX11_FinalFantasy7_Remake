#pragma once
#include "Entity.h"

#include "ptr.h"
#include "Texture.h"

namespace ff7r
{
    class Animator2D;
    class Animation2D : public Entity
    {
    private:
        Animator2D* owner;
        vector<Anim2DFrame>  frames;
        vec2                back_size;
        Ptr<Texture>       atlas_tex;

        int                 cur_frame;
        float               time;

        bool                is_finish;

    public:
        void FinalTick();
        void Create(const wstring& _strAnimName, Ptr<Texture> _AtlasTex, vec2 _vLeftTop, vec2 _vSlice, vec2 _BackSize, int _FrameCount, int _FPS);
        void Create(const wstring& _ani_name, Ptr<Texture> _atlas_tex, vec2 _left_top, vec2 _slice_cnt, float _fps);

        const Anim2DFrame& GetCurFrame() { return frames[cur_frame]; }
        vec2 GetBackSize() { return back_size; }
        Ptr<Texture> GetAtlasTex() { return atlas_tex; }

        void SetFrame(int _frame) { cur_frame = _frame; }
        bool IsFinish() { return is_finish; }
        void Reset()
        {
            cur_frame = 0;
            time = 0.f;
            is_finish = false;
        }

        void SaveToLevelFile(FILE* _File);
        void LoadFromLevelFile(FILE* _File);


        CLONE(Animation2D);
    public:
        Animation2D();
        ~Animation2D();

        friend class Animator2D;
    };
}
