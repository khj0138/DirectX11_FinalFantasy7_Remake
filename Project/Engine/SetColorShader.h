#pragma once
#include "ComputeShader.h"

#include "ptr.h"
#include "Texture.h"

namespace ff7r
{
    class SetColorShader : public ComputeShader
    {
    private:
        Ptr<Texture>   target_tex;

    public:
        void SetTargetTexture(Ptr<Texture> _Tex);
        void SetColor(vec3 _RGB) { mtrl_const_data.arr_vec4[0] = _RGB; }

    public:
        virtual void UpdateData() override;
        virtual void Clear() override;

    public:
        SetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
        ~SetColorShader();
    };

}