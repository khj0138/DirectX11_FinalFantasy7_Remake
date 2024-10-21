#include "pch.h"

#include <Engine\Transform.h>
#include <Engine\KeyMgr.h>
#include <Engine\RenderMgr.h>
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
#include "ObjectPoolMgr.h"
#include "GameMgr.h"

#include "BackWeaponScript.h"
#include "SaturationFireScript.h"
#include "StateMachine.h"
#include "HundredGunnerState.h"
#include "BeamScript.h"
#include "HGFootScript.h"

#include "HundredGunnerScript.h"
namespace ff7r
{
	HundredGunnerScript::HundredGunnerScript()
		: CharacterDataScript((UINT)SCRIPT_TYPE::HUNDREDGUNNERSCRIPT)
		, animator(nullptr)
		, state_machine(nullptr)
		, state(HUNDREDGUNNER_STATE::IDLE)
		, shoot_lazer_duration(0.2f)
		, back_weapon(nullptr)
		, right_arm(nullptr)
		, target(nullptr)
		, shoot_minigun(false)
		, shoot_laser(false)
		, overlap_wall_cnt(false)
		, shoot_beam(false)
		, first_start(false)
		, turn_end(false)
		, show_idx(0)
		, is_dead(false)
		, time(0.f)
	{
		states.resize((UINT)HUNDREDGUNNER_STATE::END);
	}

	HundredGunnerScript::~HundredGunnerScript()
	{
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}

