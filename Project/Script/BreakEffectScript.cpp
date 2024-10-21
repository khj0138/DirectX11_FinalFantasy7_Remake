#include "pch.h"

#include "EffectScript.h"
#include "ElectricEffectScript.h"
#include "ObjectPoolMgr.h"

#include "BreakEffectScript.h"

namespace ff7r
{
	BreakEffectScript::BreakEffectScript()
		: Script(SCRIPT_TYPE::BREAKEFFECTSCRIPT)
		, time(0.0f)
		, b(false)
	{
	}

	BreakEffectScript::~BreakEffectScript()
	{
	}

	void BreakEffectScript::Awake()
	{
	}

	void BreakEffectScript::Tick()
	{
		time += DT;

		if (!b)
		{
			if (time > 0.1f)
			{
				for (GameObject* effect : effects)
				{
					effect->GetScript<ElectricEffectScript>()->Next();
				}
			}
			b = true;
			time = 0.0f;
		}
		else
		{
			if (time >= 0.5f)
			{
				for (GameObject* effect : effects)
				{
					effect->SetActive(false);
				}
				GetOwner()->SetActive(false);
			}
			else
			{
				for (GameObject* effect : effects)
				{
					float t = lerp(6.0f, 0.0f, time / 0.5f);
					effect->GetScript<EffectScript>()->SetIntensity(t);
				}
			}

		}
	}

	void BreakEffectScript::OnEnable()
	{
		for (size_t i = 0; i < 8; i++)
		{
			GameObject* effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Electric Effect");

			effect->GetTransform()->SetRelativeRotY(Degree2Rad(45.f * i));
			effect->GetTransform()->FinalTick();
			vec3 right = effect->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

			effect->GetTransform()->SetRelativePos(GetTransform()->GetRelativePos() + right * 2.5f);
			effects.push_back(effect);
		}
		time = 0.0f;
		b = false;
	}

	void BreakEffectScript::OnDisable()
	{
		effects.clear();
	}
}
