#include "pch.h"

#include "UIMgr.h"
#include "StateMachine.h"
#include "BarretWeaponScript.h"
#include "BarretWeaponState.h"

#include "EffectScript.h"

#include "BarretNormalAtkScript.h"

#include <Engine\\GameObject.h>
#include <Engine\\SoundMgr.h>

namespace ff7r
{
	BarretWeaponScript::BarretWeaponScript()
		: Script(SCRIPT_TYPE::BARRETWEAPONSCRIPT)
		, test(0)
	{
		for (int i = 0; i < 10; i++) sound_buf.push_back(-1);
	}
	BarretWeaponScript::~BarretWeaponScript()
	{
		SafeDelVector(states);
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}
	}

	void BarretWeaponScript::Awake()
	{

		GetCollider3D()->SetTrigger(true);
		GetCollider3D()->SetDynamic(false);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		GetCollider3D()->SetOffsetScale(vec3(0.3f, 0.3f, 0.7f));
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.0f, -0.35f));


		states.resize((UINT)BARRET_WEAPON_STATE::END);
		states[(UINT)BARRET_WEAPON_STATE::SLEEP] = new BarretSleepState(this);
		states[(UINT)BARRET_WEAPON_STATE::NORMAL_ATTACK] = new BarretNormalAtkState(this);
		states[(UINT)BARRET_WEAPON_STATE::UPPERCUT] = new BarretUpperCutState(this);
		states[(UINT)BARRET_WEAPON_STATE::SMACKDOWN] = new BarretSmackDownState(this);
		states[(UINT)BARRET_WEAPON_STATE::MAXIMUM_FURY] = new BarretMaximumFuryState(this);
		states[(UINT)BARRET_WEAPON_STATE::LIMIT_ATTACK] = new BarretLimitAtkState(this);

		BarretNormalAtkState* _normal_atk = dynamic_cast<BarretNormalAtkState*>(states[(UINT)BARRET_WEAPON_STATE::NORMAL_ATTACK]);
		if (_normal_atk != nullptr)
			_normal_atk->CreateObjects();
		BarretUpperCutState* _upper_cut = dynamic_cast<BarretUpperCutState*>(states[(UINT)BARRET_WEAPON_STATE::UPPERCUT]);
		if (_upper_cut != nullptr)
			_upper_cut->CreateObjects();
		BarretSmackDownState* _smack_down = dynamic_cast<BarretSmackDownState*>(states[(UINT)BARRET_WEAPON_STATE::SMACKDOWN]);
		if (_smack_down != nullptr)
			_smack_down->CreateObjects();
		BarretLimitAtkState* _limit_atk = dynamic_cast<BarretLimitAtkState*>(states[(UINT)BARRET_WEAPON_STATE::LIMIT_ATTACK]);
		if (_limit_atk != nullptr)
			_limit_atk->CreateObjects();
		BarretMaximumFuryState* _max_fury= dynamic_cast<BarretMaximumFuryState*>(states[(UINT)BARRET_WEAPON_STATE::MAXIMUM_FURY]);
		if (_max_fury != nullptr)
			_max_fury->CreateObjects();
		/*attack_objects.resize(20);
		for (int i = 0; i < attack_objects.size(); i++)
		{
			attack_objects[i] = new GameObject();

		}*/
		//GameObject* fire = new GameObject;
		//fire->AddComponent(new Transform);
		state_machine = new StateMachine(states[(UINT)BARRET_WEAPON_STATE::SLEEP]);

	}

	void BarretWeaponScript::Start()
	{

	}

	void BarretWeaponScript::Tick()
	{
	}

	void BarretWeaponScript::Attack()
	{
		if (state == BARRET_WEAPON_STATE::NORMAL_ATTACK)
			SoundOn(0);
		state_machine->UpdateState();
	}

	void BarretWeaponScript::SoundOn(int _idx)
	{
		switch(_idx)
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
		}
	}

	void BarretWeaponScript::SoundOff(int _idx)
	{

	}

	/*void BarretWeaponScript::BeginOverlap(Collider3D* _other, const vec3& _contact_pos)
	{
		GameObject* _other_obj = _other->GetOwner();

		vec3 _cur_pos = GetTransform()->GetWorldPos();

		UIMgr::GetInst()->PrintWorldTextUI("12", false, true, _contact_pos);
		GameObject* _hit_effect;
		GetCollider3D()->SetCollision(false);

	}

	void BarretWeaponScript::OnOverlap(Collider3D* _other, const vec3& _contact_pos)
	{
	}

	void BarretWeaponScript::EndOverlap(Collider3D* _other, const vec3& _contact_pos)
	{
		int a = 0;
	}*/

	void BarretWeaponScript::ChangeState(BARRET_WEAPON_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}

		state = _next_state;

		state_machine->ChangeState(states[(UINT)_next_state]);
	}
	vec3 BarretWeaponScript::CalcMuzzlePos()
	{
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
		return _weapon_pos +_weapon_col_offset;
	}
}