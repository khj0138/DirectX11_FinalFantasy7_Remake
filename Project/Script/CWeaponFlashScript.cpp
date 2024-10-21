#include "pch.h"

#include "EffectScript.h"

#include "CWeaponFlashScript.h"
namespace ff7r
{
	CWeaponFlashScript::CWeaponFlashScript()
		: Script((UINT)SCRIPT_TYPE::CWEAPONFLASHSCRIPT)
		, flash_time(0.5f)
		, time(flash_time)
	{
	}

	CWeaponFlashScript::~CWeaponFlashScript()
	{
	}

	void CWeaponFlashScript::Awake()
	{
	}

	void CWeaponFlashScript::Tick()
	{
		time -= DT;

		if (time <= 0.0f)
		{
			time = flash_time;
		}

		float _scale_x = lerp(2.0f, 2.f, time / flash_time);
		float _intensity = lerp(0.01f, 12.f, time / flash_time);

		GetOwner()->GetScript<EffectScript>()->SetIntensity(_intensity);
		GetTransform()->SetRelativeScaleX(_scale_x);
	}

	void CWeaponFlashScript::OnEnable()
	{
		time = flash_time;
	}

	void CWeaponFlashScript::OnDisable()
	{
	}
}
