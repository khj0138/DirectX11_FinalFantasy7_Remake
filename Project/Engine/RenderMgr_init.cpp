#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"

#include "StructuredBuffer.h"
#include "MRT.h"

#include "ResourceMgr.h"
#include "Texture.h"

namespace ff7r
{
    void RenderMgr::Init()
    {
        light2d_buff = new StructuredBuffer;
        light3d_buff = new StructuredBuffer;
        capture_buff = new StructuredBuffer;

        // MRT 생성
        CreateMRT();
    }

    void RenderMgr::CreateMRT()
    {
        vec2 _resolution = Device::GetInst()->GetRenderResolution();

        // ====================
        // SwapChain MRT 만들기
        // ====================
        {
            mrt[(UINT)MRT_TYPE::SWAPCHAIN] = new MRT;

            Ptr<Texture> _textures[8] = {};
            _textures[0] = ResourceMgr::GetInst()->FindRes<Texture>(L"RenderTargetTex");

            Ptr<Texture> _ds_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"DepthStencilTex");

            mrt[(UINT)MRT_TYPE::SWAPCHAIN]->Create(_textures, 1, _ds_tex);
            mrt[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(vec4(0.f, 0.f, 0.f, 0.f), 0);
        }


        {
            mrt[(UINT)MRT_TYPE::HDR_TARGET] = new MRT;

            Ptr<Texture> _textures[1] = {};
            _textures[0] = ResourceMgr::GetInst()->CreateTexture(L"HDR_Target", _resolution.x, _resolution.y
                , DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            //Ptr<Texture> _depth = ResourceMgr::GetInst()->CreateTexture(L"HDR_Depth_Target", _resolution.x, _resolution.y
            //    , DXGI_FORMAT_D32_FLOAT
            //    , D3D11_BIND_DEPTH_STENCIL);

            mrt[(UINT)MRT_TYPE::HDR_TARGET]->Create(_textures, 1, nullptr);
        }

        // ====================
        // ShadowMap MRT 만들기
        // ====================
        {
            mrt[(UINT)MRT_TYPE::SHADOWMAP] = new MRT;

            vec2 _shadow_resolution = vec2(8192, 8192);

            Ptr<Texture> _textures[8] = {};
            _textures[0] = ResourceMgr::GetInst()->CreateTexture(L"DynamicShadowMapTex", _shadow_resolution.x, _shadow_resolution.y
                , DXGI_FORMAT_R32_FLOAT
                , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

            Ptr<Texture> pDSTex = ResourceMgr::GetInst()->CreateTexture(L"DynamicShadowMapDepthTex", _shadow_resolution.x, _shadow_resolution.y
                , DXGI_FORMAT_D32_FLOAT
                , D3D11_BIND_DEPTH_STENCIL);

            mrt[(UINT)MRT_TYPE::SHADOWMAP]->Create(_textures, 1, pDSTex);
        }



        // ============= PBR Rendering =====================

        {
            mrt[(UINT)MRT_TYPE::DEFERRED] = new MRT;

            Ptr<Texture> deferred_textures[8] = {};
            deferred_textures[0] = ResourceMgr::GetInst()->CreateTexture(L"ColorMetalicTarget", _resolution.x, _resolution.y
                , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            deferred_textures[1] = ResourceMgr::GetInst()->CreateTexture(L"NormalRoughnessTarget", _resolution.x, _resolution.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            deferred_textures[2] = ResourceMgr::GetInst()->CreateTexture(L"PositionTarget", _resolution.x, _resolution.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            deferred_textures[3] = ResourceMgr::GetInst()->CreateTexture(L"EmissiveOcclusionTarget", _resolution.x, _resolution.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            mrt[(UINT)MRT_TYPE::DEFERRED]->Create(deferred_textures, 4, nullptr);
        }

        {
            mrt[(UINT)MRT_TYPE::DEFERRED_CAPTURE] = new MRT;

            Ptr<Texture> deferred_textures[8] = {};
            deferred_textures[0] = ResourceMgr::GetInst()->CreateTexture(L"CaptureColorMetalicTarget", 512, 512
                , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            deferred_textures[1] = ResourceMgr::GetInst()->CreateTexture(L"CaptureNormalRoughnessTarget", 512, 512
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            deferred_textures[2] = ResourceMgr::GetInst()->CreateTexture(L"CapturePositionTarget", 512, 512
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            deferred_textures[3] = ResourceMgr::GetInst()->CreateTexture(L"CaptureEmissiveOcclusionTarget", 512, 512
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);



            // DepthStencil 용도 텍스쳐 생성
            Ptr<Texture> d =  ResourceMgr::GetInst()->CreateTexture(L"CaptureDepthStencilTex"
                , 512, 512
                , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

            mrt[(UINT)MRT_TYPE::DEFERRED_CAPTURE]->Create(deferred_textures, 4, d);
        }

        {
            mrt[(UINT)MRT_TYPE::AMBIENT_LIGHT] = new MRT;

            Ptr<Texture> _textures[1] = {};
            _textures[0] = ResourceMgr::GetInst()->CreateTexture(L"AmbientLightTarget", _resolution.x, _resolution.y
                , DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            mrt[(UINT)MRT_TYPE::AMBIENT_LIGHT]->Create(_textures, 1, nullptr);
        }


        {
            mrt[(UINT)MRT_TYPE::LIGHT] = new MRT;

            Ptr<Texture> _textures[1] = {};
            _textures[0] = ResourceMgr::GetInst()->CreateTexture(L"LightTarget", _resolution.x, _resolution.y
                , DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

            mrt[(UINT)MRT_TYPE::LIGHT]->Create(_textures, 1, nullptr);
        }

    }

    void RenderMgr::ClearMRT()
    {
        for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
        {
            if (nullptr != mrt[i])
            {
                mrt[i]->ClearTarget();
            }
        }
    }

    void RenderMgr::ClearMRT(MRT_TYPE _type)
    {
        mrt[(UINT)_type]->ClearTarget();
    }
}