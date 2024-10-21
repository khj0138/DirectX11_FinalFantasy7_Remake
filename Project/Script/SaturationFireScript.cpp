#include "pch.h"

#include "ObjectPoolMgr.h"
#include "HundredBulletScript.h"

#include "SaturationFireScript.h"
namespace ff7r
{
	SaturationFireScript::SaturationFireScript()
		: Script(SCRIPT_TYPE::SATURATIONFIRESCRIPT)
		, state(FIRE_STATE::SLEEP)
		, muzzle_effect(nullptr)
		, effect_time(0.0f)
		, rest_time(0.0f)
	{
	}

	SaturationFireScript::~SaturationFireScript()
	{
	}

	void SaturationFireScript::Awake()
	{
		if (GetOwner()->GetChild().empty())
		{
			muzzle_effect = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Muzzle_01_M.gltf")->Instantiate();
			muzzle_effect->SetName(L"Muzzle");
			muzzle_effect->GetTransform()->SetRelativeScale(1, 1, 1);
			muzzle_effect->SetLayerIndex(0);
		}
		else
		{
			muzzle_effect = GetOwner()->GetChild()[0];
		}
		
		muzzle_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"ForwardDefault3DMtrl"));
		muzzle_effect->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(
			L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Muzzle_01_T.png"));

		muzzle_effect->SetActive(false);

		GetOwner()->AddChild(muzzle_effect);
	}

	void SaturationFireScript::Start()
	{
	}

	void SaturationFireScript::Tick()
	{
		switch (state)
		{
		case FIRE_STATE::SLEEP: break;

		case FIRE_STATE::FIRE: 
		{
			effect_time += DT;

			if (effect_time >= 0.1f)
			{
				state = FIRE_STATE::REST;
				muzzle_effect->SetActive(false);
				effect_time = 0.0f;
			}
		}
		break;

		case FIRE_STATE::REST: 
		{
			rest_time += DT;

			if (rest_time >= 0.05f)
			{
				state = FIRE_STATE::SLEEP;
				rest_time = 0.0f;
			}
		}
		break;

		default: break;
		}
	}

	bool SaturationFireScript::Shoot(vec3 _target_pos)
	{
		if (state == FIRE_STATE::FIRE || state == FIRE_STATE::REST)
		{
			return false;
		}

		state = FIRE_STATE::FIRE;
		muzzle_effect->SetActive(true);

		GameObject* _bullet = ObjectPoolMgr::GetInst()->GetGameObject(L"Hundred Bullet");
		_bullet->GetTransform()->SetRelativePos(GetTransform()->GetWorldPos());
		_bullet->GetScript<HundredBulletScript>()->SetTargetPos(_target_pos);

		return true;
	}
}