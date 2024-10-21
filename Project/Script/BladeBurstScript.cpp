#include "pch.h"

#include <Engine\SoundMgr.h>

#include "ObjectPoolMgr.h"
#include "BladeScript.h"

#include "BladeBurstScript.h"
namespace ff7r
{
	BladeBurstScript::BladeBurstScript()
		: Script((UINT)SCRIPT_TYPE::BLADEBURSTSCRIPT)
		, time(0.0f)
		, blade_duration(0.0f)
		, blade_idx(0)
	{
	}

	BladeBurstScript::~BladeBurstScript()
	{
	}

	void BladeBurstScript::Awake()
	{
		GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_1,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Glow_06_T.png"));
		GetTransform()->SetRelativeScale(0.3f, 0.3f, 0.6f);
	}

	void BladeBurstScript::Tick()
	{
		time += DT;
		blade_duration += DT;

		if (time >= 1.5f)
		{
			GetOwner()->SetActive(false);
		}

		Transform* trans = GetTransform();

		float scale_t = time > 0.6f ? 0.6f : time;
		vec3 scale = vec3::Lerp(vec3(0.45f, 0.45f, 0.6f), vec3(0.45f, 0.45f, 8.0f), scale_t);
		trans->SetRelativeScale(scale);

		vec3 front = trans->GetWorldDir(DIR_TYPE::FRONT);
		trans->ShiftRelativePos(front * DT * 6.0f);


		if (blade_duration > 0.1f)
		{
			blade_duration -= 0.1f;
			GameObject* blade = ObjectPoolMgr::GetInst()->GetGameObject(L"Blade");
			blade->GetTransform()->SetRelativePos(trans->GetRelativePos() + front * scale.z * 0.35f);
			blade->GetScript<BladeScript>()->SetBladeIdx(blade_idx++);
		}
	}

	void BladeBurstScript::OnEnable()
	{
		time = 0.0f;
		blade_duration = 0.0f;
		blade_idx = 0;
		SoundMgr::GetInst()->Play(L"sound\\Cloud\\SE_PC0000_00_Cloud [7].wav", SOUND_OBJECT_TYPE::_3D);
	}

	void BladeBurstScript::OnDisable()
	{
	}
}