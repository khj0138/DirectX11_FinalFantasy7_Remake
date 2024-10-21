#include "pch.h"

#include <Engine\SoundMgr.h>
#include "ObjectPoolMgr.h"

#include "MagicFireScript.h"
namespace ff7r
{
	MagicFireScript::MagicFireScript()
		: Script((UINT)SCRIPT_TYPE::MAGICFIRESCRIPT)
		, speed(6.5f)
		, trail_effect_time(0.f)
		, life_time(0.f)
	{
	}

	MagicFireScript::~MagicFireScript()
	{
	}

	void MagicFireScript::Awake()
	{
		GetCollider3D()->SetTrigger(true);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(0.3f, 0.3f, 0.3f));
	}

	void MagicFireScript::Tick()
	{
		vec3 _pos = GetTransform()->GetWorldPos();
		_pos += velocity * DT;

		GetTransform()->SetRelativePos(_pos);

		trail_effect_time += DT;
		life_time -= DT;

		if (trail_effect_time >= 0.2f)
		{
			trail_effect_time -= 0.2f;

			GameObject* trail_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Magic Fire Trail Effect");
			trail_effect->GetTransform()->SetRelativePos(_pos);
		}
		if (life_time <= 0.f)
		{
			GetOwner()->SetActive(false);
		}
	}

	void MagicFireScript::SetTargetPos(vec3 _pos)
	{
		vec3 _dir = _pos - GetTransform()->GetRelativePos();

		velocity = _dir.Normalize() * speed;
	}

	void MagicFireScript::OnEnable()
	{
		trail_effect_time = 0.0f;
		life_time = 5.0f;
		GetCollider3D()->SetCollision(true);
	}

	void MagicFireScript::OnDisable()
	{
	}

	void MagicFireScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		SoundMgr::GetInst()->Play(L"sound\\Cloud\\Magic_Fire01 [4].wav", SOUND_OBJECT_TYPE::_3D);
		GetCollider3D()->Disable();
		GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Magic Fire Explosion Effect");
		explosion_effect->GetTransform()->SetRelativePos(_other->GetTransform()->GetWorldPos() + _other->GetOffsetPos());
		GetOwner()->SetActive(false);
	}
}