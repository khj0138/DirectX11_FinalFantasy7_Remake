#pragma once
#include "ComputeShader.h"

#include "ptr.h"
#include "Texture.h"


namespace ff7r
{
    class StructuredBuffer;
    class ParticleShader : public ComputeShader
    {
    public:
        ParticleShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group);
        ~ParticleShader();

        virtual void UpdateData() override;
        virtual void Clear() override;

        void SetParticleBuffer(StructuredBuffer* _buffer);
        void SetNoiseTexture(Ptr<Texture> _tex);
        void SetRWParticleBuffer(StructuredBuffer* _buffer) { rw_buff = _buffer; }
        void SetModuleData(StructuredBuffer* _buffer) { module_data_buff = _buffer; }
        void SetObjectPos(vec3 _pos) { mtrl_const_data.arr_vec4[0] = _pos; }
        void SetObjectRotation(Matrix _mat) { mtrl_const_data.arr_mat[0] = _mat; }

    private:
        StructuredBuffer*   particle_buff;
        StructuredBuffer*   rw_buff;
        StructuredBuffer*   module_data_buff;
        
        Ptr<Texture>        noise_tex;
    };
}