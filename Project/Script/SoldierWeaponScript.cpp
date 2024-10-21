#include "pch.h"

#include "UIMgr.h"
#include "StateMachine.h"
#include "SoldierWeaponScript.h"
#include "SoldierWeaponState.h"

#include "EffectScript.h"


#include <Engine\\GameObject.h>
#include <Engine\\SoundMgr.h>

namespace ff7r
{
	SoldierWeaponScript::SoldierWeaponScript()
		: Script(SCRIPT_TYPE::SOLDIERWEAPONSCRIPT)
		, test(0)
	{
		for (int i = 0; i < 10; i++) sound_buf.push_back(-1);
	}
	SoldierWeaponScript::~SoldierWeaponScript()
	{
		SafeDelVector(states);
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}
	}

	void SoldierWeaponScript::Awake()
	{

		// GetCollider3D()->SetTrigger(true);
		// GetCollider3D()->SetDynamic(false);
		// GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		//GetCollider3D()->SetOffsetScale(vec3(0.3f, 0.3f, 0.7f));
		//GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.0f, -0.35f));


		states.resize((UINT)SOLDIER_WEAPON_STATE::END);
		states[(UINT)SOLDIER_WEAPON_STATE::SLEEP] = new SoldierSleepState(this);
		states[(UINT)SOLDIER_WEAPON_STATE::NORMAL_ATTACK] = new SoldierNormalAtkState(this);

		SoldierNormalAtkState* _normal_atk = dynamic_cast<SoldierNormalAtkState*>(states[(UINT)SOLDIER_WEAPON_STATE::NORMAL_ATTACK]);
		if (_normal_atk != nullptr)
			_normal_atk->CreateObjects();
		/*attack_objects.resize(20);
		for (int i = 0; i < attack_objects.size(); i++)
		{
			attack_objects[i] = new GameObject();

		}*/
		//GameObject* fire = new GameObject;
		//fire->AddComponent(new Transform);
		state_machine = new StateMachine(states[(UINT)SOLDIER_WEAPON_STATE::SLEEP]);

	}

	void SoldierWeaponScript::Start()
	{

	}

	void SoldierWeaponScript::Tick()
	{
	}

	void SoldierWeaponScript::Attack()
	{
	//	if (state == SOLDIER_WEAPON_STATE::NORMAL_ATTACK)
		//	SoundOn(0);
		state_machine->UpdateState();
	}

	void SoldierWeaponScript::SoundOn(int _idx)
	{
		/*switch (_idx)
		{
		case 0:
		{
			(SoundMgr::GetInst()->Play(L"sound\\AtkShoot01_shot.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, true));

			break;
		}
		case 1:
		{

			break;
		}
		case 2:
		{

			break;
		}
		case 3:
		{

			break;
		}
		case 4:
		{

			break;
		}
		}*/
	}

	void SoldierWeaponScript::SoundOff(int _idx)
	{

	}

	/*void SoldierWeaponScript::BeginOverlap(Collider3D* _other, const vec3& _contact_pos)
	{
		GameObject* _other_obj = _other->GetOwner();

		vec3 _cur_pos = GetTransform()->GetWorldPos();

		UIMgr::GetInst()->PrintWorldTextUI("12", false, true, _contact_pos);
		GameObject* _hit_effect;
		GetCollider3D()->SetCollision(false);

	}

	void SoldierWeaponScript::OnOverlap(Collider3D* _other, const vec3& _contact_pos)
	{
	}

	void SoldierWeaponScript::EndOverlap(Collider3D* _other, const vec3& _contact_pos)
	{
		int a = 0;
	}*/

	void SoldierWeaponScript::ChangeState(SOLDIER_WEAPON_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}

		state = _next_state;

		state_machine->ChangeState(states[(UINT)_next_state]);
	}
	vec3 SoldierWeaponScript::CalcMuzzlePos()
	{
		return GetTransform()->GetWorldPos();
		vec3 _weapon_pos = GetTransform()->GetWorldPos();
		vec3 _weapon_dir;
		GetTransform()->CalcWorldDir();
		_weapon_dir = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);

		//vec3 _target_pos = _target->GetTransform()->GetWorldPos();
		//vec3 _target_scale = _target->GetCollider3D()->GetOffsetScale();
		//weapon->GetCollider3D()->SetCollision(true);

		vec3 _weapon_col_offset = GetCollider3D()->GetOffsetScale();
		_weapon_col_offset.x = 0.0f;
		_weapon_col_offset.y = 0.0f;
		//_weapon_col_offset.z *= 0.0f;
		_weapon_col_offset = _weapon_dir.Normalize() * _weapon_col_offset.z * -1.f;
		return _weapon_pos + _weapon_col_offset;
	}
}