#include "pch.h"
#include "Light3D.h"

#include "ResourceMgr.h"

#include "RenderMgr.h"
#include "Transform.h"
#include "Camera.h"

namespace ff7r
{
	Light3D::Light3D()
		: Component(COMPONENT_TYPE::LIGHT3D)
		, is_show_range(false)
		, light_idx(-1)
	{
		SetLightType(LIGHT_TYPE::DIRECTIONAL);
	}

	Light3D::Light3D(const Light3D& _Origin)
		: Component(_Origin)
		, is_show_range(_Origin.is_show_range)
		, light_idx(-1)
		, light_info(_Origin.light_info)
	{
	}

	Light3D::~Light3D()
	{
	}

	void Light3D::FinalTick()
	{
		light_info.world_pos = GetTransform()->GetWorldPos();
		light_info.world_dir = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);

		light_idx = (UINT)RenderMgr::GetInst()->RegisterLight3D(this, light_info);
		
	}

	void Light3D::Render()
	{
		GetTransform()->UpdateData();

		// Light 재질 업데이트
		material->SetScalarParam(INT_0, &light_idx);

		//if (light_info.light_type == (UINT)LIGHT_TYPE::DIRECTIONAL)
		//{
		//	Matrix matVP = camera->GetCamera()->GetViewMat() * camera->GetCamera()->GetProjMat();
		//	material->SetScalarParam(MAT_0, &matVP);
		//	material->SetTexParam(TEX_2, ResourceMgr::GetInst()->FindRes<Texture>(L"DynamicShadowMapTex"));
		//}

		material->UpdateData();

		// 볼륨 메시 렌더
		mesh->Render();
	}

	void Light3D::SetRadius(float _radius)
	{
		light_info.attenuation_radius = _radius;

		// SphereMesh 의 로컬 반지름이 0.5f 이기 때문에 2배로 적용
		//GetTransform()->SetRelativeScale(vec3(_radius * 2.f, _radius * 2.f, _radius * 2.f));
	}


	void Light3D::SetLightType(LIGHT_TYPE _type)
	{
		light_info.light_type = (int)_type;

		//if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)light_info.light_type)
		//{
		//	// 광원을 렌더링 할 때, 광원의 영향범위를 형상화 할 수 있는 메쉬(볼륨메쉬) 를 선택
		//	mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh");
		//	material = ResourceMgr::GetInst()->FindRes<Material>(L"DirLightMtrl");
		//
		//	camera->GetCamera()->SetFar(100000.f);
		//	camera->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		//	camera->GetCamera()->SetOrthoWidth(16000.f);
		//	camera->GetCamera()->SetOrthoHeight(16000.f);
		//}
		//
		//else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)light_info.light_type)
		//{
		//	mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"SphereMesh");
		//	material = ResourceMgr::GetInst()->FindRes<Material>(L"PointLightMtrl");
		//}
		//
		//else
		//{
		//	mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"ConeMesh");
		//	material = ResourceMgr::GetInst()->FindRes<Material>(L"SpotLightMtrl");
		//}
		//
		//if (nullptr != material)
		//{
		//	material->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"NormalTargetTex"));
		//	material->SetTexParam(TEX_1, ResourceMgr::GetInst()->FindRes<Texture>(L"PositionTargetTex"));
		//}
	}


	void Light3D::RenderShadowMap()
	{
	}


	void Light3D::SaveToLevelFile(FILE* _File)
	{
		fwrite(&light_info, sizeof(LightInfo), 1, _File);
	}

	void Light3D::LoadFromLevelFile(FILE* _File)
	{
		fread(&light_info, sizeof(LightInfo), 1, _File);
	}
}