#include "pch.h"

#include <Engine\random_number.h>

#include "EffectScript.h"

#include "CWeaponSparkScript.h"
namespace ff7r
{
	CWeaponSparkScript::CWeaponSparkScript()
		: Script((UINT)SCRIPT_TYPE::CWEAPONSPARKSCRIPT)
	{
	}

	CWeaponSparkScript::~CWeaponSparkScript()
	{
	}

	void CWeaponSparkScript::Awake()
	{
		GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"SphereMesh"));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Grd_03_T.png"));
		GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(235.f / 255.f, 47.f / 255.f, 27.f / 255.f, 1.0f));
		GetOwner()->GetScript<EffectScript>()->SetIntensity(4.0f);
	}

	void CWeaponSparkScript::Tick()
	{
		if (is_first)
		{
			is_first = false;
			return;
		}

		if (time <= 0.0f)
		{
			GetOwner()->SetActive(false);
		}

		float _intensity = lerp(3.5f, 6.0f, time / 0.08f);
		float _scale_yz = lerp(0.0f, 0.05f, time / 0.08f);
		float _scale_x = GetTransform()->GetRelativeScale().x;

		GetOwner()->GetScript<EffectScript>()->SetIntensity(_intensity);
		GetTransform()->SetRelativeScale(_scale_x, _scale_yz, _scale_yz);
		time -= DT;

	}

	void CWeaponSparkScript::OnEnable()
	{
		float _scale_x = randf(3.0f) + 0.5f;
		float _rot_y = randf(Degree2Rad(60.f)) - Degree2Rad(30.f);
		float _rot_z = randf(Degree2Rad(60.f)) - Degree2Rad(30.f);

		GetTransform()->SetRelativeScale(_scale_x, 0.05f, 0.05f);
		GetTransform()->SetRelativeRot(1.0f, _rot_y, _rot_z);

		is_first = true;
		time = 0.08f;
	}

	void CWeaponSparkScript::OnDisable()
	{
	}

	void CWeaponSparkScript::SetSpawnPosition(vec3 _pos, vec3 _dir)
	{
		Transform* trans = GetTransform();

		vec3 _rot = CalcEulerAngleFromVectors(trans->GetRelativeRot(), _dir);
		trans->SetRelativeRot(_rot);
		trans->FinalTick();

		vec3 _right = trans->GetWorldDir(DIR_TYPE::RIGHT);
		trans->SetRelativePos(_pos + _right * trans->GetRelativeScale().x * 0.5f);
	}
}