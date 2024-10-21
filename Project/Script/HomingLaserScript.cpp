#include "pch.h"

#include <Engine\Transform.h>
#include <Engine\KeyMgr.h>
#include <Engine\SoundMgr.h>

#include "GameMgr.h"
#include "StateMachine.h"
#include "HomingLaserState.h"
#include "CharacterDataScript.h"

#include "HomingLaserScript.h"
namespace ff7r
{
	HomingLaserScript::HomingLaserScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::HOMINGLASERSCRIPT)
		, luanch_time(0.3f)
		, luanch_speed(12.0f)
		, shoot_speed(15.0f)
		, life_time(0.0f)
		, dir(vec3(1.0f, 1.0f, 1.0f))
		, state_machine(nullptr)
		, state(HOMINGLASER_STATE::SLEEP)
	{
	}

	HomingLaserScript::~HomingLaserScript()
	{
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}

		for (UINT i = 0; i < (UINT)HOMINGLASER_STATE::END; i++)
		{
			delete states[i];
			states[i] = nullptr;
		}
	}

	void HomingLaserScript::Awake()
	{
		SetAttackData(ATK_TYPE::PUSH_NONE, 10);
		GetParticleSystem()->SetFrustumCheck(false);
		GetParticleSystem()->SetIntensity(7.0f);
		ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::TRAIL;
		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
		module_data.spawn_scale_min = vec3(0.25f, 0.25f, 0.25f);
		module_data.spawn_scale_max = vec3(0.25f, 0.25f, 0.25f);
		module_data.start_scale = 1.0f;
		module_data.end_scale = 0.0f;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CUSTOM_0;
		module_data.speed = 0.1f;
		module_data.spawn_color = vec3(110.f / 255.f, 10.f / 255.f, 200.f / 255.f);
		module_data.space = 0;

		Collider3D* collider = GetCollider3D();
		collider->SetTrigger(false);
		collider->SetDynamic(true);
		collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		collider->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		collider->SetOffsetScale(vec3(0.3f, 0.3f, 0.3f));
		collider->SetMove(false);
		collider->SetMass(0.00001f);

		states.resize((UINT)HOMINGLASER_STATE::END);
		states[(UINT)HOMINGLASER_STATE::SLEEP] = new HomingLaserSleepState(this);
		states[(UINT)HOMINGLASER_STATE::LAUNCH] = new HomingLaserLaunchState(this);
		states[(UINT)HOMINGLASER_STATE::SHOOT] = new HomingLaserShootState(this);

		state = HOMINGLASER_STATE::SLEEP;
		state_machine = new StateMachine(states[(UINT)HOMINGLASER_STATE::SLEEP]);

	}

	void HomingLaserScript::Start()
	{
		// ChangeState(HOMINGLASER_STATE::LAUNCH);
	}

	void HomingLaserScript::Tick()
	{
		state_machine->UpdateState();
		//DrawDebugSphere(GetTransform()->GetWorldPos(), 0.2f, vec4(0.0f, 1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f));

		if (state == HOMINGLASER_STATE::SHOOT)
		{
			life_time += DT;

			if (life_time >= 3.5f)
			{
				GetOwner()->SetActive(false);
				GetCollider3D()->Disable();
			}
		}
	}

	void HomingLaserScript::OnEnable()
	{
		ChangeState(HOMINGLASER_STATE::SLEEP);
		GetCollider3D()->SetCollision(true);
		life_time = 0.0f;
	}

	void HomingLaserScript::OnDisable()
	{
	}

	void HomingLaserScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			GetOwner()->SetActive(false);
			GetCollider3D()->Disable();
		}
		else
		{
			SoundMgr::GetInst()->Play(L"sound\\HundredGunner\\Battle_Resident [18].wav", SOUND_OBJECT_TYPE::_3D, GetOwner());
			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(GetAttackType(), GetAttackDamage(), _contact_pos[0]);
		}
	}

	void HomingLaserScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void HomingLaserScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void HomingLaserScript::SetDir(vec3 _dir)
	{
		dir = _dir;
		GetParticleSystem()->GetModuleData().velocity_dir = dir;
	}

	void HomingLaserScript::Move(vec3 _velocity)
	{
		GetTransform()->ShiftRelativePos(_velocity * DT);
	}

	void HomingLaserScript::MoveCurve(vec3 _origin, float _time)
	{
		vec3 target_pos = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos();

		// vec3 p1 = vec3::Lerp()
	}

	void HomingLaserScript::ChangeState(HOMINGLASER_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}

		state = _next_state;

		switch (state)
		{
		case HOMINGLASER_STATE::SLEEP:	state_machine->ChangeState(states[(UINT)HOMINGLASER_STATE::SLEEP]);		break;
		case HOMINGLASER_STATE::LAUNCH: state_machine->ChangeState(states[(UINT)HOMINGLASER_STATE::LAUNCH]);	break;
		case HOMINGLASER_STATE::SHOOT:	state_machine->ChangeState(states[(UINT)HOMINGLASER_STATE::SHOOT]);		break;
		default: break;
		}
	}
}