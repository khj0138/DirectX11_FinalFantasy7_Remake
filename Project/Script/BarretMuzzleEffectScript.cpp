#include "pch.h"

#include "BarretMuzzleEffectScript.h"
namespace ff7r
{
	BarretMuzzleEffectScript::BarretMuzzleEffectScript()
		: Script((UINT)SCRIPT_TYPE::BARRETMUZZLEEFFECTSCRIPT)
	{
	}

	BarretMuzzleEffectScript::~BarretMuzzleEffectScript()
	{
	}

	void BarretMuzzleEffectScript::Awake()
	{
		Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Smk_04O_T.png");
		Animator2D* _anim = GetOwner()->GetAnimator2D();

		_anim->CreateAnimation(L"muzzle_effect", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 60.f);
		_anim->SetManagementActive(true);

	}

	void BarretMuzzleEffectScript::Tick()
	{
	}

	void BarretMuzzleEffectScript::OnEnable()
	{
		GetOwner()->GetAnimator2D()->Play(L"muzzle_effect", false);
		GetOwner()->GetAnimator2D()->Reset();
	}
}