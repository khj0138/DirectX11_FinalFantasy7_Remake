#include "pch.h"
#include "MagicFireTrailScript.h"

namespace ff7r
{
	MagicFireTrailScript::MagicFireTrailScript()
		: Script(SCRIPT_TYPE::MAGICFIRETRAILSCRIPT)
	{
	}

	MagicFireTrailScript::~MagicFireTrailScript()
	{
	}

	void MagicFireTrailScript::Awake()
	{
		Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Fire_04_T.png");
		Animator2D* _anim = GetOwner()->GetAnimator2D();

		_anim->CreateAnimation(L"magic_fire_effet", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 60.f);
		_anim->SetManagementActive(true);
	}

	void MagicFireTrailScript::OnEnable()
	{
		GetOwner()->GetAnimator2D()->Play(L"magic_fire_effet", false);
		GetOwner()->GetAnimator2D()->Reset();
	}
}