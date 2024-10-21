#include "pch.h"

#include "Engine.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "ResourceMgr.h"
#include "Device.h"
#include "MRT.h"
#include "RenderMgr.h"

#include "ConstBuffer.h"
#include "KeyMgr.h"

#include "Camera.h"
#include "ReflectionCapture.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"

namespace ff7r
{
	int ReflectionCapture::g_capture_id = 0;
	vector<Ptr<Texture>> ReflectionCapture::g_capture_array = {};
	Ptr<Texture> ReflectionCapture::g_capture_cube_array = {};
	Ptr<Texture> ReflectionCapture::g_irradiance_array = {};
	GameObject ReflectionCapture::g_capture_camera;

	ReflectionCapture::ReflectionCapture()
		: Component(COMPONENT_TYPE::REFLECTIONCAPTURE)
		, width(512)
		, height(512)
		, is_capture(false)
		, capture_id(g_capture_id++)
		, mip_level(6)
		, cube_array_slot(14)
	{
		D3D11_TEXTURE2D_DESC cubeDesc = {};
		cubeDesc.Width = width;
		cubeDesc.Height = height;
		cubeDesc.MipLevels = mip_level;
		cubeDesc.ArraySize = 6;
		cubeDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		cubeDesc.Usage = D3D11_USAGE_DEFAULT;
		cubeDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		cubeDesc.CPUAccessFlags = 0;
		cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		cubeDesc.SampleDesc.Count = 1;
		cubeDesc.SampleDesc.Quality = 0;

		capture_info.level = 1;
		capture_info.radius = 100.f;

		filter_environment_map = ResourceMgr::GetInst()->CreateCubeTexture(L"FilterCaptureTex_" + std::to_wstring(capture_id), cubeDesc, true);
		
		environment_map = ResourceMgr::GetInst()->FindRes<Texture>(L"DefaultPrefilterMap");
		light_capture = ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureLightTarget");

		light_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"DefaultLightMtrl")->Clone();
		light_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureColorMetalicTarget"));
		light_mtrl->SetTexParam(TEX_1, ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureNormalRoughnessTarget"));
		light_mtrl->SetTexParam(TEX_2, ResourceMgr::GetInst()->FindRes<Texture>(L"CapturePositionTarget"));
		light_mtrl->SetTexParam(TEX_3, ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureEmissiveOcclusionTarget"));

		merge_light_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"MergeAmbientMtrl");
		merge_light_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureColorMetalicTarget"));
		merge_light_mtrl->SetTexParam(TEX_3, ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureEmissiveOcclusionTarget"));
		merge_light_mtrl->SetTexParam(TEX_4, ResourceMgr::GetInst()->FindRes<Texture>(L"CaptureLightTarget"));

		pre_filter_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"PrefilterMapMtrl");

		if (capture_id == 0)
		{
			g_capture_camera.AddComponent(new Transform);
			g_capture_camera.AddComponent(new Camera);
			g_capture_camera.GetCamera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

			g_capture_cube_array = new Texture;
			g_irradiance_array = new Texture;
		}
	}

	ReflectionCapture::~ReflectionCapture()
	{
	}

	void ReflectionCapture::StartSetting()
	{
		is_capture = true;

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = width;
		viewport.Height = height;

		CONTEXT->RSSetViewports(1, &viewport);

		g_global_data.resolution.x = 512.f;
		g_global_data.resolution.y = 512.f;

		ConstBuffer* pGlobalBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
		pGlobalBuffer->SetData(&g_global_data, sizeof(GlobalConst));
		pGlobalBuffer->UpdateData();

		g_capture_camera.GetTransform()->SetRelativePos(GetTransform()->GetWorldPos());
	}	

	void ReflectionCapture::EndSetting()
	{
		CONTEXT->GenerateMips(environment_map->GetSRV().Get());

		D3D11_VIEWPORT _viewport;
		_viewport.TopLeftX = 0.f;
		_viewport.TopLeftY = 0.f;
		_viewport.MinDepth = 0.f;
		_viewport.MaxDepth = 1.f;

		pre_filter_mtrl->SetTexParam(TEX_CUBE_0, environment_map);
		for (int face = 0; face < 6; face++)
		{
			_viewport.Width = width;
			_viewport.Height = height;

			pre_filter_mtrl->SetScalarParam(INT_0, &face);

			for (int i = 0; i < mip_level; i++)
			{		
				CONTEXT->RSSetViewports(1, &_viewport);

				float roughness = (float)i / 6.0f;

				pre_filter_mtrl->SetScalarParam(FLOAT_0, &roughness);
				CONTEXT->OMSetRenderTargets(1, filter_environment_map->GetRTV(face * 6 + i).GetAddressOf(), nullptr);
				pre_filter_mtrl->UpdateData();
				CONTEXT->Draw(4, 0);

				_viewport.Width = _viewport.Width / 2;
				_viewport.Height = _viewport.Height / 2;
			}
		}
		// 기존 viewport로 설정
		{
			vec2 _resolution = Engine::GetInst()->GetWindowResolution();
	
			_viewport;
			_viewport.TopLeftX = 0.f;
			_viewport.TopLeftY = 0.f;

			_viewport.MinDepth = 0.f;
			_viewport.MaxDepth = 1.f;

			_viewport.Width = _resolution.x;
			_viewport.Height = _resolution.y;

			CONTEXT->RSSetViewports(1, &_viewport);
			g_global_data.resolution.x = _resolution.x;
			g_global_data.resolution.y = _resolution.y;
		}

		if ((int)g_capture_array.size() - 1 < capture_id)
		{
			g_capture_array.push_back(filter_environment_map);
		}
		else
		{
			g_capture_array[capture_id] = filter_environment_map;
		}

		g_capture_cube_array->CreateCubeArray(g_capture_array
			, g_capture_array.size(), width, height, mip_level, cube_array_slot);
		g_irradiance_array->CreateArray(g_capture_array.size());

		_viewport.Width = 32;
		_viewport.Height = 32;

		CONTEXT->RSSetViewports(1, &_viewport);
		CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"IrradianceMtrl");

