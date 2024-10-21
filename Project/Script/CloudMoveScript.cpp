#include "pch.h"

#include <Engine\GameObject.h>
#include <Engine\Transform.h>
#include <Engine\Animator3D.h>
#include <Engine\SoundMgr.h>
#include <Engine\random_number.h>

#include "StateMachine.h"
#include "CloudState.h"
#include "CloudMoveScript.h"
#include "EffectScript.h"
#include "PlayerCameraScript.h"
#include "FindEnemyScript.h"
#include "CloudMoveScript.h"
#include "CharacterDataScript.h"
#include "ObjectPoolMgr.h"
#include "MagicFireScript.h"
#include "func.h"

#include "GameMgr.h"
#include "UIMgr.h"


namespace ff7r
{
#define ROT_Y_TIME 0.13f

	CloudMoveScript::CloudMoveScript()
		: PlayerDataScript((UINT)SCRIPT_TYPE::CLOUDMOVESCRIPT)
		, animator(nullptr)
		, state(CLOUD_STATE::END)
		, state_machine(nullptr)
		, n_attack_idx(0)
		, player_camera(nullptr)
		, rot_y_start(0.0f)
		, rot_y_end(0.0f)
		, is_rot_y(false)
		, rot_y_time(0.0f)
		, rot_y_idx(0)
		, hit_anim_idx(0)
		, hit_dir_idx(0)
		, limit0_cam_idx(-1)
		, cross_slash_cnt(0)
		, time_speed(0.1f)
		, cross_slash_effect(nullptr)
		, braver_slash_effect(nullptr)
		, weapon(nullptr)
		//, command(nullptr)
	{
	}

	CloudMoveScript::~CloudMoveScript()
	{
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}

		for (UINT i = 0; i < (UINT)CLOUD_STATE::END; i++)
		{
			if (states[i])
			{
				delete states[i];
			}
			states[i] = nullptr;
		}

