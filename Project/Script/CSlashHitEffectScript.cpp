#include "pch.h"

#include <Engine\random_number.h>

#include "EffectScript.h"

#include "CSlashHitEffectScript.h"
namespace ff7r
{
	CSlashHitEffectScript::CSlashHitEffectScript()
		: Script((UINT)SCRIPT_TYPE::CSLASHHITEFFECTSCRIPT)
		, time(0.f)
	{
	}

	CSlashHitEffectScript::~CSlashHitEffectScript()
	{
	}

	void CSlashHitEffectScript::Awake()
	{
		scale = vec2(randf(6.5f) + 3.0f, randf(4.5f) + 2.0f);
		vec3 _rand_rot = vec3(randf(XM_2PI), randf(XM_2PI), randf(XM_2PI));

		GetTransform()->SetRelativeScale(scale.x, scale.y, 1.0f);
		GetTransform()->SetRelativeRot(_rand_rot);
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(220.f / 255.f, 87.f / 255.f, 27.f / 255.f, 1.0f));
		GetOwner()->GetScript<EffectScript>()->SetIntensity(4.0f);
		GetOwner()->GetScript<EffectScript>()->SetRangeX(vec2(0.48f, 0.52f));
		GetOwner()->GetScript<EffectScript>()->SetRangeY(vec2(0.5f, 1.0f));
		GetOwner()->GetScript<EffectScript>()->IsChangeRange(false, false);
	}

	void CSlashHitEffectScript::Tick()
	{
		time += DT;
		//vec2 _scale =  vec2::Lerp(scale, vec2(0.f, 0.f), time / 0.1f);
		float x = lerp(1.0f, 0.0f, time / 0.1f);
		GetTransform()->SetRelativeScale(scale.x, scale.y, x);
		if (time >= 0.1f)
		{
			DestroyObject(GetOwner());
		}
		//float y = lerp()
		//GetOwner()->GetScript<EffectScript>()->SetRangeY(vec2(0.5f, (time / 0.15f) * 0.5f + 0.5f));
	}
}