		for (UINT i = 0; i < g_capture_array.size(); i++)
		{
			_mtrl->SetTexParam(TEX_CUBE_0, g_capture_array[i]);
			_mtrl->UpdateData();
			CONTEXT->OMSetRenderTargets(1, g_irradiance_array->GetRTV(i).GetAddressOf(), nullptr);
			CONTEXT->Draw(4, 0);
		}


		CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);

		CONTEXT->PSSetShaderResources(15, 1, g_irradiance_array->GetSRV().GetAddressOf());
		

		_viewport.Width = 1600;
		_viewport.Height = 900;

		CONTEXT->RSSetViewports(1, &_viewport);
	}

	void ReflectionCapture::UpdateCapture()
	{
	}

	void ReflectionCapture::Capture(int _idx)
	{
		switch (_idx)
		{
		case 0: g_capture_camera.GetTransform()->SetRelativeRot(vec3(0.f, XM_PI / 2.0f, 0.0f)); break;
		case 1: g_capture_camera.GetTransform()->SetRelativeRot(vec3(0.f, XM_PI / 2.0f * 3.0f, 0.0f)); break;
		case 2: g_capture_camera.GetTransform()->SetRelativeRot(vec3(XM_PI / 2.0f * 3.0f, 0.0f, 0.0f)); break;
		case 3: g_capture_camera.GetTransform()->SetRelativeRot(vec3(XM_PI / 2.0f, 0.0f, 0.0f)); break;
		case 4: g_capture_camera.GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));break;
		case 5: g_capture_camera.GetTransform()->SetRelativeRot(vec3(0.f, XM_PI, 0.0f)); break;
		}

		g_capture_camera.GetTransform()->FinalTick();

		CONTEXT->ClearRenderTargetView(environment_map->GetRTV(_idx).Get(), vec4(0.0f, 0.0f, 0.0f, 1.0f));
		CONTEXT->ClearRenderTargetView(light_capture->GetRTV().Get(), vec4(0.0f, 0.0f, 0.0f, 1.0f));

		Camera* _camera = g_capture_camera.GetCamera();

		_camera->SetFOV(XM_PI / 2.f);
		_camera->SetFar(capture_info.radius);
		_camera->SetNear(0.0001f);

		_camera->FinalTick();

		g_transform_data.mat_view = _camera->GetViewMat();
		g_transform_data.mat_view_inv = _camera->GetViewInvMat();
		g_transform_data.mat_proj = _camera->GetProjMat();
		g_transform_data.mat_proj_inv = _camera->GetProjInvMat();

		RenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED_CAPTURE)->OMSet(true);
		const vector<GameObject*>& _objects = LevelMgr::GetInst()->GetCurLevel()->GetLayer(1)->GetObjects();
		for (const auto& _object : _objects)
		{
			RenderComponent* _render = _object->GetRenderComponent();
			if (_render && _render->GetMaterial() != nullptr 
				&& _render->GetMaterial()->GetShader()->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
			{
				_object->Render();
			}
		}

		CONTEXT->OMSetRenderTargets(1, light_capture->GetRTV().GetAddressOf(), nullptr);
		light_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		CONTEXT->OMSetRenderTargets(1, environment_map->GetRTV(_idx).GetAddressOf(), nullptr);
		merge_light_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		RenderMgr::GetInst()->ClearMRT(MRT_TYPE::DEFERRED_CAPTURE);	
	}

	void ReflectionCapture::SaveToLevelFile(FILE* _File)
	{
		fwrite(&capture_info, sizeof(CaptureInfo), 1, _File);
	}

	void ReflectionCapture::LoadFromLevelFile(FILE* _FILE)
	{
		fread(&capture_info, sizeof(CaptureInfo), 1, _FILE);
	}


	void ReflectionCapture::Awake()
	{
		RenderMgr::GetInst()->RegisterUpdateCapture(this);
	}

	void ReflectionCapture::Begin()
	{
	}

	void ReflectionCapture::FinalTick()
	{
		capture_info.world_pos = GetTransform()->GetRelativePos();

		if (is_capture)
		{
			RenderMgr::GetInst()->RegisterCapture(this, capture_info);
		}

		//DrawDebugSphere(capture_info.world_pos, capture_info.radius, vec4(0.0f, 1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f));
	}
}