		ClearCommand();
	}

	void CloudMoveScript::Awake()
	{
		animator = GetAnimator3D();

		states.resize((UINT)CLOUD_STATE::END);
		states[(UINT)CLOUD_STATE::IDLE]			= new CloudIdleState(this);
		states[(UINT)CLOUD_STATE::MOVE_ENTER]	= new CloudMoveEnterState(this);
		states[(UINT)CLOUD_STATE::MOVE]			= new CloudMoveState(this);
		states[(UINT)CLOUD_STATE::ATTACK]		= new CloudAttackState(this);
		states[(UINT)CLOUD_STATE::HIT]			= new CloudHitState(this);
		states[(UINT)CLOUD_STATE::AVOID]		= new CloudAvoidState(this);
		states[(UINT)CLOUD_STATE::ABILITY]		= new CloudAbilityState(this);
		states[(UINT)CLOUD_STATE::MAGIC]		= new CloudMagicState(this);
		states[(UINT)CLOUD_STATE::LIMIT]		= new CloudLimitState(this);
		states[(UINT)CLOUD_STATE::AI_ATTACK]	= new CloudAIAttackState(this);
		states[(UINT)CLOUD_STATE::AI_MOVE]		= new CloudAIMoveState(this);
		states[(UINT)CLOUD_STATE::ITEM]			= new CloudItemState(this);
		states[(UINT)CLOUD_STATE::AI_MOVE_ENTER]= new CloudAIMoveEnterState(this);


		state = CLOUD_STATE::IDLE;
		state_machine = new StateMachine(states[(UINT)CLOUD_STATE::IDLE]);


		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 1.0f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(0.7f, 1.7f, 0.7f));
		GetCollider3D()->SetMass(1.2f);

		RegisterAnimationEvents();
		RegisterSkillData();
		CreateChild();
	}

	void CloudMoveScript::Start()
	{
		GetCollider3D()->SetGravity(true);
	}

	void CloudMoveScript::Tick()
	{
		return;
		CommandData* _command = GetCommand();

		bool _is_control = GameMgr::GetInst()->IsControl(GetOwner());

		if (_is_control)
		{
			ExecuteCommand();

			switch (state)
			{
			case CLOUD_STATE::IDLE:
			{
				if (INPUT_MOVE_KEY)
				{
					ChangeState(CLOUD_STATE::MOVE_ENTER);
				}
				else if (KEY_TAP(KEY::LBTN))
				{
					ChangeState(CLOUD_STATE::ATTACK);
				}
				else if (KEY_TAP(KEY::RBTN))
				{
					ChangeState(CloudMoveScript::CLOUD_STATE::AVOID);
				}

			}
			break;
			case CLOUD_STATE::MOVE: break;
			case CLOUD_STATE::ATTACK: break;

			default: break;
			}
		}
		else
		{
			CheckAIStateChange();
		}
		state_machine->UpdateState();

		if (is_rot_y)
		{
			RotateY();
		}

		if (target)
		{
			if (!GetOwner()->GetScript<FindEnemyScript>()->CheckEnemyDistance(target) || !target->GetActive() 
				|| !target->GetCollider3D()->GetCollision())
			{
				target = nullptr;
			}
		}
		else
		{
			target = GetOwner()->GetScript<FindEnemyScript>()->FindClosestEnemy();
		}


	}

	void CloudMoveScript::Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos)
	{
		if (_type != ATK_TYPE::DEFAULT)
		{
			vec3 _front = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _hit_dir = GetCollider3D()->GetWorldPos() - _contact_pos;

			float _angle_y = CalcYAxisFromVector(_front, _hit_dir);
			_angle_y = fmodf(_angle_y, XM_2PI);
			_angle_y = _angle_y < 0.f ? _angle_y + XM_2PI : _angle_y;

			switch (_type)
			{
			case ATK_TYPE::PUSH_NONE:
			{
				hit_anim_idx = 0;
				if (_angle_y < Degree2Rad(45.f) || _angle_y >= Degree2Rad(315.f))
				{
					hit_dir_idx = 0;
				}
				else if (_angle_y >= Degree2Rad(45.f) && _angle_y < Degree2Rad(135.f))
				{
					hit_dir_idx = 1;
				}
				else if (_angle_y >= Degree2Rad(135.f) && _angle_y < Degree2Rad(225.f))
				{
					hit_dir_idx = 2;
				}
				else
				{
					hit_dir_idx = 3;
				}
			}
			break;

			case ATK_TYPE::PUSH_WEAK:
			{
				hit_anim_idx = 1;
				if (_angle_y < Degree2Rad(90.f) || _angle_y >= Degree2Rad(270.f))
				{
					hit_dir_idx = 0;
				}
				else
				{
					hit_dir_idx = 1;
				}
			}
			break;

			case ATK_TYPE::PUSH_STRONG:
			{
				hit_anim_idx = 2;
				if (_angle_y < Degree2Rad(90.f) || _angle_y >= Degree2Rad(270.f))
				{
					hit_dir_idx = 0;
				}
				else
				{
					hit_dir_idx = 1;
				}
			}
			break;

			case ATK_TYPE::UP:
			{
				hit_anim_idx = 3;
				if (_angle_y < Degree2Rad(90.f) || _angle_y >= Degree2Rad(270.f))
				{
					hit_dir_idx = 0;
				}
				else
				{
					hit_dir_idx = 1;
				}
			}
			break;

			default: break;
			}

			ChangeState(CLOUD_STATE::HIT);
		}

		CharacterDataScript::Hit(_type, _damage, _contact_pos);

		if (GetCurHP() <= 0.f)
		{
			SetCurHP(500.f);
		}
		// UIMgr::GetInst()->PrintWorldTextUI(to_string(_damage), false, true, _contact_pos);
	}

	void CloudMoveScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			GetCollider3D()->SetGravity(false);
		}
	}

	void CloudMoveScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void CloudMoveScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			GetCollider3D()->SetGravity(true);
		}
	}

	void CloudMoveScript::SetWeapon(CloudWeaponScript* _weapon)
	{
		weapon = _weapon;
		weapon->SetCloud(this);

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

	void CloudMoveScript::RotateCloud(float _origin, float _rot_y)
	{
		rot_y_time = 0.0f;		
		rot_y_end = _origin + _rot_y;

		if (is_rot_y)
		{
			rot_y_start = GetTransform()->GetRelativeRot().y;
		}
		else
		{
			rot_y_start = _origin;
		}

		is_rot_y = true;
	}

	void CloudMoveScript::RotateCloudToTarget()
	{
		if (target)
		{
			animator->CalcWorldDir();
			vec3 _front_dir = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _target_dir = target->GetTransform()->GetWorldPos() - GetTransform()->GetWorldPos();

			//cloud->GetTransform()->SetRelativeRotY(_cam_rot - 90.f * 3.141592 / 180.f + rot_y[(UINT)cur_dir]);

			float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

			_angle = fmodf(_angle, XM_2PI);
			if (fabs(_angle) < Degree2Rad(10.f) || fabs(_angle) > Degree2Rad(350.f))
			{
				GetTransform()->ShiftRelativeRotY(_angle);

			}
			else
			{
				if (fabs(_angle) > XM_PI)
				{
					_angle = _angle > XM_PI ? _angle - XM_2PI : _angle + XM_2PI;
				}

				RotateCloud(GetTransform()->GetRelativeRot().y, _angle);
			}

		}
	}

	float CloudMoveScript::GetPlayerCameraRotY()
	{
		if (player_camera)
		{
			return player_camera->GetTransform()->GetRelativeRot().y - Degree2Rad(90.f);
		}

		return 0.0f;
	}

	bool CloudMoveScript::IsPlayerAtkState()
	{
		if (state == CLOUD_STATE::ATTACK || state == CLOUD_STATE::ABILITY || state == CLOUD_STATE::LIMIT)
		{
			return true;
		}

		return false;
	}

	void CloudMoveScript::RegisterCharacterAbility(wstring _ability_name)
	{
	}

	void CloudMoveScript::ExecuteCommand()
	{
		CommandData* _command = GetCommand();
		if (_command && state != CLOUD_STATE::HIT)
		{
			if ((target = _command->target) && target->GetActive())
			{
				if (_command->type == CommandData::COMMAND_TYPE::ABILITY)
				{
					if (atb_gauge >= _command->cost)
					{
						atb_gauge -= _command->cost;
						if (_command->name == L"Braver")
						{
							animator->SetTrigger(L"Braver_Param");
						}
						else if (_command->name == L"Counter Stance")
						{
							animator->SetTrigger(L"CounterStance_Param");
							SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\Battle_Resident [73].wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);
						}
						else if (_command->name == L"Blade Burst")
						{
							animator->SetTrigger(L"BladeBurst_Param");
						}
					}

					ChangeState(CLOUD_STATE::ABILITY);
				}
				else if (_command->type == CommandData::COMMAND_TYPE::MAGIC)
				{
					magic_name = L"";
					magic_name = _command->name;

					cur_mp -= _command->cost;
					ChangeState(CLOUD_STATE::MAGIC);
				}
				else if (_command->type == CommandData::COMMAND_TYPE::ITEM)
				{
					ChangeState(CLOUD_STATE::ITEM);
				}
				else if (_command->type == CommandData::COMMAND_TYPE::LIMIT)
				{
					limit_gauge = 0.f;
					ChangeState(CloudMoveScript::CLOUD_STATE::LIMIT);
				}
			}

			ClearCommand();
		}

		if (is_rot_y)
		{
			RotateY();
		}
	}

	void CloudMoveScript::RotateY()
	{
		rot_y_time += DT;

		if (rot_y_time >= ROT_Y_TIME)
		{
			is_rot_y = false;
			GetTransform()->SetRelativeRotY(rot_y_end);
			animator->CalcWorldDir();
			return;
		}

		float _rot_y = lerp(rot_y_start, rot_y_end, rot_y_time / ROT_Y_TIME);
		GetTransform()->SetRelativeRotY(_rot_y);
		animator->CalcWorldDir();
	}

	void CloudMoveScript::ChangeState(CLOUD_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}

		state = _next_state;

		state_machine->ChangeState(states[(UINT)_next_state]);
	}

	void CloudMoveScript::ChangeMoveState()
	{
		bool _is_control = GameMgr::GetInst()->IsControl(GetOwner());

		if (_is_control)
		{
			ChangeState(CLOUD_STATE::MOVE);
		}
		else
		{
			ChangeState(CLOUD_STATE::AI_MOVE);
		}
	}

	void CloudMoveScript::MoveTurnEnd()
	{
		animator->SetBool(L"Move_Turn_Param", false);
		dynamic_cast<CloudMoveState*>(states[(UINT)CLOUD_STATE::MOVE])->TurnEnd();
	}

	void CloudMoveScript::SetTimeSpeedSlow()
	{
		TimeMgr::GetInst()->SetTimeSpeed(time_speed);
	}

	void CloudMoveScript::SetTimeSpeedOrigin()
	{
		TimeMgr::GetInst()->SetTimeSpeed(1.0f);
	}

	void CloudMoveScript::CheckAIStateChange()
	{
		if (state == CLOUD_STATE::IDLE)
		{
			if (target)
				ChangeState(CLOUD_STATE::AI_ATTACK);

			else
			{
				vec3 _control_pos = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos();
				vec3 _pos = GetTransform()->GetWorldPos();
				vec3 _dist = _control_pos - _pos;
				_dist.y = 0.0f;
				if ((_dist).Length() > 10.0f)
				{
					ChangeState(CLOUD_STATE::AI_MOVE_ENTER);
				}
			}
		}
	}

	void CloudMoveScript::ChangeCinematicCameraLimit0()
	{
		limit0_cam_idx++;

		player_camera->GetScript<PlayerCameraScript>()->TurnCinematicCamera(L"Cloud_Limit0", limit0_cam_idx);
	}

	void CloudMoveScript::IncreaseCrossSlashEffect()
	{
		cross_slash_cnt++;

		if (cross_slash_cnt > 5)
		{
			cross_slash_cnt = 0;
			cross_slash_effect->SetActive(false);
		}
		else
		{
			SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\SE_PC0000_00_Cloud [8].wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);
			cross_slash_effect->SetActive(true);
			cross_slash_effect->GetScript<EffectScript>()->SetRangeX(vec2(0.0f, (float)cross_slash_cnt / 5.0f));
		}
	}

	void CloudMoveScript::OnBraverSlashEffect()
	{
		braver_slash_effect->SetActive(true);
		braver_slash_effect->GetScript<EffectScript>()->SetEnabled(true);
		OnWeaponCollider();
	}

	void CloudMoveScript::OffBraverSlashEffect()
	{
		braver_slash_effect->SetActive(false);
		braver_slash_effect->GetScript<EffectScript>()->SetEnabled(false);
		OffWeaponCollider();
	}

	void CloudMoveScript::OnWeaponCollider()
	{
		wstring _name = animator->GetCurState()->GetName();

		int _sound_idx = randi(40) + 29;
		SoundMgr::GetInst()->Play(L"sound\\Cloud\\Voice\\VO_PC0000_00_Cloud [" + to_wstring(_sound_idx) + L"].wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);


		if (_name == L"B_Attack0" || _name == L"B_Attack0_1" || _name == L"B_Attack0_2_2")
		{
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::ATK0);
		}
		else if (_name == L"B_Attack1")
		{
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::ATK1);
		}
		else if (_name == L"B_Attack2")
		{
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::ATK2);
		}
		else if (_name == L"B_Attack3")
		{
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::ATK3);
		}
		else if (_name == L"Braver")
		{
			SoundMgr::GetInst()->Play(L"sound\\Cloud\\SE\\SE_PC0000_00_Cloud [34].wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::BRAVER);
		}
		else
		{
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::ATK0);
		}

		weapon->SetCollider(true);
	}

	void CloudMoveScript::OffWeaponCollider()
	{
		wstring _name = animator->GetCurState()->GetName();
		if (_name == L"B_Attack3")
		{
			weapon->ChangeWeaponType(CloudWeaponScript::WEAPON_TYPE::DEFUALT);
		}

		weapon->SetCollider(false);
	}

	void CloudMoveScript::ShootMagic()
	{
		if (magic_name == L"Fire")
		{
			GameObject* fire = ObjectPoolMgr::GetInst()->GetGameObject(L"Magic Fire");
			fire->GetTransform()->SetRelativePos(GetOwner()->FindChildObject(L"Magic Socket")->GetTransform()->GetWorldPos());
			fire->GetScript<MagicFireScript>()->SetTargetPos(target->GetTransform()->GetWorldPos() + target->GetCollider3D()->GetOffsetPos());
		}
	}

	void CloudMoveScript::BladeBurst()
	{
		float rot_y = GetTransform()->GetRelativeRot().y + Degree2Rad(90.f);

		vec3 _front = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 _left = RotateVec3YAxis(_front, Degree2Rad(-15.f));
		vec3 _right = RotateVec3YAxis(_front, Degree2Rad(15.f));

		vec3 _pos = GetTransform()->GetWorldPos();

		GameObject* blade_burst = ObjectPoolMgr::GetInst()->GetGameObject(L"Blade Burst");
		blade_burst->GetTransform()->SetRelativePos(_pos + _front * 1.5f);
		blade_burst->GetTransform()->SetRelativeRotY(rot_y);

		blade_burst = ObjectPoolMgr::GetInst()->GetGameObject(L"Blade Burst");
		blade_burst->GetTransform()->SetRelativePos(_pos + _left * 1.5f);
		blade_burst->GetTransform()->SetRelativeRotY(rot_y + Degree2Rad(-15.f));

		blade_burst = ObjectPoolMgr::GetInst()->GetGameObject(L"Blade Burst");
		blade_burst->GetTransform()->SetRelativePos(_pos + _right * 1.5f);
		blade_burst->GetTransform()->SetRelativeRotY(rot_y + Degree2Rad(15.f));
	}

	void CloudMoveScript::RegisterAnimationEvents()
	{
		animator->RegisterAnimationEvent(L"ChangeIdleState", [this]() { ChangeIdleState(); });
		animator->RegisterAnimationEvent(L"ChangeMoveState", [this]() { ChangeMoveState(); });
		animator->RegisterAnimationEvent(L"MoveTurnEnd", [this]() { MoveTurnEnd(); });
		animator->RegisterAnimationEvent(L"SetTimeSpeedSlow", [this]() { SetTimeSpeedSlow(); });
		animator->RegisterAnimationEvent(L"SetTimeSpeedOrigin", [this]() { SetTimeSpeedOrigin(); });
		animator->RegisterAnimationEvent(L"OnBraverSlashEffect", [this]() { OnBraverSlashEffect(); });
		animator->RegisterAnimationEvent(L"OffBraverSlashEffect", [this]() { OffBraverSlashEffect(); });
		animator->RegisterAnimationEvent(L"IncreaseCrossSlashEffect", [this]() { IncreaseCrossSlashEffect(); });
		animator->RegisterAnimationEvent(L"ChangeCinematicCameraLimit0", [this]() { ChangeCinematicCameraLimit0(); });
		animator->RegisterAnimationEvent(L"OnWeaponCollider", [this]() { OnWeaponCollider(); });
		animator->RegisterAnimationEvent(L"OffWeaponCollider", [this]() { OffWeaponCollider(); });
		animator->RegisterAnimationEvent(L"OnWeaponCounterEffect", [this]() { OnWeaponCounterEffect(); });
		animator->RegisterAnimationEvent(L"OffWeaponCounterEffect", [this]() { OffWeaponCounterEffect(); });
		animator->RegisterAnimationEvent(L"ShootMagic", [this]() { ShootMagic(); });
		animator->RegisterAnimationEvent(L"BladeBurst", [this]() { BladeBurst(); });
	}

	void CloudMoveScript::RegisterSkillData()
	{
		SkillData _braver;
		_braver.name = L"Braver";
		_braver.cost = 1;
		_braver.info_text = L"Hello !";

		SetAbilityData(_braver);

		_braver.name = L"Counter Stance";
		_braver.cost = 1;
		_braver.info_text = L"Hi !";

		SetAbilityData(_braver);

		_braver.name = L"Blade Burst";
		_braver.cost = 1;
		_braver.info_text = L"Hi !";

		SetAbilityData(_braver);

		_braver.name = L"Fire";
		_braver.cost = 4;
		_braver.info_text = L"So Hot!";

		SetMagicData(_braver);

		_braver.name = L"Heal";
		_braver.cost = 1;
		_braver.info_text = L"HP!";

		SetItemData(_braver);

		_braver.name = L"Cross Slash";
		_braver.cost = 1;
		_braver.info_text = L"Limit!";

		SetLimitData(_braver);
	}

	void CloudMoveScript::CreateChild()
	{
		Ptr<MeshData> _meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Character\\Player\\PC0000_Cloud\\Model\\FX_PC0000_CrossSlash_01_M.gltf");
		if (_meshdata != nullptr)
		{
			cross_slash_effect = _meshdata->Instantiate();
			cross_slash_effect->SetName(L"Cross Slash");
			cross_slash_effect->GetTransform()->SetRelativePos(2.5f, 1.f, 0.f);
			cross_slash_effect->GetTransform()->SetRelativeRot(0.f, 0.f, Degree2Rad(270.f));
			cross_slash_effect->SetLayerIndex(8);
			cross_slash_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
			cross_slash_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(vec4(1.0f, 0.3f, 0.1f, 1.0f)));
			cross_slash_effect->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0
				, ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Character\\Player\\PC0000_Cloud\\Texture\\FX_PC0000_CrossSlash_01_T.png"));

			cross_slash_effect->AddComponent(new EffectScript);
			cross_slash_effect->GetScript<EffectScript>()->SetIntensity(5.0f);

			GetOwner()->AddChild(cross_slash_effect);

			cross_slash_effect->SetActive(false);
		}

		_meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Ring_05_M.gltf");
		if (_meshdata != nullptr)
		{
			braver_slash_effect = _meshdata->Instantiate();
			braver_slash_effect->SetName(L"Braver_Slash_Effect");
			braver_slash_effect->GetTransform()->SetRelativeScale(3.f, 3.f, 3.f);
			braver_slash_effect->GetTransform()->SetRelativePos(0, 1.5f, 0);
			braver_slash_effect->GetTransform()->SetRelativeRot(0, Degree2Rad(-81.f), 0);
			braver_slash_effect->SetLayerIndex(8);
			braver_slash_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
			braver_slash_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(0.2f, 0.3f, 0.8f, 1.0f));
			braver_slash_effect->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0
				, ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Smk_UV_01_T.png"));

			braver_slash_effect->AddComponent(new EffectScript);
			braver_slash_effect->GetScript<EffectScript>()->SetIntensity(4.0f);
			braver_slash_effect->GetScript<EffectScript>()->SetIncreaseSpeed(3.5f);
			braver_slash_effect->GetScript<EffectScript>()->SetMaxUVLength(1.0f);
			braver_slash_effect->GetScript<EffectScript>()->IsChangeRange(false, true);

			braver_slash_effect->GetScript<EffectScript>()->SetEnabled(false);	
			braver_slash_effect->SetActive(false);

			GetOwner()->AddChild(braver_slash_effect);
		}
	}
}