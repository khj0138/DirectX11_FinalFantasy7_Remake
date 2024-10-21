#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "StructuredBuffer.h"

#include "BlurShader.h"

#include "Camera.h"
#include "Light2D.h"
#include "Light3D.h"
#include "Transform.h"

#include "RenderComponent.h"
#include "ResourceMgr.h"
#include "ReflectionCapture.h"
#include "MRT.h"

#include "Layer.h"
#include "Level.h"
#include "LevelMgr.h"

namespace ff7r
{
	RenderMgr::RenderMgr()
		: light2d_buff(nullptr)
		, light3d_buff(nullptr)
		, capture_buff(nullptr)
		, RENDER_FUNC(nullptr)
		, editor_camera(nullptr)
		, mrt{}
		, ambient_color(vec3(1.0f, 1.0f, 1.0f))
		, ambient_intensity(0.3f)
	{
		vec2 _resolution = Device::GetInst()->GetRenderResolution();
		copy_render_target = ResourceMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)_resolution.x, (UINT)_resolution.y
			, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

		blur_render_target = ResourceMgr::GetInst()->CreateTexture(L"BlurCopyTex", (UINT)_resolution.x, (UINT)_resolution.y
			, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		//ResourceMgr::GetInst()->FindRes<Material>(L"GrayMtrl")->SetTexParam(TEX_0, copy_render_target);
		//ResourceMgr::GetInst()->FindRes<Material>(L"DistortionMtrl")->SetTexParam(TEX_0, copy_render_target);
	}

	RenderMgr::~RenderMgr()
	{
		if (light2d_buff)
		{
			delete light2d_buff;
			light2d_buff = nullptr;
		}

		if (light3d_buff)
		{
			delete light3d_buff;
			light3d_buff = nullptr;
		}

		if (capture_buff)
		{
			delete capture_buff;
			capture_buff = nullptr;
		}

		SafeDelArray(mrt);
	}

	void RenderMgr::Render()
	{
		// 광원 및 전역 데이터 업데이트 및 바인딩
		//g_global_data.main_camera_pos = editor_camera->GetTransform()->GetRelativePos();
		//g_global_data.main_camera_pos = cameras[1]->GetTransform()->GetRelativePos();

		UpdateData();

		ClearMRT();
		//SortObjects();
		//RenderShadowMap();

		UpdateCapture();
		(this->*RENDER_FUNC)();

		Clear();
	}


	void RenderMgr::RenderPlay()
	{
		// 카메라 기준 렌더링
		g_global_data.main_camera_pos = cameras[0]->GetTransform()->GetRelativePos();
		for (size_t i = 0; i < cameras.size(); ++i)
		{
			if (nullptr == cameras[i])
				continue;

			// 물체 분류작업
			// - 해당 카메라가 볼 수 있는 물체(레이어 분류)
			// - 재질에 따른 분류 (재질->쉐이더) 쉐이더 도메인
			//   쉐이더 도메인에 따라서 렌더링 순서분류
			cameras[i]->SortObjects();
			cameras[i]->Render();
		}
	}

	void RenderMgr::RenderEditor()
	{
		g_global_data.main_camera_pos = editor_camera->GetTransform()->GetRelativePos();
		editor_camera->SortObjects();
		editor_camera->Render();

		if (cameras.size() >= 2)
		{
			cameras[1]->SortObjects();
			cameras[1]->Render();
		}
	}

	int RenderMgr::RegisterCamera(Camera* _camera, int _idx)
	{
		if (cameras.size() <= _idx)
		{
			cameras.resize(_idx + 1);
		}

		cameras[_idx] = _camera;
		return _idx;
	}

	void RenderMgr::RegisterCapture(ReflectionCapture* _capture, const CaptureInfo& _info)
	{
		if (capture.size() <= _capture->GetCaptureId())
		{
			capture.resize(_capture->GetCaptureId() + 1);
			capture_info.resize(_capture->GetCaptureId() + 1);
		}
		capture[_capture->GetCaptureId()] = _capture;
		capture_info[_capture->GetCaptureId()] = _info;
	}

	void RenderMgr::SetRenderFunc(bool _is_play)
	{
		if (_is_play)
		{
			RENDER_FUNC = &RenderMgr::RenderPlay;
		}
		else
		{
			RENDER_FUNC = &RenderMgr::RenderEditor;
		}
	}

