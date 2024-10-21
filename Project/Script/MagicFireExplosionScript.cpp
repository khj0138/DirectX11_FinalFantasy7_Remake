#include "pch.h"

#include "MagicFireExplosionScript.h"
namespace ff7r
{
	MagicFireExplosionScript::MagicFireExplosionScript()
		: Script((UINT)SCRIPT_TYPE::MAGICFIREEXPLOSIONSCRIPT)
	{
	}

	MagicFireExplosionScript::~MagicFireExplosionScript()
	{
	}

	void MagicFireExplosionScript::Awake()
	{
		Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_02A_T.png");
		Animator2D* _anim = GetOwner()->GetAnimator2D();

		_anim->CreateAnimation(L"magic_fire_explosion", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 60.f);
		_anim->SetManagementActive(true);
	}

	void MagicFireExplosionScript::Tick()
	{
	}

	void MagicFireExplosionScript::OnEnable()
	{
		GetOwner()->GetAnimator2D()->Play(L"magic_fire_explosion", false);
		GetOwner()->GetAnimator2D()->Reset();
	}
}