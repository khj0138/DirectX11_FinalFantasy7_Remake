#pragma once
#include "Shader.h"

namespace ff7r
{
    struct ScalarParam
    {
        SCALAR_PARAM    param;
        string          desc;
    };

    struct TexParam
    {
        TEX_PARAM       param;
        string          desc;
    };


    class GraphicsShader :
        public Shader
    {
    private:
        ComPtr<ID3DBlob>                vs_blob;
        ComPtr<ID3DBlob>				vs_inst_blob;
        ComPtr<ID3DBlob>                hs_blob;
        ComPtr<ID3DBlob>                ds_blob;
        ComPtr<ID3DBlob>                gs_blob;
        ComPtr<ID3DBlob>                ps_blob;

        ComPtr<ID3D11VertexShader>      vs;
        ComPtr<ID3D11VertexShader>      vs_inst;
        ComPtr<ID3D11HullShader>        hs;
        ComPtr<ID3D11DomainShader>      ds;
        ComPtr<ID3D11GeometryShader>    gs;
        ComPtr<ID3D11PixelShader>       ps;

        ComPtr<ID3D11InputLayout>	    input_layout;
        ComPtr<ID3D11InputLayout>	    input_layout_inst;
        D3D11_PRIMITIVE_TOPOLOGY        topology;

        RS_TYPE                         rs_type;
        DS_TYPE                         ds_type;
        BS_TYPE                         bs_type;

        SHADER_DOMAIN                   domain;

        vector<ScalarParam>             scalar_param;
        vector<TexParam>                tex_param;

    public:
        void CreateVertexShader(const wstring& _strFileName, const string& _strFuncName);
        void CreateInstVertexShader(const wstring& _strFileName, const string& _strFuncName);
        void CreateHullShader(const wstring& _strFileName, const string& _strFuncName);
        void CreateDomainShader(const wstring& _strFileName, const string& _strFuncName);
        void CreateGeometryShader(const wstring& _strFileName, const string& _strFuncName);
        void CreatePixelShader(const wstring& _strFileName, const string& _strFuncName);
        void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { topology = _Topology; }
        void SetRSType(RS_TYPE _Type) { rs_type = _Type; }
        void SetDSType(DS_TYPE _Type) { ds_type = _Type; }
        void SetBSType(BS_TYPE _Type) { bs_type = _Type; }
        void SetDomain(SHADER_DOMAIN _domain) { domain = _domain; }

        ComPtr<ID3D11VertexShader> GetVSInst() { return vs_inst; }
        SHADER_DOMAIN GetDomain() { return domain; }
        virtual void UpdateData() override;
        void UpdateDataInst();

        void AddScalarParam(SCALAR_PARAM _Param, const string& _desc) { scalar_param.push_back(ScalarParam{ _Param, _desc }); }
        void AddTexParam(TEX_PARAM _Param, const string& _desc) { tex_param.push_back(TexParam{ _Param, _desc }); }

        const vector<ScalarParam>& GetScalarParam() { return scalar_param; }
        const vector<TexParam>& GetTexParam() { return tex_param; }

    public:
        GraphicsShader();
        ~GraphicsShader();
    };
}