	UINT RenderMgr::RegisterLight2D(Light2D* _light2d, const LightInfo& _info)
	{
		light2d.push_back(_light2d);
		light2d_info.push_back(_info);
		return light2d.size() - 1;
	}

	UINT RenderMgr::RegisterLight3D(Light3D* _light3d, const LightInfo& _info)
	{
		light3d.push_back(_light3d);
		light3d_info.push_back(_info);
		return light3d.size() - 1;
	}

	Camera* RenderMgr::GetMainCam()
	{
		if (cameras.empty())
		{
			return nullptr;
		}

		return cameras[0];
	}

	Camera* RenderMgr::GetCam(UINT _idx)
	{
		if (_idx < cameras.size())
			return cameras[_idx];
		return nullptr;
	}

	void RenderMgr::CopyRenderTarget()
	{
		Ptr<Texture> _render_target = ResourceMgr::GetInst()->FindRes<Texture>(L"RenderTargetTex");
		CONTEXT->CopyResource(copy_render_target->GetTex2D().Get(), _render_target->GetTex2D().Get());
	}
	void RenderMgr::BlurRenderTarget()
	{
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f / 4.f;
		viewport.Height = 900.f / 4.f;

		CONTEXT->RSSetViewports(1, &viewport);

		Ptr<Texture> _blur_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4");
		CONTEXT->OMSetRenderTargets(1, _blur_tex->GetRTV().GetAddressOf(), nullptr);
		Ptr<Material>_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, copy_render_target);
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
		Ptr<BlurShader> blur = (BlurShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"BlurCS").Get();

		blur->SetTexSize(vec2(1600.f / 4.f, 900.f / 4.f));
		blur->SetTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4"), ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4_2"));
		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f;
		viewport.Height = 900.f;

		CONTEXT->RSSetViewports(1, &viewport);


		_blur_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4");
		CONTEXT->OMSetRenderTargets(1, blur_render_target->GetRTV().GetAddressOf(), nullptr);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	}


	void RenderMgr::UpdateData()
	{
		if (light3d_buff->GetElementCount() < light3d_info.size())
		{
			light3d_buff->Create(sizeof(LightInfo), light3d_info.size(), SB_TYPE::READ_ONLY, true);
		}
		light3d_buff->SetData(light3d_info.data(), sizeof(LightInfo) * light3d_info.size());
		light3d_buff->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);
		
		if (capture_buff->GetElementCount() < capture.size())
		{
			capture_buff->Create(sizeof(CaptureInfo), capture.size(), SB_TYPE::READ_ONLY, true);
		}
		capture_buff->SetData(capture_info.data(), sizeof(CaptureInfo) * capture_info.size());
		capture_buff->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);


		// GlobalData 에 광원 개수정보 세팅
		//g_global_data.light2d_cnt = light2d.size();
		g_global_data.light_cnt = light3d.size();
		g_global_data.ambient_color = ambient_color;
		g_global_data.ambient_intensity = ambient_intensity;


		// 전역 상수 데이터 바인딩
		ConstBuffer* pGlobalBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
		pGlobalBuffer->SetData(&g_global_data, sizeof(GlobalConst));
		pGlobalBuffer->UpdateData();
		pGlobalBuffer->UpdateData_CS();

		CaptureConst c;
		c.min_level = 0;
		c.max_level = 10;
		c.object_cnt = capture.size();

		ConstBuffer* t = Device::GetInst()->GetConstBuffer(CB_TYPE::CAPTURE);
		t->SetData(&c, sizeof(CaptureConst));
		t->UpdateData();
		t->UpdateData_CS();
	}


	void RenderMgr::UpdateCapture()
	{
		for (size_t i = 0; i < update_capture.size(); i++)
		{
			update_capture[i]->StartSetting();

			for (size_t j = 0; j < 6; j++)
			{
				update_capture[i]->Capture(j);
			}
			
			update_capture[i]->EndSetting();
		}

		update_capture.clear();
	}

	void RenderMgr::RenderShadowMap()
	{
		// ShadowMap MRT 로 교체
		GetMRT(MRT_TYPE::SHADOWMAP)->OMSet();

		for (size_t i = 0; i < light3d.size(); ++i)
		{
			//light3d[i]->RenderShadowMap();
		}
	}


	void RenderMgr::Clear()
	{
		light2d.clear();
		light2d_info.clear();
		light3d.clear();
		light3d_info.clear();	
		// capture.clear();
		// capture_info.clear();
	}
}