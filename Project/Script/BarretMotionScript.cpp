#include "pch.h"
#include <Engine\GameObject.h>
#include <Engine\Transform.h>
#include <Engine\Animator3D.h>

#include "StateMachine.h"
#include "BarretState.h"
#include "BarretMotionScript.h"
#include "BarretWeaponScript.h"
#include "CharacterDataScript.h"
#include "PlayerCameraScript.h"
#include "BWeaponLimitEffectScript.h"
#include "FindEnemyScript.h"
#include "GameMgr.h"
#include "ObjectPoolMgr.h"
#include "UIMgr.h"
#include "MagicFireScript.h"
#include <Engine\SoundMgr.h>

namespace ff7r
{
#define ROT_Y_TIME 0.13f

	BarretMotionScript::BarretMotionScript()
		: PlayerDataScript((UINT)SCRIPT_TYPE::BARRETMOTIONSCRIPT)
		, animator(nullptr)
		, state(BARRET_STATE::END)
		, state_machine(nullptr)
		, n_attack_idx(0)
		, player_camera(nullptr)
		, rot_y_start(0.0f)
		, rot_y_end(0.0f)
		, is_rot_y(false)
		, rot_y_time(0.0f)
		, rot_y_idx(0)
		, limit0_cam_idx(-1)
		, is_test(false)
		, weapon(nullptr)
		, magic_name(L"")
	{
		for (int i = 0; i < 10; i++)
		{
			sound_buf.push_back(-1);
		}
	}
	BarretMotionScript::~BarretMotionScript()
	{
		if (state_machine)
		{
			delete state_machine;
			state_machine = nullptr;
		}
		for (UINT i = 0; i < (UINT)BARRET_STATE::END; i++)
		{
			delete states[i];
			states[i] = nullptr;
		}
		ClearCommand();
	}
	void BarretMotionScript::Awake()
	{
		animator = GetAnimator3D();

		states.resize((UINT)BARRET_STATE::END);
		states[(UINT)BARRET_STATE::IDLE] = new BarretIdleState(this);
		states[(UINT)BARRET_STATE::MOVE_ENTER] = new BarretMoveEnterState(this);
		states[(UINT)BARRET_STATE::MOVE] = new BarretMoveState(this);
		states[(UINT)BARRET_STATE::ATTACK_ENTER] = new BarretAttackEnterState(this);
		states[(UINT)BARRET_STATE::ATTACK] = new BarretAttackState(this);
		states[(UINT)BARRET_STATE::AI_MOVE_ENTER] = new BarretAIMoveEnterState(this);
		states[(UINT)BARRET_STATE::AI_MOVE] = new BarretAIMoveState(this);
		states[(UINT)BARRET_STATE::AI_ATTACK_ENTER] = new BarretAIAttackEnterState(this);
		states[(UINT)BARRET_STATE::AI_ATTACK] = new BarretAIAttackState(this);
		states[(UINT)BARRET_STATE::AVOID] = new BarretAvoidState(this);
		//states[(UINT)BARRET_STATE::OVERCHARGE] = new BarretOverChargeState(this);
		states[(UINT)BARRET_STATE::ABILITY] = new BarretAbilityState(this);
		states[(UINT)BARRET_STATE::MAGIC] = new BarretMagicState(this);
		states[(UINT)BARRET_STATE::LIMIT] = new BarretLimitState(this);

		state_changes.resize((UINT)BARRET_STATE::END);
		state_changes[(UINT)BARRET_STATE::IDLE] |= (UINT)1 << (UINT)KEY::W;
		state_changes[(UINT)BARRET_STATE::IDLE] |= (UINT)1 << (UINT)KEY::LBTN;
		state_changes[(UINT)BARRET_STATE::IDLE] |= (UINT)1 << (UINT)KEY::RBTN;
		state_changes[(UINT)BARRET_STATE::IDLE] |= (UINT)1 << (UINT)KEY::O;
		state_changes[(UINT)BARRET_STATE::IDLE] |= (UINT)1 << (UINT)KEY::L;
		state_changes[(UINT)BARRET_STATE::MOVE] |= (UINT)1 << (UINT)KEY::LBTN;
		state_changes[(UINT)BARRET_STATE::MOVE] |= (UINT)1 << (UINT)KEY::RBTN;
		state_changes[(UINT)BARRET_STATE::ATTACK] |= (UINT)1 << (UINT)KEY::RBTN;


		state = BARRET_STATE::IDLE;
		state_machine = new StateMachine(states[(UINT)BARRET_STATE::IDLE]);

		animator->RegisterAnimationEvent(L"ChangeIdleState", [this]() { ChangeIdleState(); });
		animator->RegisterAnimationEvent(L"ChangeMoveState", [this]() { ChangeMoveState(); });
		animator->RegisterAnimationEvent(L"ChangeAttackState", [this]() { ChangeAttackState(); });
		animator->RegisterAnimationEvent(L"ChargeLimit", [this]() { LimitCharge(); });
		animator->RegisterAnimationEvent(L"MoveTurnEnd", [this]() { MoveTurnEnd(); });
		animator->RegisterAnimationEvent(L"LookTarget", [this]() { LookTarget(); });
		animator->RegisterAnimationEvent(L"WeaponLimit", [this]() { WeaponLimit(); });
		animator->RegisterAnimationEvent(L"UseBullet", [this]() { UseBullet(); });
		//animator->RegisterAnimationEvent(L"AttackEnd", [this]() { AttackEnd(); });

		animator->RegisterAnimationEvent(L"WeaponAttack", [this]() { WeaponAttack(); });
		animator->RegisterAnimationEvent(L"ChangeCinematicCameraLimit0", [this]() { ChangeCinematicCameraLimit0(); });

		//animator->RegisterAnimationEvent(L"WalkSoundOn", [this]() {SoundOn(0); });
		//animator->RegisterAnimationEvent(L"WalkSoundOff", [this]() {SoundOff(0); });
		//animator->RegisterAnimationEvent(L"AvoidSoundOn", [this]() {SoundOn(1); });
		//animator->RegisterAnimationEvent(L"AvoidSoundOff", [this]() {SoundOff(1); });
		//animator->RegisterAnimationEvent(L"EffectOff", [this]() { OffEffectScript(); });
		//animator->RegisterAnimationEvent(L"OnBraverSlashEffect", [this]() { OnBraverSlashEffect(); });
		//animator->RegisterAnimationEvent(L"OffBraverSlashEffect", [this]() { OffBraverSlashEffect(); });
		//animator->RegisterAnimationEvent(L"ChangeCinematicCameraLimit0", [this]() { ChangeCinematicCameraLimit0(); });

		SkillData _ability;
		_ability.name = L"UpperCut";
		_ability.cost = 1;
		_ability.info_text = L"Barret AtkBurst 07_0";
		SetAbilityData(_ability);

		_ability;
		_ability.name = L"SmackDown";
		_ability.cost = 1;
		_ability.info_text = L"Barret AtkBurst 05_0";
		SetAbilityData(_ability);

		_ability;
		_ability.name = L"MaximumFury";
		_ability.cost = 1;
		_ability.info_text = L"Barret AtkBurst 04_0";
		SetAbilityData(_ability);

		_ability;
		_ability.name = L"FireInTheHole";
		_ability.cost = 1;
		_ability.info_text = L"Barret Limit";
		SetAbilityData(_ability);

		SkillData _magic;
		_magic.name = L"Fire";
		_magic.cost = 1;
		_magic.info_text = L"hot";
		SetMagicData(_magic);

		SkillData item;
		_magic.name = L"Cure";
		_magic.cost = 1;
		_magic.info_text = L"warm";
		SetMagicData(_magic);

		// 테스트용 코드
		if (animator->GetAnimStates().size() <= 1)
		{
			if (animator->GetAnimStates().empty())
			{
				// default state 없어짐
				assert(nullptr);
			}
		}
	}
	void BarretMotionScript::Start()
	{

	}
	void BarretMotionScript::Tick()
	{

		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::T))
		{
			//GameMgr::GetInst()->ChangeLevel();
			SetTest(GetTest() ? false : true);
		}
		if (target && !target->IsDead())
		{
			if (!GetOwner()->GetScript<FindEnemyScript>()->CheckEnemyDistance(target))
			{
				target = nullptr;
			}
			else if (GetTest() && KEY_TAP(KEY::T))
			{
				target = GetOwner()->GetScript<FindEnemyScript>()->FindClosestEnemy();
			}
		}
		else
		{
			target = GetOwner()->GetScript<FindEnemyScript>()->FindClosestEnemy();
		}


		bool _is_control = GameMgr::GetInst()->IsControl(GetOwner());

		if (_is_control)
		{
			CommandData* _command = GetCommand();
			if (_command)
			{
				if (state != BARRET_STATE::HIT)
				{
					if (_command->type == CommandData::COMMAND_TYPE::ABILITY)
					{
						if (_command->name == L"FireInTheHole")
						{
							animator->SetTrigger(L"Limit_Param");
							ChangeState(BARRET_STATE::LIMIT);
						}
						else
						{

							if (_command->name == L"SmackDown")
							{
								animator->SetInteger(L"Ability_Index_Param", 1);
							}
							else if (_command->name == L"UpperCut")
							{
								animator->SetInteger(L"Ability_Index_Param", 2);
							}
							else if (_command->name == L"MaximumFury")
							{
								animator->SetInteger(L"Ability_Index_Param", 3);
							}
							ChangeState(BARRET_STATE::ABILITY);
						}

					}
					else if (_command->type == CommandData::COMMAND_TYPE::MAGIC)
					{
						if (_command->name == L"Fire")
						{
							animator->SetTrigger(L"Magic_Param");
							ChangeState(BARRET_STATE::MAGIC);

						}
						else if (_command->name == L"Cure")
						{
							animator->SetTrigger(L"Item_Param");
							ChangeState(BARRET_STATE::ITEM);
						}
					}
					/*else if (_command->type == CommandData::COMMAND_TYPE::LIMIT)
					{


						ChangeState(BARRET_STATE::LIMIT);
					}*/
					/*else if (_command->type == CommandData::COMMAND_TYPE::ITEM)
					{

						ChangeState(BARRET_STATE::ITEM);
					}*/
					/*else if (command->type == CommandData::COMMAND_TYPE::MAGIC)
					{
						magic_name = L"";
						magic_name = _command->name;

						cur_mp -= _command->cost;
						ChangeState(BARRET_STATE::MAGIC);
					}*/

					ClearCommand();
				}

				if (is_rot_y)
				{
					RotateY();
				}
				return;
			}

			CheckStateChange();

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

	}
	void BarretMotionScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
			GetCollider3D()->SetGravity(false);
	}
	void BarretMotionScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretMotionScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretMotionScript::Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos)
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

			ChangeState(BARRET_STATE::HIT);
		}

		UIMgr::GetInst()->PrintWorldTextUI(to_string(_damage), false, true, _contact_pos);
	}
	void BarretMotionScript::RotateBarret(float _origin, float _rot_y)
	{
		if (is_rot_y)
		{
			GetTransform()->SetRelativeRotY(rot_y_end);
		}

		rot_y_time = 0.0f;
		rot_y_start = _origin;
		rot_y_end = _origin + _rot_y;
		is_rot_y = true;
	}
	void BarretMotionScript::LookTarget(bool _now)
	{
		GameObject* _target = GetTarget();
		if (_target != nullptr)
		{
			animator->CalcWorldDir();
			vec3 _front_dir = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _target_dir = _target->GetTransform()->GetWorldPos() - GetTransform()->GetWorldPos();
			float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

			_angle = fmodf(_angle, XM_2PI);
			//_angle = Rad2Degree(_angle);

			int _turn_type = 0;

			float _rot_y = GetTransform()->GetRelativeRot().y;

			if (_now)
			{
				GetTransform()->SetRelativeRotY(_rot_y + _angle);
			}
			else
			{
				RotateBarret(_rot_y, _angle);
			}
		}


	}
	void BarretMotionScript::AttackEnd()
	{
		animator->SetBool(L"Attack_Param", false);
	}
	float BarretMotionScript::GetPlayerCameraRotY()
	{
		if (player_camera)
		{
			return player_camera->GetTransform()->GetRelativeRot().y - Degree2Rad(90.f);
		}

		return 0.0f;
	}
	void BarretMotionScript::RotateY()
	{
		rot_y_time += DT;

		if (rot_y_time >= ROT_Y_TIME)
		{
			is_rot_y = false;
			GetTransform()->SetRelativeRotY(rot_y_end);
			return;
		}

		float _rot_y = lerp(rot_y_start, rot_y_end, rot_y_time / ROT_Y_TIME);
		GetTransform()->SetRelativeRotY(_rot_y);
	}

	void BarretMotionScript::ChangeState(BARRET_STATE _next_state)
	{
		if (state == _next_state)
		{
			return;
		}
		state = _next_state;

		state_machine->ChangeState(states[(UINT)_next_state]);
	}
	void BarretMotionScript::ChangeMoveState()
	{
		bool _is_control = GameMgr::GetInst()->IsControl(GetOwner());

		if (_is_control)
		{
			ChangeState(BARRET_STATE::MOVE);
		}
		else
		{
			ChangeState(BARRET_STATE::AI_MOVE);
		}
	}
	void BarretMotionScript::ChangeAttackState()
	{
		bool _is_control = GameMgr::GetInst()->IsControl(GetOwner());

		if (_is_control)
		{
			ChangeState(BARRET_STATE::ATTACK);
		}
		else
		{
			ChangeState(BARRET_STATE::AI_ATTACK);
		}
	}
	void BarretMotionScript::LimitCharge()
	{
		weapon->GetTransform()->CalcWorldDir();
		GameObject* _limit_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Limit Effect");
		_limit_effect->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos());
		_limit_effect->GetScript<BWeaponLimitEffectScript>()->SetDir(weapon->GetTransform()->GetWorldDir(DIR_TYPE::FRONT));
	}
	void BarretMotionScript::WeaponAttack()
	{
		if (weapon != nullptr && target != nullptr)
		{
			weapon->Attack();
		}
	}
	void BarretMotionScript::WeaponLimit()
	{
		weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::LIMIT_ATTACK);
	}
	void BarretMotionScript::UseBullet()
	{
		normal_bullet--;
		if (normal_bullet == 0)
		{
			animator->SetBool(L"Reload_Param", true);
		}
	}
	void BarretMotionScript::ChangeCinematicCameraLimit0()
	{

		limit0_cam_idx++;

		player_camera->GetScript<PlayerCameraScript>()->TurnCinematicCamera(L"Barret_Limit0", limit0_cam_idx);
	}
	void BarretMotionScript::SoundOn(int _idx)
	{
		switch (_idx)
		{
		case 0:
		{
			if (sound_buf[0] == -1)
				sound_buf[0] = (SoundMgr::GetInst()->Play(L"sound\\barret_metal1_run.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 0));

			break;
		}
		case 1:
		{
			sound_buf[1] = (SoundMgr::GetInst()->Play(L"sound\\avoid1.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1));
			break;
		}
		//case 2 :
		//{
		//	SoundMgr::GetInst()->Play(L"sound\AtkShoot01_finish_shot.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 0);
		//	break;
		//}
		//case 3:
		//{
		//	SoundMgr::GetInst()->Play(L"sound\AtkShoot01_finish_shot.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 0);
		//	break;
		//}
		}
	}
	void BarretMotionScript::SoundOff(int _idx)
	{
		if (sound_buf[_idx] != -1)
		{
			SoundMgr::GetInst()->Stop(sound_buf[_idx], GetOwner());
			sound_buf[_idx] = -1;
		}

	}
	void BarretMotionScript::OnEffectScript()
	{
	}
	void BarretMotionScript::OffEffectScript()
	{
	}
	void BarretMotionScript::CheckStateChange()
	{
		// AddForce 예제 //
		if (GetTest() && KEY_TAP(KEY::P))
		{
			//GetOwner()->GetCollider3D()->AddForce(vec3(0.0f, 1000.0f, 0.0f));
			GetOwner()->GetCollider3D()->AddForce(vec3(0.0f, 500.0f, 0.0f));
		}
		// AddForce 예제 //
		if (INPUT_MOVE_KEY)
		{
			if (CheckChangeKey(KEY::W))
			{
				ChangeState(BARRET_STATE::MOVE_ENTER);
				return;
			}
		}
		if (GetTest() && KEY_TAP(KEY::LBTN))
		{
			if (CheckChangeKey(KEY::LBTN))
			{
				ChangeState(BARRET_STATE::ATTACK_ENTER);
				return;
			}
		}
		if (GetTest() && KEY_TAP(KEY::RBTN))
		{
			if (CheckChangeKey(KEY::RBTN))
			{
				ChangeState(BARRET_STATE::AVOID);
				return;
			}
		}
		if (GetTest() && KEY_TAP(KEY::L))
		{
			if (CheckChangeKey(KEY::L))
				ChangeState(BARRET_STATE::LIMIT);
		}
		if (GetTest() && KEY_TAP(KEY::M))
		{
			//if (CheckChangeKey(KEY::M))
			//	ChangeState(BARRET_STATE::MAGIC);
		}
		if (GetTest() && KEY_TAP(KEY::O))
		{
			/*if (CheckChangeKey(KEY::O))
			{
				ChangeState(BARRET_STATE::OVERCHARGE);
				return;
			}*/

		}
	}
	void BarretMotionScript::CheckAIStateChange()
	{

		if (state == BARRET_STATE::IDLE)
		{
			if (target)
				ChangeState(BARRET_STATE::AI_ATTACK_ENTER);

			else
			{
				vec3 _control_pos = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos();
				vec3 _pos = GetTransform()->GetWorldPos();
				vec3 _dist = _control_pos - _pos;
				_dist.y = 0.0f;
				if ((_dist).Length() > 10.0f)
				{
					ChangeState(BARRET_STATE::AI_MOVE_ENTER);
				}
			}
		}
	}
	bool BarretMotionScript::CheckChangeKey(KEY _key)
	{
		if (state_changes[(UINT64)state] & 1 << (UINT64)(_key))
			return true;
		return false;
	}
	void BarretMotionScript::SetWeapon(BarretWeaponScript* _weapon)
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
	bool BarretMotionScript::IsPlayerAtkState()
	{
		{
			if (state == BARRET_STATE::ATTACK || state == BARRET_STATE::ABILITY || state == BARRET_STATE::LIMIT)
			{
				return true;
			}

			return false;
		}
	}
	void BarretMotionScript::MoveTurnEnd()
	{
		animator->SetBool(L"Move_Turn_Param", false);
		dynamic_cast<BarretMoveState*>(states[(UINT)BARRET_STATE::MOVE])->TurnEnd();
	}

	void BarretMotionScript::ShootMagic()
	{
		if (magic_name == L"Fire")
		{
			GameObject* fire = ObjectPoolMgr::GetInst()->GetGameObject(L"Magic Fire");
			fire->GetTransform()->SetRelativePos(GetOwner()->FindChildObject(L"Magic Socket")->GetTransform()->GetWorldPos());
			fire->GetScript<MagicFireScript>()->SetTargetPos(target->GetTransform()->GetWorldPos() + target->GetCollider3D()->GetOffsetPos());
		}


	}

	void BarretMotionScript::CreateEffect()
	{
		//GameObject* _effect = new GameObject();
		//Ptr<MeshData> _meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Character\\Player\\PC0000_Cloud\\Model\\FX_PC0000_CrossSlash_01_M.gltf");
		///*if (_meshdata != nullptr)
		//{
		//	cross_slash_effect = _meshdata->Instantiate();
		//	cross_slash_effect->SetName(L"Cross Slash");
		//	cross_slash_effect->GetTransform()->SetRelativePos(2.5f, 1.f, 0.f);
		//	cross_slash_effect->GetTransform()->SetRelativeRot(0.f, 0.f, Degree2Rad(270.f));
		//	cross_slash_effect->SetLayerIndex(8);
		//	cross_slash_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		//	cross_slash_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(vec4(1.0f, 0.3f, 0.1f, 1.0f)));
		//	cross_slash_effect->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0
		//		, ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Character\\Player\\PC0000_Cloud\\Texture\\FX_PC0000_CrossSlash_01_T.png"));

		//	cross_slash_effect->AddComponent(new EffectScript);
		//	cross_slash_effect->GetScript<EffectScript>()->SetIntensity(5.0f);

		//	GetOwner()->AddChild(cross_slash_effect);

		//	cross_slash_effect->SetActive(false);
		//}*/

		//_meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Flash_01_M.gltf");
		//if (_meshdata != nullptr)
		//{
		//	_effect = _meshdata->Instantiate();
		//	_effect->SetName(L"Normal Attack Effect");
		//	_effect->GetTransform()->SetRelativeScale(3.f, 3.f, 3.f);
		//	_effect->GetTransform()->SetRelativePos(2.5, 1.5f, 0);
		//	_effect->GetTransform()->SetRelativeRot(0, 0, Degree2Rad(270.f));
		//	_effect->SetLayerIndex(8);
		//	_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		//	_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(1.0f, 0.5f, 0.0f, 1.0f));
		//	_effect->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0
		//		, ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Fire_UV_03_T.png"));

		//	_effect->AddComponent(new EffectScript);
		//	_effect->GetScript<EffectScript>()->SetIntensity(4.0f);
		//	_effect->GetScript<EffectScript>()->SetMaxUVLength(1.0f);
		//	_effect->GetScript<EffectScript>()->IsChangeRange(false, false);

		//	_effect->GetScript<EffectScript>()->SetEnabled(false);
		//	_effect->SetActive(false);

		//	GetOwner()->AddChild(_effect);
		//}
	}
	void BarretMotionScript::Reload()
	{
		animator->SetBool(L"Reload_Param", false);

		normal_bullet = 20;


	}
	/*void BarretMotionScript::ChangeCinematicCameraLimit0()
	{
	}
	void BarretMotionScript::OnBraverSlashEffect()
	{
	}
	void BarretMotionScript::OffBraverSlashEffect()
	{
	}*/
}