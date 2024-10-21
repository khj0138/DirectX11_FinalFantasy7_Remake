#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "AnimationState.h"
#include "AnimatorController.h"

namespace ff7r
{
    class StructuredBuffer;
    class Animation3DShader;
    class Animator3D : public Component
    {
    public:
        Animator3D();
        Animator3D(const Animator3D& _origin);
        ~Animator3D();

        CLONE(Animator3D);

        virtual void Awake() override;
        virtual void FinalTick() override;

        void UpdateData();
        void ClearData();

        void SetAnimClip(const vector<Ptr<Animation3D>>& _animations);
        void SetAnimClip(const map<wstring, Ptr<Animation3D>>& _animations);
        void SetAnimatorController(Ptr<AnimatorController> _controller);

        void ChangeState(AnimationState* _state, float _duration, int _frame_idx);
        void CreateParameter(wstring _key, Parameter* _param) { _param->SetKey(_key); controller->parameters.insert({ _key, _param }); }

        AnimationState* GetCurState() { return cur_state; }
        map<wstring, AnimationState*>& GetAnimStates() { return controller->anim_states; }
        AnimationState* CreateEmptyState(wstring _key);
        bool RenameState(wstring _prev_key, wstring _new_key);
        void DeleteState(wstring _key);
        bool RenameParameter(wstring _prev_key, wstring _new_key);
        void DeleteParameter(wstring _key);


        AnimationState* GetAnimState(wstring _key);
        Ptr<Animation3D> GetAnimation(wstring _key);
        Transition* CreateTransition(AnimationState* _src, AnimationState* _dest);
        Parameter* GetParameter(wstring _key);


        map<wstring, Ptr<Animation3D>>& GetAnimations() { return animations; }
        map<wstring, Ptr<Animation3D>>::iterator GetAnimationsBegin() { return animations.begin(); }
        map<wstring, Ptr<Animation3D>>::iterator GetAnimationsEnd() { return animations.end(); }
        int GetAnimationsCount() { return animations.size(); }

        map<wstring, Parameter*>& GetParameters() { return controller->parameters; }

        void SetFloat(wstring _key, float _f) { SetParameter(_key, _f, DATA_TYPE::FLOAT); }
        void SetInteger(wstring _key, int _i) { SetParameter(_key, _i, DATA_TYPE::INT); }
        void SetBool(wstring _key, bool _b) { SetParameter(_key, _b, DATA_TYPE::BOOL); }
        void SetTrigger(wstring _key) { SetParameter(_key, true, DATA_TYPE::TRIGGER); }
        void SetBoneMotion(bool _is_bone_motion) { is_bone_motion = _is_bone_motion; }
        void RegisterAnimationEvent(wstring _name, function<void()> _event) { anim_events.insert({ _name, _event }); }

        bool IsBoneMotion() { return is_bone_motion; }
        function<void()> GetAnimationEvent(wstring _name);
        map<wstring, function<void()>>::iterator GetAnimationEventBegin() { return anim_events.begin(); }
        map<wstring, function<void()>>::iterator GetAnimationEventEnd() { return anim_events.end(); }

        StructuredBuffer* GetFinalJointMat() { return joint_final_mat_buff; }
        StructuredBuffer* GetCurJointMat() { return cur_joint_mat_buff; }
        Animation3DShader* GetAnimationShader() { return shader; }
        void BoneMotion();

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

        void SaveAnimator();
        void CalcWorldDir();
        void CalcCurRootMatrix();
        void ResetRootMat() { prev_root_mat = Matrix(); cur_root_mat = Matrix(); prev_rot = {}; }
        void ResetStartClip() { is_start_clip = false; }
        void SetRot(float _a, float _b) { rot_ratio = fabs(_a); rot_origin = _b; }
        void SetMoveFront(bool _is_move_front) { is_move_front = _is_move_front; }

    private:
        AnimationState* FindNextState(AnimationState* _state);
        void SetParameter(wstring _key, Data _data, DATA_TYPE _type);
        void check_mesh(Ptr<Mesh> _pMesh);

        Animation3DShader*          shader;
        AnimationState*             cur_state;
        AnimationState*             wait_state;
        AnimationState*             any_state;

        vector<Matrix>				final_joint_mat; // 텍스쳐에 전달할 최종 행렬정보
        Matrix                      cur_root_mat;
        Matrix                      prev_root_mat;
        FrameTrans                  last_frame_trans;

        // test 
        vec3 world_dir[3];
        vec3 prev_rot;
        // tt

        vector<FrameTrans>          joint_trans;
        StructuredBuffer* cur_joint_mat_buff;
        StructuredBuffer* joint_trans_buff;  // 애니메이션 변경시 보간을 위한 현재 trans정보
        StructuredBuffer* joint_final_mat_buff;  // 특정 프레임의 최종 행렬

        Ptr<AnimatorController>     controller;

        float                       wait_time;
        float                       rot_ratio;
        float                       rot_origin;

        int                         start_frame_idx;

        bool						is_mat_final_update; // 최종행렬 연산 수행여부
        bool                        is_change_clip;
        bool                        is_start_clip;
        bool                        is_start_change_clip;
        bool                        is_bone_motion;
        bool                        is_move_front;

        map<wstring, Ptr<Animation3D>>      animations;
        map<wstring, function<void()>>      anim_events;
    };
}