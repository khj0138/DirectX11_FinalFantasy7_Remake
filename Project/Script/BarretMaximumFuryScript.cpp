#include "pch.h"
#include "BarretMaximumFuryScript.h"
#include "BWeaponHitEffectScript.h"
#include "CharacterDataScript.h"
#include "EffectScript.h"

#include "ObjectPoolMgr.h"
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
namespace ff7r
{
	BarretMaximumFuryScript::BarretMaximumFuryScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::BARRETMAXIMUMFURYSCRIPT)
		, boom(false)
	{
	}
	BarretMaximumFuryScript::~BarretMaximumFuryScript()
	{
	}
	void BarretMaximumFuryScript::Awake()
	{

		GetCollider3D()->SetTrigger(true);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(3.0f, 3.0f, 3.0f));
		GetCollider3D()->SetCollision(true);
	}
	void BarretMaximumFuryScript::Tick()
	{
	}
	void BarretMaximumFuryScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{

		if (_other->GetOwner()->GetLayerIndex() != 3)
			return;
		vec3 _atk_pos = GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos();
		vec3 _other_pos = _other->GetTransform()->GetWorldPos() + _other->GetOffsetPos();

		int _dmg = randi(8, 12);

		if (boom)
		{
			//SetAttackData(ATK_TYPE::UP, _dmg * 8);
			GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Normal Attack Effect");
			explosion_effect->GetTransform()->SetRelativePos(_atk_pos);
			boom = false;

			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::UP, _dmg * 7, _atk_pos);

			SoundMgr::GetInst()->Play(L"sound\\AtkShoot01_finish_hit.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, true);

		}
		GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Maximum Fury Effect");
		explosion_effect->GetTransform()->SetRelativePos(_atk_pos);
		GameObject* _hit_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Normal Attack Hit Effect");
		_hit_effect->GetTransform()->SetRelativePos(_atk_pos);
		_hit_effect->GetScript<BWeaponHitEffectScript>()->SetDir((_atk_pos - _other_pos));
		_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::DEFAULT, _dmg, _atk_pos);

		GetOwner()->SetActive(false);
		GetCollider3D()->SetGravity(false);
	}
	void BarretMaximumFuryScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretMaximumFuryScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretMaximumFuryScript::OnEnable()
	{
		GetCollider3D()->SetCollision(true);
	}
	void BarretMaximumFuryScript::OnDisable()
	{
		GetCollider3D()->Disable();
	}
}