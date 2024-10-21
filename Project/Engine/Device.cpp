#include "pch.h"
#include "Device.h"

#include "Engine.h"
#include "ConstBuffer.h"

#include "ResourceMgr.h"

namespace ff7r
{
    Device::Device()
        : hwnd(nullptr)
        , viewport{}
        , constbuffer{}
    {
    }

    Device::~Device()
    {
        SafeDelArray(constbuffer);
    }

    int Device::Init(HWND _hwnd, UINT _width, UINT _height)
    {
        hwnd = _hwnd;
        render_resolution = vec2((float)_width, (float)_height);

        g_global_data.resolution = render_resolution;



        int _flag = 0;
#ifdef _DEBUG
        _flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL _level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;



        if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
            , nullptr, _flag
            , nullptr, 0
            , D3D11_SDK_VERSION
            , device.GetAddressOf(), &_level
            , context.GetAddressOf())))
        {
            MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }


        HRESULT hr = device->CreateDeferredContext(0, &res_context);
        
        if (FAILED(CreateSwapChain()))
        {
            MessageBox(nullptr, L"����ü�� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }

        if (FAILED(CreateView()))
        {
            MessageBox(nullptr, L"View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }

        // ViewPort ����
        viewport.TopLeftX = 0.f;
        viewport.TopLeftY = 0.f;

        vec2 _win_resolution = Engine::GetInst()->GetWindowResolution();
        viewport.Width = _win_resolution.x;
        viewport.Height = _win_resolution.y;

        viewport.MinDepth = 0.f;
        viewport.MaxDepth = 1.f;

        context->RSSetViewports(1, &viewport);


        // RasterizerState ����
        if (FAILED(CreateRasterizerState()))
        {
            MessageBox(nullptr, L"�����Ͷ����� ������Ʈ ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }

        // DepthStencilState ����
        if (FAILED(CreateDepthStencilState()))
        {
            MessageBox(nullptr, L"DepthStencilState ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }

        // BlendState ����
        if (FAILED(CreateBlendState()))
        {
            MessageBox(nullptr, L"BlendState ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }

        // ���÷� ����
        if (FAILED(CreateSampler()))
        {
            MessageBox(nullptr, L"���÷� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
            return E_FAIL;
        }

        // ������� ����
        CreateConstBuffer();


        return S_OK; // E_FAIL;
    }

    void Device::ExcuteCommadList()
    {
        Microsoft::WRL::ComPtr<ID3D11CommandList> commandList;
        HRESULT hr = res_context->FinishCommandList(FALSE, &commandList);
        if (hr == S_OK)
        {
            context->ExecuteCommandList(commandList.Get(), FALSE);

            //context->Flush();

            CONTEXT->VSSetSamplers(0, 1, sampler[0].GetAddressOf());
            CONTEXT->HSSetSamplers(0, 1, sampler[0].GetAddressOf());
            CONTEXT->DSSetSamplers(0, 1, sampler[0].GetAddressOf());
            CONTEXT->GSSetSamplers(0, 1, sampler[0].GetAddressOf());
            CONTEXT->PSSetSamplers(0, 1, sampler[0].GetAddressOf());

            CONTEXT->VSSetSamplers(1, 1, sampler[1].GetAddressOf());
            CONTEXT->HSSetSamplers(1, 1, sampler[1].GetAddressOf());
            CONTEXT->DSSetSamplers(1, 1, sampler[1].GetAddressOf());
            CONTEXT->GSSetSamplers(1, 1, sampler[1].GetAddressOf());
            CONTEXT->PSSetSamplers(1, 1, sampler[1].GetAddressOf());

            CONTEXT->VSSetSamplers(2, 1, sampler[2].GetAddressOf());
            CONTEXT->HSSetSamplers(2, 1, sampler[2].GetAddressOf());
            CONTEXT->DSSetSamplers(2, 1, sampler[2].GetAddressOf());
            CONTEXT->GSSetSamplers(2, 1, sampler[2].GetAddressOf());
            CONTEXT->PSSetSamplers(2, 1, sampler[2].GetAddressOf());

            CONTEXT->VSSetSamplers(3, 1, sampler[3].GetAddressOf());
            CONTEXT->HSSetSamplers(3, 1, sampler[3].GetAddressOf());
            CONTEXT->DSSetSamplers(3, 1, sampler[3].GetAddressOf());
            CONTEXT->GSSetSamplers(3, 1, sampler[3].GetAddressOf());
            CONTEXT->PSSetSamplers(3, 1, sampler[3].GetAddressOf());

            CONTEXT->VSSetSamplers(4, 1, sampler[4].GetAddressOf());
            CONTEXT->HSSetSamplers(4, 1, sampler[4].GetAddressOf());
            CONTEXT->DSSetSamplers(4, 1, sampler[4].GetAddressOf());
            CONTEXT->GSSetSamplers(4, 1, sampler[4].GetAddressOf());
            CONTEXT->PSSetSamplers(4, 1, sampler[4].GetAddressOf());

        }
    }

    int Device::CreateSwapChain()
    {
        // ����ü�� ����
        DXGI_SWAP_CHAIN_DESC _desc = {};

        _desc.OutputWindow = hwnd;    // ��� ������
        _desc.Windowed = true;          // â���, ��üȭ�� ���

        _desc.BufferCount = 1;
        _desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        _desc.BufferDesc.Width = (UINT)render_resolution.x;
        _desc.BufferDesc.Height = (UINT)render_resolution.y;
        _desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        _desc.BufferDesc.RefreshRate.Denominator = 1;
        _desc.BufferDesc.RefreshRate.Numerator = 60;
        _desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        _desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        _desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

        _desc.SampleDesc.Count = 1;
        _desc.SampleDesc.Quality = 0;
        _desc.Flags = 0;


        // ����ü�� ����
        ComPtr<IDXGIDevice>  _dxgid_device;
        ComPtr<IDXGIAdapter> _adapter;
        ComPtr<IDXGIFactory> _factory;

        HRESULT hr = S_OK;

        hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)_dxgid_device.GetAddressOf());
        hr = _dxgid_device->GetParent(__uuidof(IDXGIAdapter), (void**)_adapter.GetAddressOf());
        hr = _adapter->GetParent(__uuidof(IDXGIFactory), (void**)_factory.GetAddressOf());

        hr = _factory->CreateSwapChain(device.Get(), &_desc, swapchain.GetAddressOf());

        return hr;
    }

    int Device::CreateView()
    {
        ComPtr<ID3D11Texture2D> _tex;
        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)_tex.GetAddressOf());

        // RenderTarget �� �ؽ��� ���
        ResourceMgr::GetInst()->CreateTexture(L"RenderTargetTex", _tex);

        // DepthStencil �뵵 �ؽ��� ����
        ResourceMgr::GetInst()->CreateTexture(L"DepthStencilTex"
            , (UINT)render_resolution.x, (UINT)render_resolution.y
            , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);


        return S_OK;
    }

    int Device::CreateRasterizerState()
    {
        rs_state[(UINT)RS_TYPE::CULL_BACK] = nullptr;

        D3D11_RASTERIZER_DESC _desc = {};
        _desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
        _desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        DEVICE->CreateRasterizerState(&_desc, rs_state[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

        _desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        _desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        DEVICE->CreateRasterizerState(&_desc, rs_state[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

        _desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        _desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        DEVICE->CreateRasterizerState(&_desc, rs_state[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

        return S_OK;
    }

    int Device::CreateDepthStencilState()
    {
        // Less
        ds_state[(UINT)DS_TYPE::LESS] = nullptr;

        // Less Equal
        D3D11_DEPTH_STENCIL_DESC _desc = {};
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        _desc.StencilEnable = false;
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DEVICE->CreateDepthStencilState(&_desc, ds_state[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

        // Greater
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_GREATER;
        _desc.StencilEnable = false;
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DEVICE->CreateDepthStencilState(&_desc, ds_state[(UINT)DS_TYPE::GREATER].GetAddressOf());

        // Greater Equal
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
        _desc.StencilEnable = false;
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DEVICE->CreateDepthStencilState(&_desc, ds_state[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());

        // No Write
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_LESS;
        _desc.StencilEnable = false;
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        DEVICE->CreateDepthStencilState(&_desc, ds_state[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

        // NoTest NoWrite
        _desc.DepthEnable = false;
        _desc.StencilEnable = false;
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        DEVICE->CreateDepthStencilState(&_desc, ds_state[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());


        // ���� �޽��� BackFace ���� �տ� �ִ� ������ stencil �� ����
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_GREATER;          // �������Ǵ� �����޽��� �޸��� �� �־�� ���
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // �����޽��� ���̸� ������ ����

        _desc.StencilEnable = true;                          // ���ٽ� On

        _desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;  // ���ٽ� �׽�Ʈ�� �׻� ���
        _desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;    // ���ٽ� �� 1 ����
        _desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;    // ����
        _desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;    // ���� 


        // ���� �޽��� FrontFace ���� �ڿ� �ִ� ������ stencil �� ����
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_LESS;             // �������Ǵ� �����޽��� �ո��� �� ������� ���
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // �����޽��� ���̸� ������ ����

        _desc.StencilEnable = true;                          // ���ٽ� On

        _desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
        _desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;   // ����
        _desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // ����
        _desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;   // ���� �׽�Ʈ ���� -> ���ٽ� �� ����


        // ���� �޽����� ����, ���ٽǰ� ���� (CULL_NONE)
        _desc.DepthEnable = true;
        _desc.DepthFunc = D3D11_COMPARISON_GREATER;             // �������Ǵ� �����޽��� �ո��� �� ������� ���
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // �����޽��� ���̸� ������ ����

        _desc.StencilEnable = true;                          // ���ٽ� On

        _desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        _desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;   // 
        _desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // ����
        _desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // ���� �׽�Ʈ ���� -> ���ٽ� �� ����

        _desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        _desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR;   // ����
        _desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // ����
        _desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;   // ���� �׽�Ʈ ���� -> ���ٽ� �� ����

        // ���ٽ� ���� 1�� �����ϰ� �ִ� ������ ���ؼ� ���
        _desc.DepthEnable = false;
        _desc.DepthFunc = D3D11_COMPARISON_GREATER;           // �������Ǵ� �����޽��� �޸��� �� �־�� ���
        _desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;   // �����޽��� ���̸� ������ ����

        _desc.StencilEnable = true;                           // ���ٽ� On

        _desc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;  // ���ٽ� �׽�Ʈ�� �׻� ���
        _desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;        // ���ٽ� �� 0
        _desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;        // ���ٽ� �� 0
        _desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;   // ���ٽ� �� 0


        return S_OK;
    }

    int Device::CreateBlendState()
    {
        // No Blend
        bs_state[(UINT)BS_TYPE::DEFAULT] = nullptr;

        // Mask
        D3D11_BLEND_DESC _desc = {};
        _desc.AlphaToCoverageEnable = true;
        _desc.IndependentBlendEnable = false;

        _desc.RenderTarget[0].BlendEnable = true;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        DEVICE->CreateBlendState(&_desc, bs_state[(UINT)BS_TYPE::MASK].GetAddressOf());


        // Alpha Blend
        _desc.AlphaToCoverageEnable = false;
        _desc.IndependentBlendEnable = false;

        _desc.RenderTarget[0].BlendEnable = true;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        DEVICE->CreateBlendState(&_desc, bs_state[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());




        // One One
        _desc.AlphaToCoverageEnable = false;
        _desc.IndependentBlendEnable = false;

        _desc.RenderTarget[0].BlendEnable = true;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        DEVICE->CreateBlendState(&_desc, bs_state[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());


        // DEFEREED_DECAL_BLEND
        _desc.AlphaToCoverageEnable = false;
        _desc.IndependentBlendEnable = true;

        _desc.RenderTarget[0].BlendEnable = true;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


        _desc.RenderTarget[1].BlendEnable = true;
        _desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
        _desc.RenderTarget[1].DestBlend = D3D11_BLEND_ONE;

        _desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ZERO;
        _desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ONE;

        _desc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        DEVICE->CreateBlendState(&_desc, bs_state[(UINT)BS_TYPE::DEFEREED_DECAL_BLEND].GetAddressOf());

        return S_OK;
    }







    int Device::CreateSampler()
    {
        D3D11_SAMPLER_DESC _desc = {};

        _desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.Filter = D3D11_FILTER_ANISOTROPIC;
        DEVICE->CreateSamplerState(&_desc, sampler[0].GetAddressOf());

        _desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        DEVICE->CreateSamplerState(&_desc, sampler[1].GetAddressOf());

        _desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        DEVICE->CreateSamplerState(&_desc, sampler[2].GetAddressOf());

        _desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.MinLOD = 0.0f;
        _desc.MaxLOD = 5.0f; // ���߿� �� �����ؾ� �� ����
        _desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        DEVICE->CreateSamplerState(&_desc, sampler[3].GetAddressOf());

        _desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        _desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        _desc.MinLOD = 0.0f;
        _desc.MaxLOD = D3D11_FLOAT32_MAX; 
        _desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        DEVICE->CreateSamplerState(&_desc, sampler[4].GetAddressOf());

        CONTEXT->VSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->HSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->DSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->GSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->PSSetSamplers(0, 1, sampler[0].GetAddressOf());

        CONTEXT->VSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->HSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->DSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->GSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->PSSetSamplers(1, 1, sampler[1].GetAddressOf());

        CONTEXT->VSSetSamplers(2, 1, sampler[2].GetAddressOf());
        CONTEXT->HSSetSamplers(2, 1, sampler[2].GetAddressOf());
        CONTEXT->DSSetSamplers(2, 1, sampler[2].GetAddressOf());
        CONTEXT->GSSetSamplers(2, 1, sampler[2].GetAddressOf());
        CONTEXT->PSSetSamplers(2, 1, sampler[2].GetAddressOf());

        CONTEXT->VSSetSamplers(3, 1, sampler[3].GetAddressOf());
        CONTEXT->HSSetSamplers(3, 1, sampler[3].GetAddressOf());
        CONTEXT->DSSetSamplers(3, 1, sampler[3].GetAddressOf());
        CONTEXT->GSSetSamplers(3, 1, sampler[3].GetAddressOf());
        CONTEXT->PSSetSamplers(3, 1, sampler[3].GetAddressOf());

        CONTEXT->VSSetSamplers(4, 1, sampler[4].GetAddressOf());
        CONTEXT->HSSetSamplers(4, 1, sampler[4].GetAddressOf());
        CONTEXT->DSSetSamplers(4, 1, sampler[4].GetAddressOf());
        CONTEXT->GSSetSamplers(4, 1, sampler[4].GetAddressOf());
        CONTEXT->PSSetSamplers(4, 1, sampler[4].GetAddressOf());

       //RES_CONTEXT->VSSetSamplers(0, 1, sampler[0].GetAddressOf());
       //RES_CONTEXT->HSSetSamplers(0, 1, sampler[0].GetAddressOf());
       //RES_CONTEXT->DSSetSamplers(0, 1, sampler[0].GetAddressOf());
       //RES_CONTEXT->GSSetSamplers(0, 1, sampler[0].GetAddressOf());
       //RES_CONTEXT->PSSetSamplers(0, 1, sampler[0].GetAddressOf());
       //
       //RES_CONTEXT->VSSetSamplers(1, 1, sampler[1].GetAddressOf());
       //RES_CONTEXT->HSSetSamplers(1, 1, sampler[1].GetAddressOf());
       //RES_CONTEXT->DSSetSamplers(1, 1, sampler[1].GetAddressOf());
       //RES_CONTEXT->GSSetSamplers(1, 1, sampler[1].GetAddressOf());
       //RES_CONTEXT->PSSetSamplers(1, 1, sampler[1].GetAddressOf());
       //
       //RES_CONTEXT->VSSetSamplers(2, 1, sampler[2].GetAddressOf());
       //RES_CONTEXT->HSSetSamplers(2, 1, sampler[2].GetAddressOf());
       //RES_CONTEXT->DSSetSamplers(2, 1, sampler[2].GetAddressOf());
       //RES_CONTEXT->GSSetSamplers(2, 1, sampler[2].GetAddressOf());
       //RES_CONTEXT->PSSetSamplers(2, 1, sampler[2].GetAddressOf());
       //
       //RES_CONTEXT->VSSetSamplers(3, 1, sampler[3].GetAddressOf());
       //RES_CONTEXT->HSSetSamplers(3, 1, sampler[3].GetAddressOf());
       //RES_CONTEXT->DSSetSamplers(3, 1, sampler[3].GetAddressOf());
       //RES_CONTEXT->GSSetSamplers(3, 1, sampler[3].GetAddressOf());
       //RES_CONTEXT->PSSetSamplers(3, 1, sampler[3].GetAddressOf());
       //
       //RES_CONTEXT->VSSetSamplers(4, 1, sampler[4].GetAddressOf());
       //RES_CONTEXT->HSSetSamplers(4, 1, sampler[4].GetAddressOf());
       //RES_CONTEXT->DSSetSamplers(4, 1, sampler[4].GetAddressOf());
       //RES_CONTEXT->GSSetSamplers(4, 1, sampler[4].GetAddressOf());
       //RES_CONTEXT->PSSetSamplers(4, 1, sampler[4].GetAddressOf());

        return S_OK;
    }

    void Device::CreateConstBuffer()
    {
        constbuffer[(UINT)CB_TYPE::TRANSFORM] = new ConstBuffer((UINT)CB_TYPE::TRANSFORM);
        constbuffer[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(TransformConst), 1);

        constbuffer[(UINT)CB_TYPE::MATERIAL] = new ConstBuffer((UINT)CB_TYPE::MATERIAL);
        constbuffer[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(MtrlConst), 1);

        constbuffer[(UINT)CB_TYPE::GLOBAL] = new ConstBuffer((UINT)CB_TYPE::GLOBAL);
        constbuffer[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(GlobalConst), 1);

        constbuffer[(UINT)CB_TYPE::CAPTURE] = new ConstBuffer((UINT)CB_TYPE::CAPTURE);
        constbuffer[(UINT)CB_TYPE::CAPTURE]->Create(sizeof(CaptureConst), 1);
    }
}