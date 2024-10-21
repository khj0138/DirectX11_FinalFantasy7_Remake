#include "pch.h"

#include <Engine\EventMgr.h>
#include <Engine\GameObject.h>
#include <Engine\Transform.h>
#include <Engine\ParticleSystem.h>

#include "StateMachine.h"
#include "PrimaryFireScript.h"
#include "HundredGunnerRightArmState.h"

#include "HundredGunnerRightArmScript.h"
namespace ff7r
{
	HundredGunnerRightArmScript::HundredGunnerRightArmScript()
		: Script((UINT)SCRIPT_TYPE::HUNDREDGUNNERRIGHTARMSCRIPT)
		, state(RIGHTARM_STATE::SLEEP)
		, muzzle_object(nullptr)
		, primary_fire(nullptr)
		, state_machine(nullptr)
		, states{}
		, ready_time(2.0f)
	{
	}

	HundredGunnerRightArmScript::~HundredGunnerRightArmScript()
	{
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}

		for (auto state : states)
		{
			if (state)
			{
				delete state;
				state = nullptr;
			}
		}
	}

	void HundredGunnerRightArmScript::Awake()
	{
		GetMeshRender()->SetFrustumCheck(false);

		states.resize((UINT)RIGHTARM_STATE::END);
		states[(UINT)RIGHTARM_STATE::SLEEP] = new HundredGunnerRightArmSleepState(this);
		states[(UINT)RIGHTARM_STATE::READY] = new HundredGunnerRightArmReadyState(this);
		states[(UINT)RIGHTARM_STATE::FIRE] = new HundredGunnerRightArmFireState(this);
		state = RIGHTARM_STATE::SLEEP;

		state_machine = new StateMachine(states[(UINT)RIGHTARM_STATE::SLEEP]);

		CreateFireObject();
	}

	void HundredGunnerRightArmScript::Start()
	{
	}

	void HundredGunnerRightArmScript::Tick()
	{
		switch (state)
		{
		case RIGHTARM_STATE::SLEEP: break;
		case RIGHTARM_STATE::READY: break;
		case RIGHTARM_STATE::FIRE: break;
		default: break;
		}

		state_machine->UpdateState();
	}

	void HundredGunnerRightArmScript::CreateFireObject()
	{
		const vector<GameObject*>& child = GetOwner()->GetChild();

		for (auto& obj : child)
		{
			if (obj->GetSocket())
			{
				GameObject* _object;
				if (obj->GetChild().empty())
				{
					_object = new GameObject;
					_object->AddComponent(new Transform);
					_object->AddComponent(new ParticleSystem);
					_object->SetLayerIndex(0);
					_object->SetName(L"Fire Muzzle");
					obj->AddChild(_object);
				}
				else
				{
					_object = obj->GetChild()[0];
				}

				_object->GetParticleSystem()->SetIntensity(3.0f);

				ParticleModule& module_data = _object->GetParticleSystem()->GetModuleData();
				module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::SPHERE;
				module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::TO_CENTER;
				module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
				module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
				module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
				module_data.shape_scale = vec3(5.f, 5.f, 5.0f);
				module_data.velocity_alignment = true;
				module_data.velocity_scale = true;
				module_data.velocity_scale_max = vec3(2.0f, 1.f, 1.f);
				module_data.spawn_scale_min = vec3(0.1f, 0.1f, 0.1f);
				module_data.spawn_scale_max = vec3(0.15f, 0.15f, 0.15f);
				module_data.start_scale = 1.0f;
				module_data.end_scale = 0.5f;
				module_data.speed = 4.0f;
				module_data.spawn_color = vec3(255.f / 255.f, 128.f / 255.f, 20.f / 255.f);
				module_data.space = 0;
				module_data.life_time_min = 0.5f;
				module_data.life_time_max = 1.0f;

				_object->SetActive(false);

				muzzle_object = _object;
				break;
			}
		}
	}

	void HundredGunnerRightArmScript::ChangeState(RIGHTARM_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}

		state = _next_state;

		switch (state)
		{
		case RIGHTARM_STATE::SLEEP:		state_machine->ChangeState(states[(UINT)RIGHTARM_STATE::SLEEP]);	break;
		case RIGHTARM_STATE::READY:		state_machine->ChangeState(states[(UINT)RIGHTARM_STATE::READY]);	break;
		case RIGHTARM_STATE::FIRE:		state_machine->ChangeState(states[(UINT)RIGHTARM_STATE::FIRE]);		break;
		default: break;
		}
	}
}