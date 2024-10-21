#include "pch.h"

#include <Engine\GameObject.h>
#include "SoldierScript.h"
#include "UIMgr.h"
namespace ff7r
{
	SoldierScript::SoldierScript()
		: CharacterDataScript(SCRIPT_TYPE::SOLDIERSCRIPT)
		, time(0.f)
		, is_dead(false)
		, dead_event(nullptr)
	{
	}

	SoldierScript::~SoldierScript()
	{
	}

	void SoldierScript::Awake()
	{
		animator = GetAnimator3D();

		SetMaxHP(2000.f);
		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CAPSULE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.9f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(1.8f, 1.5f, 1.8f));
		GetCollider3D()->SetMass(1.3f);
		//GetCollider3D()->SetMove(false);
	}

	void SoldierScript::Tick()
	{
		animator->SetBool(L"Is_Ground", is_ground);
		if (is_dead)
		{
			time += DT;

			if (time > 3.f)
			{
				if (dead_event)
				{
					dead_event();
					dead_event = nullptr;
				}
				DestroyObject(GetOwner());
			}
		}
	}

	void SoldierScript::Start()
	{
		GetCollider3D()->SetGravity(true);
	}

	void SoldierScript::Hit(ATK_TYPE _hit, int _damage, vec3 _contact_pos)
	{
		if (GetCurHP() <= 0)
		{
			return;
		}

		if (!is_ground)
		{
			return;
		}

		string _str_damage = std::to_string(_damage);
		UIMgr::GetInst()->PrintWorldTextUI(_str_damage, false, true, _contact_pos);
		vec3 _front = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 _hit_dir = GetCollider3D()->GetWorldPos() - _contact_pos;

		float _angle_y = CalcYAxisFromVector(_front, _hit_dir);
		_angle_y = fmodf(_angle_y, XM_2PI);
		_angle_y = _angle_y < 0.f ? _angle_y + XM_2PI : _angle_y;

		vec3 t = RotateVec3YAxis(vec3(100.0f, 300.0f, 0.0f), _angle_y);

		SetCurHP((GetCurHP() - _damage) <= 0 ? 0 : GetCurHP() - _damage);

		if (GetCurHP() <= 0)
		{
			animator->SetTrigger(L"Damaged");
			animator->SetInteger(L"Dmg_Type", 2);
			GetCollider3D()->Disable();
			is_dead = true;
			return;
		}

		animator->SetTrigger(L"Damaged");
		if (_hit == ATK_TYPE::PUSH_NONE)
		{
			animator->SetInteger(L"Dmg_Type", 0);
		}
		else if (_hit == ATK_TYPE::PUSH_WEAK)
		{
			animator->SetInteger(L"Dmg_Type", 1);
		}
		else if (_hit == ATK_TYPE::UP)
		{
			GetCollider3D()->AddForce(t);
			animator->SetInteger(L"Dmg_Type", 3);
		}
		else if (_hit == ATK_TYPE::DOWN)
		{
			animator->SetInteger(L"Dmg_Type", 2);
		}
		else
		{
			animator->SetInteger(L"Dmg_Type", 0);
		}


	}

	void SoldierScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			GetCollider3D()->SetGravity(false);
			is_ground = true;
		}
		else if(_other->GetOwner()->GetLayerIndex() == 7)	
		{
			GetTransform()->SetRelativeRotY(_other->GetOwner()->GetParent()->GetParent()->GetTransform()->GetRelativeRot().y + XM_PI);
		}
	}

	void SoldierScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void SoldierScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			GetCollider3D()->SetGravity(true);
			is_ground = false;
		}
	}
	void SoldierScript::SetWeapon(SoldierWeaponScript* _weapon)
	{
		weapon = _weapon;

		const vector<GameObject*>& child = GetOwner()->GetChild();
		for (auto& obj : child)
		{
			if (obj->GetSocket())
			{
				wstring socket_name = obj->GetName();

				if (socket_name.find(L"Weapon Socket") != wstring::npos)
				{
					obj->AddChild(_weapon->GetOwner());
				}
			}
		}
	}
}