		for (UINT i = 0; i < (UINT)HUNDREDGUNNER_STATE::END; i++)
		{
			if (states[i])
			{
				delete states[i];
				states[i] = nullptr;
			}
		}
	}

	void HundredGunnerScript::Awake()
	{
		SetMaxHP(1500.f);

		GetMeshRender()->SetFrustumCheck(false);

		// GetOwner()->AddComponent(new Collider3D);
		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CAPSULE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 3.8f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(6.0f, 7.0f, 6.0f));
		GetCollider3D()->SetMove(false);
		GetCollider3D()->SetMass(1000.f);
		//
		//GetOwner()->SetLayerIndex(

		animator = GetAnimator3D();

		animator->RegisterAnimationEvent(L"ChangeIdleState", [this]() { ChangeIdleState(); });
		animator->RegisterAnimationEvent(L"ShootFire", [this]() { ShootFire(); });
		animator->RegisterAnimationEvent(L"ReadyFire", [this]() { ReadyFire(); });
		animator->RegisterAnimationEvent(L"ShootMinigun", [this]() { ShootMinigun(); });
		animator->RegisterAnimationEvent(L"ShootMinigunEnd", [this]() { ShootMinigunEnd(); });
		animator->RegisterAnimationEvent(L"ShootLaser", [this]() { ShootLaser(); });
		animator->RegisterAnimationEvent(L"ShootBeam", [this]() { ShootBeam(); });

		states[(UINT)HUNDREDGUNNER_STATE::IDLE] = new HundredGunnerSleepState(this);
		states[(UINT)HUNDREDGUNNER_STATE::TURN] = new HundredGunnerTurnState(this);
		states[(UINT)HUNDREDGUNNER_STATE::ATK_LASER] = new HundredGunnerAttackLazerState(this);
		states[(UINT)HUNDREDGUNNER_STATE::ATK_FIRE] = new HundredGunnerAttackFireState(this);
		states[(UINT)HUNDREDGUNNER_STATE::ATK_SHOOT] = new HundredGunnerAttackShootState(this);
		states[(UINT)HUNDREDGUNNER_STATE::ATK_BODY] = new HundredGunnerAttackBodyState(this);
		states[(UINT)HUNDREDGUNNER_STATE::ATK_BEAM] = new HundredGunnerAttackBeamState(this);

		state = HUNDREDGUNNER_STATE::IDLE;
		state_machine = new StateMachine(states[(UINT)HUNDREDGUNNER_STATE::IDLE]);

		back_weapon = GetOwner()->GetScriptInChildren<BackWeaponScript>();
		right_arm = GetOwner()->GetScriptInChildren<HundredGunnerRightArmScript>();

		if (!back_weapon || !right_arm)
		{
			assert(nullptr);
		}

		const vector<GameObject*>& child = GetOwner()->GetChild();

		for (auto& obj : child)
		{
			if (obj->GetSocket())
			{
				wstring socket_name = obj->GetName();

				if (socket_name.find(L"Shoot") != wstring::npos)
				{
					minigun.push_back(obj->GetScriptInChildren<SaturationFireScript>());
				}
				else if (socket_name.find(L"Foot") != wstring::npos)
				{
					for (const auto& foot : obj->GetChild())
					{
						if (foot->GetName().find(L"Break") == wstring::npos)
						{
							foot->AddComponent(new HGFootScript);
						}
					}
				}
			}
		}
	}

	void HundredGunnerScript::Start()
	{
		target = GameMgr::GetInst()->GetCurPlayer();
		GameMgr::GetInst()->SetDestroyPillar();
	}

	void HundredGunnerScript::Tick()
	{
		if(!is_dead)
		{
			if (idx == 0)
			{
				next_state_time += DT;
				if (next_state_time >= 3.0f)
				{
					next_state_time = 0.f;
					if (state == HUNDREDGUNNER_STATE::IDLE)
					{
						ChangeState(HUNDREDGUNNER_STATE::ATK_SHOOT);
						idx++;
					}
				}
			}
			else if (idx == 1)
			{
				if (state == HUNDREDGUNNER_STATE::IDLE)
				{
					ChangeState(HUNDREDGUNNER_STATE::ATK_BEAM);
					idx++;
				}
			}
			else if (turn_end && state == HUNDREDGUNNER_STATE::IDLE)
			{
				next_state_time += DT;
				if (next_state_time >= 0.5f)
				{
					turn_end = false;

					switch (next)
					{
					case 0:
						ChangeState(HUNDREDGUNNER_STATE::ATK_FIRE);
						break;
					case 1:
						ChangeState(HUNDREDGUNNER_STATE::ATK_LASER);
						break;
					case 2:
						ChangeState(HUNDREDGUNNER_STATE::ATK_SHOOT);
						break;
					default: break;
					}

					next = 0;
					next_state_time = 0.0f;;
				}
			}
			else if (state == HUNDREDGUNNER_STATE::IDLE)
			{
				next_state_time += DT;

				if (next_state_time >= 3.0f)
				{
					next = show_idx > 2 ? randi(2) : show_idx++;
					ChangeState(HUNDREDGUNNER_STATE::TURN);
					next_state_time = 0.0f;

				}
			}
		}


		state_machine->UpdateState();

		if (shoot_beam)
		{
			DestroyPillar();
		}

	}

	void HundredGunnerScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 11)
		{
			overlap_wall_cnt++;
		}
	}

	void HundredGunnerScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{

	}

	void HundredGunnerScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 11)
		{
			overlap_wall_cnt--;
		}
	}

	void HundredGunnerScript::Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos)
	{
		CharacterDataScript::Hit(_type, _damage, _contact_pos);

		if (GetCurHP() <= 0.f)
		{
			GetCollider3D()->Disable();
			animator->SetBool(L"Die", true);
			is_dead = true;
		}
	}

	void HundredGunnerScript::ChangeState(HUNDREDGUNNER_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}

		state = _next_state;

		state_machine->ChangeState(states[(UINT)_next_state]);
	}

	void HundredGunnerScript::ShootBeam()
	{
		vec3 pos = GetCollider3D()->GetWorldPos();
		vec3 rot = GetTransform()->GetRelativeRot();
		GameObject* beam = ObjectPoolMgr::GetInst()->GetGameObject(L"Beam");
		beam->GetScript<BeamScript>()->SetRot(vec3(0.0f, Degree2Rad(45.f), Degree2Rad(45.f)));
		beam->GetTransform()->SetRelativePos(pos);
		beam->GetTransform()->SetRelativeRot(rot);
		beam = ObjectPoolMgr::GetInst()->GetGameObject(L"Beam");
		beam->GetScript<BeamScript>()->SetRot(vec3(0.0f, Degree2Rad(-37.f), Degree2Rad(-55.f)));
		beam->GetTransform()->SetRelativePos(pos);
		beam->GetTransform()->SetRelativeRot(rot);
		beam = ObjectPoolMgr::GetInst()->GetGameObject(L"Beam");
		beam->GetScript<BeamScript>()->SetRot(vec3(0.0f, Degree2Rad(-42.f), Degree2Rad(60.f)));
		beam->GetTransform()->SetRelativePos(pos);
		beam->GetTransform()->SetRelativeRot(rot);
		beam = ObjectPoolMgr::GetInst()->GetGameObject(L"Beam");
		beam->GetScript<BeamScript>()->SetRot(vec3(0.0f, Degree2Rad(55.f), Degree2Rad(-45.f)));
		beam->GetTransform()->SetRelativePos(pos);
		beam->GetTransform()->SetRelativeRot(rot);
		beam = ObjectPoolMgr::GetInst()->GetGameObject(L"Beam");
		beam->GetScript<BeamScript>()->SetRot(vec3(0.0f, Degree2Rad(35.f), Degree2Rad(-25.f)));
		beam->GetTransform()->SetRelativePos(pos);
		beam->GetTransform()->SetRelativeRot(rot);
		beam = ObjectPoolMgr::GetInst()->GetGameObject(L"Beam");
		beam->GetScript<BeamScript>()->SetRot(vec3(0.0f, Degree2Rad(15.f), Degree2Rad(-35.f)));
		beam->GetTransform()->SetRelativeRot(rot);
		beam->GetTransform()->SetRelativePos(pos);
		shoot_beam = true;

		SoundMgr::GetInst()->Play(L"sound\\HundredGunner\\beam.wav", SOUND_OBJECT_TYPE::_2D);
	}

	void HundredGunnerScript::DestroyPillar()
	{
		time += DT;

		if (time >= 0.6f)
		{		
			RenderMgr::GetInst()->GetEditorCam()->SetPostProcess(Camera::CameraPostProcess::FLASH, true);
			RenderMgr::GetInst()->GetEditorCam()->SetFlashIntensity((time - 0.5f) * 1.1f);
		}
		if (time >= 0.8)
		{
			GameMgr::GetInst()->DestroyPillar();
		}

		if (time >= 1.2f)
		{
			RenderMgr::GetInst()->GetEditorCam()->SetPostProcess(Camera::CameraPostProcess::FLASH, false);
			shoot_beam = false;
		}
		
	}
}