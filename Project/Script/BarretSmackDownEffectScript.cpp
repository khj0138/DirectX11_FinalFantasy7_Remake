#include "pch.h"

#include "BarretSmackDownEffectScript.h"
namespace ff7r
{
	BarretSmackDownEffectScript::BarretSmackDownEffectScript()
		: Script((UINT)SCRIPT_TYPE::BARRETSMACKDOWNEFFECTSCRIPT)
	{
	}

	BarretSmackDownEffectScript::~BarretSmackDownEffectScript()
	{
	}

	void BarretSmackDownEffectScript::Awake()
	{
		Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_05O_T.png");
		Animator2D* _anim = GetOwner()->GetAnimator2D();

		_anim->CreateAnimation(L"normal_attack_hit", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 60.f);
		_anim->SetManagementActive(true);

	}

	void BarretSmackDownEffectScript::Tick()
	{
	}

	void BarretSmackDownEffectScript::OnEnable()
	{
		GetOwner()->GetAnimator2D()->Play(L"normal_attack_hit", false);
		GetOwner()->GetAnimator2D()->Reset();
	}
}