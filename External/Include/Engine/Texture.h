#pragma once
#include "Resource.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

namespace ff7r
{
    class Texture : public Resource
    {
    public:
        Texture(bool _bEngine = false);
        ~Texture();

        int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage);
        int Create(ComPtr<ID3D11Texture2D> _tex2D);
        int CreateArray(UINT _arr_size);
        int CreateCube(D3D11_TEXTURE2D_DESC _desc ,bool _use_generate_mip  = false);
        int CreateCubeArray(vector<Ptr<Texture>> _cube_array, UINT _cnt, UINT _width, UINT _height, UINT _mip_level, UINT _slot);

        float Width() { return (float)desc.Width; }
        float Height() { return (float)desc.Height; }

        ComPtr<ID3D11Texture2D>             GetTex2D() { return texture2d; }
        ComPtr<ID3D11ShaderResourceView>    GetSRV() { return srv; }
        ComPtr<ID3D11RenderTargetView>	    GetRTV(UINT _idx = 0) { return rtv[_idx]; }
        ComPtr<ID3D11DepthStencilView>	    GetDSV() { return dsv; }
        ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return uav; }

        void UpdateData(int _iRegisterNum, int _PipelineStage);
        void UpdateDataCS(int _iRegisterNum, bool _bShaderRes);

        static void Clear(int _iRegisterNum);

        void Clear();
        void ClearCS(bool _bShaderRes);

        bool IsCube() { return desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE ? true : false; }

        virtual int Save(const wstring& _strRelativePath) override;

    private:
        virtual int Load(const wstring& _strFilePath) override;

        virtual void UpdateData() override;

        ComPtr<ID3D11Texture2D>                 texture2d;
        vector<ComPtr<ID3D11RenderTargetView>>  rtv; // render target views

        ComPtr<ID3D11ShaderResourceView>    srv; // shader resource view
        ComPtr<ID3D11DepthStencilView>	    dsv; // depth stencil view
        ComPtr<ID3D11UnorderedAccessView>   uav; // unordered acces view

        D3D11_TEXTURE2D_DESC                desc;
        ScratchImage                        image;

        UINT                                register_num;


    };
}
