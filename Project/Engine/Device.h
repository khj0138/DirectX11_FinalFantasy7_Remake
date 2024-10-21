#pragma once

#include "ptr.h"
#include "Texture.h"

namespace ff7r
{
	class ConstBuffer;
	class Device : public Singleton<Device>
	{
	public:
		Device();
		~Device();

		int		Init(HWND _hWnd, UINT _width, UINT _height);
		void	Present() { swapchain->Present(0, 0); }

		vec2	GetRenderResolution() { return render_resolution; }

		ID3D11Device* GetDevice() { return device.Get(); }
		ID3D11DeviceContext* GetDeviceContext() { return context.Get(); }
		ID3D11DeviceContext* GetDeviceResContext() { return res_context.Get(); }

		ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _type) { return rs_state[(UINT)_type]; }
		ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _type) { return ds_state[(UINT)_type]; }
		ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _type) { return bs_state[(UINT)_type]; }

		ConstBuffer* GetConstBuffer(CB_TYPE _type) { return constbuffer[(UINT)_type]; }

		void ExcuteCommadList();
	private:
		int		CreateSwapChain();
		int		CreateView();
		int		CreateRasterizerState();
		int		CreateBlendState();
		int		CreateDepthStencilState();
		int		CreateSampler();
		void	CreateConstBuffer();

		HWND							hwnd;

		ComPtr<ID3D11Device>			device;
		ComPtr<ID3D11DeviceContext>		context;
		ComPtr<ID3D11DeviceContext>		res_context;
		ComPtr<IDXGISwapChain>			swapchain;
		ComPtr<ID3D11SamplerState>		sampler[5];
		ComPtr<ID3D11RasterizerState>	rs_state[(UINT)RS_TYPE::END];
		ComPtr<ID3D11DepthStencilState>	ds_state[(UINT)DS_TYPE::END];
		ComPtr<ID3D11BlendState>		bs_state[(UINT)BS_TYPE::END];

		D3D11_VIEWPORT					viewport;

		vec2							render_resolution;

		ConstBuffer* constbuffer[(UINT)CB_TYPE::END];
	};
}
