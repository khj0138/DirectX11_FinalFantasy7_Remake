#pragma once
#include "Component.h"

namespace ff7r
{
    class Transform : public Component
    {
    public:
        Transform();
        ~Transform();

        CLONE(Transform);

        virtual void FinalTick() override;
        void UpdateData();

        void SetRelativePos(vec3 _pos) { relative_pos = _pos; }
        void SetRelativePosXY(vec2 _pos) { relative_pos.x = _pos.x; relative_pos.y = _pos.y; }
        void SetRelativePosXY(float _x, float _y) { relative_pos.x = _x; relative_pos.y = _y; }
        void SetRelativeScale(vec3 _scale) { relative_scale = _scale; }
        void SetRelativeScaleX(float _x) { relative_scale.x = _x; }
        void SetRelativeScaleXY(vec2 _scale) { relative_scale.x = _scale.x; relative_scale.y = _scale.y; }
        void SetRelativeScaleXY(float _x, float _y) { relative_scale.x = _x; relative_scale.y = _y; }
        void SetRelativeRot(vec3 _rotation) { relative_rotation = _rotation; }
        void SetRelativeRotY(float _rot_y) { relative_rotation.y = _rot_y; }
        void SetRelativeRotZ(float _rot_z) { relative_rotation.z = _rot_z; }
        void SetRelativePos(float _x, float _y, float _z) { relative_pos = vec3(_x, _y, _z); }
        void SetRelativePosX(float _x) { relative_pos.x = _x; }
        void SetRelativeScale(float _x, float _y, float _z) { relative_scale = vec3(_x, _y, _z); }
        void SetRelativeRot(float _x, float _y, float _z) { relative_rotation = vec3(_x, _y, _z); }
        void ShiftRelativePos(vec3 _shift_pos) { relative_pos += _shift_pos; }
        void ShiftRelativeRot(vec3 _shift_rot) { relative_rotation += _shift_rot; }
        void ShiftRelativeRotY(float _y) { relative_rotation.y += _y; }

        void CalcWorldDir();

        vec3 GetRelativePos() const { return relative_pos; }
        vec2 GetRelativePosXY() const { return vec2(relative_pos.x, relative_pos.y); }
        vec3 GetRelativeScale() const { return relative_scale; }    
        vec3 GetRelativeRot() const { return relative_rotation; }
        vec3 GetRelativeDir(DIR_TYPE _type) const { return relative_dir[(UINT)_type]; }
        vec3 GetWorldDir(DIR_TYPE _type) const { return world_dir[(UINT)_type]; }
        vec3 GetWorldPos() { return mat_world.Translation(); }

        // 상대 이동, 크기를 절대값으로 지정  
        void SetAbsolute(bool _set) { is_absolute = _set; }
        void SetAbsoluteRot(bool _set) { is_absolute_rot = _set; }

        const Matrix& GetWorldScaleMat() { return mat_world_scale; }
        const Matrix& GetWorldMat() const { return mat_world; }
        void SetWorldMat(const Matrix& _mat) { mat_world = _mat; }

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

        void operator = (const Transform& _other)
        {
            relative_pos = _other.relative_pos;
            relative_scale = _other.relative_scale;
            relative_rotation = _other.relative_rotation;
            is_absolute = _other.is_absolute;
            mat_world_scale = _other.mat_world_scale;
            mat_world = _other.mat_world;
            mat_world_inv = _other.mat_world_inv;


            for (int i = 0; i < 3; ++i)
            {
                relative_dir[i] = _other.relative_dir[i];
                world_dir[i] = _other.world_dir[i];
            }
        }

    private:
        vec3    relative_pos;
        vec3    relative_scale;
        vec3    relative_rotation;

        bool    is_absolute;    // 상대 이동, 크기를 절대값으로 지정    
        bool    is_absolute_rot;

        vec3    relative_dir[3];
        vec3    world_dir[3];

        Matrix  mat_world_scale;    // 월드 크기 행렬
        Matrix  mat_world; // 크기, 회전, 이동 정보를 합쳐놓음
        Matrix  mat_world_inv;

        friend class Socket;
    };

}