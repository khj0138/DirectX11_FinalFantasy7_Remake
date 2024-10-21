#include "pch.h"

#include "BarretMaxFuryEffectScript.h"
namespace ff7r
{
	BarretMaxFuryEffectScript::BarretMaxFuryEffectScript()
		: Script((UINT)SCRIPT_TYPE::BARRETMAXFURYEFFECTSCRIPT)
	{
	}

	BarretMaxFuryEffectScript::~BarretMaxFuryEffectScript()
	{
	}

	void BarretMaxFuryEffectScript::Awake()
	{
		Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Fire_01_T.png");
		Animator2D* _anim = GetOwner()->GetAnimator2D();

		_anim->CreateAnimation(L"normal_attack_hit", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 60.f);
		_anim->SetManagementActive(true);

	}

	void BarretMaxFuryEffectScript::Tick()
	{
	}

	void BarretMaxFuryEffectScript::OnEnable()
	{
		GetOwner()->GetAnimator2D()->Play(L"normal_attack_hit", false);
		GetOwner()->GetAnimator2D()->Reset();
	}
}