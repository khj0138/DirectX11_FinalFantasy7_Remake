#include "pch.h"
#include "MRT.h"

#include "Device.h"

namespace ff7r
{
	MRT::MRT()
		: render_targets{}
		, rt_cnt(0)
		, viewport{}
	{
		for (int i = 0; i < 8; i++)
		{
			clear_color[i] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	MRT::~MRT()
	{
	}

	void MRT::Create(Ptr<Texture>* _textures, UINT _tex_cnt	, Ptr<Texture> _ds_tex)
	{
		for (UINT i = 0; i < _tex_cnt; ++i)
		{
			render_targets[i] = _textures[i];
		}

		rt_cnt = _tex_cnt;

		ds_tex = _ds_tex;

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		viewport.Width = _textures[0]->Width();
		viewport.Height = _textures[0]->Height();

		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
	}

	void MRT::ClearTarget()
	{
		for (UINT i = 0; i < rt_cnt; ++i)
		{
			CONTEXT->ClearRenderTargetView(render_targets[i]->GetRTV().Get(), clear_color[i]);
		}

		if (nullptr != ds_tex)
		{
			CONTEXT->ClearDepthStencilView(ds_tex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
		}
	}

	void MRT::OMSet(bool _bStay)
	{
		ID3D11RenderTargetView* _rtv[8] = {};
		for (UINT i = 0; i < rt_cnt; ++i)
		{
			_rtv[i] = render_targets[i]->GetRTV().Get();
		}

		if (nullptr != ds_tex)
		{
			CONTEXT->OMSetRenderTargets(rt_cnt, _rtv, ds_tex->GetDSV().Get());
		}
		else
		{
			ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

			if (_bStay)
			{
				CONTEXT->OMGetRenderTargets(0, nullptr, pDSV.GetAddressOf());
			}

			CONTEXT->OMSetRenderTargets(rt_cnt, _rtv, pDSV.Get());
		}

		CONTEXT->RSSetViewports(1, &viewport);
	}
}