#pragma once
#include "Resource.h"

#include "ptr.h"
#include "GraphicsShader.h"
#include "Texture.h"

namespace ff7r
{
    class Material : public Resource
    {
    public:
        Material(bool _is_engine_res = false);
        ~Material();
        
        CLONE(Material);

        void SetScalarParam(SCALAR_PARAM _Param, const void* _Src);
        void SetTexParam(TEX_PARAM _Param, const Ptr<Texture>& _Tex);

        void GetScalarParam(SCALAR_PARAM _param, void* _pData);
        Ptr<Texture> GetTexParam(TEX_PARAM _param) { return textures[(UINT)_param]; }

        void SetShader(Ptr<GraphicsShader> _Shader) { shader = _Shader; }
        Ptr<GraphicsShader> GetShader() { return shader; }

        void SetMaterialFactor(MtrlFactor _factor) { const_data.factor = _factor; }
        virtual void UpdateData() override;
        void UpdateDataInst();

        void SetAnimation3D(bool _is_ture) { const_data.arr_anim[0] = (int)_is_ture; }
        void SetJointCount(int _joint_cnt) { const_data.arr_anim[1] = _joint_cnt; }

        virtual int Save(const wstring& _strRelativePath);

    private:
        virtual int Load(const wstring& _strFilePath);

        Ptr<GraphicsShader>    shader;
        MtrlConst              const_data;
        Ptr<Texture>           textures[TEX_END];
    };

}