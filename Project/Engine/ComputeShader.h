#pragma once
#include "Shader.h"

namespace ff7r
{
    class ComputeShader : public Shader
    {
    public:
        ComputeShader();
        ~ComputeShader();

        void CreateComputeShader(const wstring& _strFileName, const string& _strFuncName);
        void Execute();

    protected:
        MtrlConst                       mtrl_const_data;

        // 그룹 개수
        UINT                            group_x_cnt;
        UINT                            group_y_cnt;
        UINT                            group_z_cnt;

        // 그룹 1개당 스레드 개수
        UINT                            thread_x_per_group;
        UINT                            thread_y_per_group;
        UINT                            thread_z_per_group;

    private:
        CLONE_DISABLE(ComputeShader);

        virtual void UpdateData() = 0;
        virtual void Clear() = 0;

        ComPtr<ID3D11ComputeShader>     compute_shader;
        ComPtr<ID3DBlob>                compute_shader_blob;
    };

}