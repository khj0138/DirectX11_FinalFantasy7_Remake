#include "pch.h"

#include "EffectScript.h"

#include "ElectricEffectScript.h"
namespace ff7r
{
	ElectricEffectScript::ElectricEffectScript()
		: Script((UINT)SCRIPT_TYPE::ELECTRICEFFECTSCRIPT)
	{
	}

	ElectricEffectScript::~ElectricEffectScript()
	{
	}

	void ElectricEffectScript::Awake()
	{
		GetTransform()->SetRelativeScale(5.f, 0.5f, 5.f);
		GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_1,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Elec_UV_03_T.png"));

		GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(VEC4_0, vec4(0.2f, 0.3f, 1.0f, 1.0f));
		GetOwner()->GetScript<EffectScript>()->SetIntensity(6.0f);
	}

	void ElectricEffectScript::Tick()
	{
	}

	void ElectricEffectScript::OnEnable()
	{
		GetOwner()->GetScript<EffectScript>()->SetRangeX(vec2(0.0f, 0.63f));
		GetOwner()->GetScript<EffectScript>()->SetIntensity(6.0f);
	}

	void ElectricEffectScript::OnDisable()
	{
	}

	void ElectricEffectScript::Next()
	{
		GetOwner()->GetScript<EffectScript>()->SetRangeX(vec2(0.0f, 1.0f));
	}
}