#include "pch.h"
#include "BarretState.h"
#include "BarretWeaponScript.h"
#include "GameMgr.h"
#include "ObjectPoolMgr.h"

namespace ff7r
{
	void BarretIdleState::Enter()
	{

	}
	void BarretIdleState::Excute()
	{
		if (INPUT_MOVE_KEY)
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::MOVE_ENTER);
		}
		else if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::LBTN))
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::ATTACK_ENTER);
		}
		else if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::RBTN))
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::AVOID);
		}
		else if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::L))
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::LIMIT);
		}
	}
	void BarretIdleState::Exit()
	{
	}

	void BarretMoveEnterState::Enter()
	{
		barret->animator->SetBool(L"Move_Param", true);

		float camera_rot_y = barret->GetPlayerCameraRotY();
		float player_rot_y = barret->GetTransform()->GetRelativeRot().y;


		if (abs(camera_rot_y - player_rot_y) > 5.0f)
			int a = -0;
		barret->rot_y_idx = 0;
		if (KEY_INPUT(KEY::A))
		{
			camera_rot_y += -XM_PI / 2.f;
			barret->rot_y_idx = 3;
		}
		else if (KEY_INPUT(KEY::S))
		{
			camera_rot_y += XM_PI;
			barret->rot_y_idx = 2;
		}
		else if (KEY_INPUT(KEY::D))
		{
			camera_rot_y += XM_PI / 2.f;
			barret->rot_y_idx = 1;
		}

		camera_rot_y = fmodf(camera_rot_y, XM_PI * 2.f);
		player_rot_y = fmodf(player_rot_y, XM_PI * 2.f);

		rot_y = camera_rot_y;

		float rotY = camera_rot_y - player_rot_y;
		rotY = Rad2Degree(rotY);

		int _turn_type = (int)TURN_TYPE::END;
		if (fabs(rotY) <= 5.f)
		{
			_turn_type = (int)TURN_TYPE::NONE;
		}
		else if (fabs(rotY) <= 175.f)
		{
			if (rotY > 0)
			{
				_turn_type = (int)TURN_TYPE::R90;
				barret->animator->SetRot(rotY / 90.f, Degree2Rad(90.f));
			}
			else
			{
				_turn_type = (int)TURN_TYPE::L90;
				barret->animator->SetRot(rotY / 90.f, Degree2Rad(-90.f));
			}
			//_turn_type = rotY > 0 ? (int)TURN_TYPE::R90 : (int)TURN_TYPE::L90;
		}
		else if (fabs(rotY) <= 185.f)
		{
			if (rotY > 0)
			{
				_turn_type = (int)TURN_TYPE::R180;
				barret->animator->SetRot(rotY / 180.f, Degree2Rad(180.f));
			}
			else
			{
				_turn_type = (int)TURN_TYPE::R180;
				barret->animator->SetRot(rotY / 180.f, Degree2Rad(180.f));
			}
			//_turn_type = rotY > 0 ? (int)TURN_TYPE::R180 : (int)TURN_TYPE::R180;
		}
		//else if (fabs(rotY) <= 275.f)
		//{
		//	if (rotY < 0)
		//	{
		//		_turn_type = (int)TURN_TYPE::R90;
		//		barret->animator->SetRot((rotY + 180.f) / 90.f, Degree2Rad(90.f));
		//	}
		//	else
		//	{
		//		_turn_type = (int)TURN_TYPE::L90;
		//		barret->animator->SetRot((rotY - 180.f) / 90.f, Degree2Rad(-90.f));
		//	}
		//	//_turn_type = rotY < 0 ? (int)TURN_TYPE::R90 : (int)TURN_TYPE::L90;
		//}
		else if (fabs(rotY) <= 355.f)
		{
			if (rotY > 0)
			{
				_turn_type = (int)TURN_TYPE::L90;
				barret->animator->SetRot((360.f - rotY) / 90.f, Degree2Rad(-90.f));
			}
			else
			{
				_turn_type = (int)TURN_TYPE::R90;
				barret->animator->SetRot((360.f + rotY) / 90.f, Degree2Rad(90.f));
			}
		}
		else
		{
			_turn_type = (int)TURN_TYPE::NONE;
		}

		barret->animator->SetInteger(L"Run_Index_Param", _turn_type);
	}
	void BarretMoveEnterState::Excute()
	{
	}
	void BarretMoveEnterState::Exit()
	{
		barret->animator->SetRot(1.0f, 0.0f);
		barret->GetTransform()->SetRelativeRotY(rot_y);
	}

	void BarretMoveState::Enter()
	{
		prev_dir = (DIR_TYPE)(barret->rot_y_idx * 2);
		barret->SoundOn(0);
	}
	void BarretMoveState::Excute()
	{
		if (barret->animator->GetParameter(L"Move_Param")->data.b == false || is_turn)
		{
			return;
		}

		DIR_TYPE cur_dir = DIR_TYPE::END;

		if (KEY_INPUT(KEY::W))
		{
			if (KEY_INPUT(KEY::A))
			{
				cur_dir = DIR_TYPE::FRONT_LEFT;
			}
			else if (KEY_INPUT(KEY::D))
			{
				cur_dir = DIR_TYPE::FRONT_RIGHT;
			}
			else
			{
				cur_dir = DIR_TYPE::FRONT;
			}
		}
		else if (KEY_INPUT(KEY::S))
		{
			if (KEY_INPUT(KEY::A))
			{
				cur_dir = DIR_TYPE::BACK_LEFT;
			}
			else if (KEY_INPUT(KEY::D))
			{
				cur_dir = DIR_TYPE::BACK_RIGHT;
			}
			else
			{
				cur_dir = DIR_TYPE::BACK;
			}
		}
		else
		{
			if (KEY_INPUT(KEY::D))
			{
				cur_dir = DIR_TYPE::RIGHT;
			}
			else if (KEY_INPUT(KEY::A))
			{
				cur_dir = DIR_TYPE::LEFT;
			}
		}

		int diff = (int)prev_dir - (int)cur_dir;

		if (abs(diff) == 1 || abs(diff) == 7)
		{
			if (barret->player_camera)
			{
				float _cam_rot = barret->GetPlayerCameraRotY();
				//float _cur_rot = barret->GetTransform()->GetRelativeRot().y;
				float _rot_y = barret->GetTransform()->GetRelativeRot().y;

				//barret->GetTransform()->SetRelativeRotY(_cam_rot - 90.f * 3.141592 / 180.f + rot_y[(UINT)cur_dir]);

				if (abs(diff) == 1)
				{
					_rot_y = diff > 0 ? Degree2Rad(-45.f) : Degree2Rad(45.f);
				}
				else
				{
					_rot_y = diff < 0 ? Degree2Rad(-45.f) : Degree2Rad(45.f);
				}
				barret->RotateBarret(_cam_rot + rot_y[(UINT)prev_dir], _rot_y);
			}
		}
		else if (abs(diff) == 0)
		{
			if (!barret->is_rot_y && cur_dir != DIR_TYPE::END)
			{
				float _cam_rot = barret->GetPlayerCameraRotY();
				barret->GetTransform()->SetRelativeRotY(_cam_rot + rot_y[(UINT)cur_dir]);
				barret->animator->CalcWorldDir();
			}
		}
		else if (abs(diff) == 4)
		{
			if (cur_dir != DIR_TYPE::END)
			{
				barret->animator->SetBool(L"Move_Turn_Param", true);
				is_turn = true;
			}
		}

		prev_dir = cur_dir;

		if (!(INPUT_MOVE_KEY))
		{
			barret->animator->SetBool(L"Move_Param", false);
			barret->SoundOff(0);
		}
	}
	void BarretMoveState::Exit()
	{
		barret->SoundOff(0);
		barret->animator->SetBool(L"Move_Param", false);
	}
	void BarretAttackEnterState::Enter()
	{

		//float player_rot_y = barret->GetTransform()->GetRelativeRot().y;
		GameObject* _target = barret->GetTarget();
		if (_target != nullptr)
		{
			barret->animator->SetBool(L"Attack_Param", true);
			vec3 _front_dir = barret->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _target_dir = _target->GetTransform()->GetWorldPos() - barret->GetTransform()->GetWorldPos();
			float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

			rot_y = barret->GetTransform()->GetRelativeRot().y + _angle;


			//player_rot_y = fmodf(player_rot_y, 360.f);
			_angle = fmodf(_angle, XM_PI * 2.f);
			_angle = Rad2Degree(_angle);

			int _turn_type = (int)TURN_TYPE::END;
			if (fabs(_angle) <= 1.f)
			{
				_turn_type = (int)TURN_TYPE::NONE;
			}
			else if (fabs(_angle) <= 180.f)
			{
				if (_angle > 0)
				{
					_turn_type = (int)TURN_TYPE::R120;
					barret->animator->SetRot(_angle / 120.f, Degree2Rad(120.f));
				}
				else
				{
					_turn_type = (int)TURN_TYPE::L120;
					barret->animator->SetRot(_angle / 120.f, Degree2Rad(-1.f * 120.f));
				}
			}

			else if (fabs(_angle) <= 359.f)
			{
				if (_angle < 0)
				{
					_turn_type = (int)TURN_TYPE::R120;
					barret->animator->SetRot((360.f - _angle) / 120.f, Degree2Rad(120.f));
				}
				else
				{
					_turn_type = (int)TURN_TYPE::L120;
					barret->animator->SetRot((360.f - _angle) / 120.f, Degree2Rad(-1.f * 120.f));
				}
			}
			else
			{
				_turn_type = (int)TURN_TYPE::NONE;
			}

			barret->animator->SetInteger(L"N_Attack_Index_Param", _turn_type);
		}
		else
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
		}

	}
	void BarretAttackEnterState::Excute()
	{
	}
	void BarretAttackEnterState::Exit()
	{
		barret->animator->SetRot(1.0f, 0.0f);
		barret->GetTransform()->SetRelativeRotY(rot_y);
	}
	void BarretAttackState::Enter()
	{
		barret->weapon->SetTarget(barret->GetTarget());
		barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::NORMAL_ATTACK);
		barret->Reload();

	}
	void BarretAttackState::Excute()
	{

		GameObject* _target = barret->GetTarget();
		if (_target != nullptr)
		{
			vec3 _front_dir = barret->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _target_dir = _target->GetTransform()->GetWorldPos() - barret->GetTransform()->GetWorldPos();
			float _angle = CalcYAxisFromVector(_front_dir, _target_dir);


			int _turn_type = 0;

			float _cam_rot = barret->GetPlayerCameraRotY();
			float _rot_y = barret->GetTransform()->GetRelativeRot().y;


			//barret->GetTransform()->SetRelativeRotY(_rot_y + _angle);

			barret->LookTarget(false);

		}
		if (!(KEY_PRESSED(KEY::LBTN)))
		{
			barret->animator->SetBool(L"Attack_Param", false);

		}
	}
	void BarretAttackState::Exit()
	{
		barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SLEEP);
		barret->animator->SetBool(L"Attack_Param", false);
	}

	void BarretAvoidState::Enter()
	{
		barret->SoundOn(1);
		barret->animator->SetTrigger(L"Avoid_Param");

	}
	void BarretAvoidState::Excute()
	{
	}
	void BarretAvoidState::Exit()
	{
	}

	void BarretOverChargeState::Enter()
	{
		barret->animator->SetTrigger(L"Overcharge_Param");
	}
	void BarretOverChargeState::Excute()
	{
	}
	void BarretOverChargeState::Exit()
	{
	}

	void BarretAbilityState::Enter()
	{
		barret->LookTarget(false);
		barret->animator->SetTrigger(L"Ability");

		switch (barret->animator->GetParameter(L"Ability_Index_Param")->data.i)
		{
		case 1:
		{
			barret->weapon->SetTarget(barret->GetTarget());
			barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SMACKDOWN);
			break;
		}
		case 2:
		{
			barret->weapon->SetTarget(barret->GetTarget());
			barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::UPPERCUT);
			break;
		}
		case 3:
		{
			barret->weapon->SetTarget(barret->GetTarget());
			barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::MAXIMUM_FURY);
			break;
		}
		default:
			break;
		}
	}
	void BarretAbilityState::Excute()
	{
	}
	void BarretAbilityState::Exit()
	{
		barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SLEEP);
		barret->animator->SetInteger(L"Ability_Index_Param", -1);
	}

	void BarretMagicState::Enter()
	{
		barret->animator->SetTrigger(L"Magic_Param");

	}
	void BarretMagicState::Excute()
	{
	}
	void BarretMagicState::Exit()
	{
	}

	void BarretLimitState::Enter()
	{
		barret->LookTarget(false);
		barret->limit0_cam_idx = -1;

		barret->animator->SetTrigger(L"Limit");
		barret->weapon->SetTarget(barret->GetTarget());

	}
	void BarretLimitState::Excute()
	{
	}
	void BarretLimitState::Exit()
	{
		barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SLEEP);

	}
	void BarretAIAttackState::Enter()
	{
		//return;

		barret->weapon->SetTarget(barret->GetTarget());
		barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::NORMAL_ATTACK);
		barret->Reload();
	}
	void BarretAIAttackState::Excute()
	{
		if (GameMgr::GetInst()->IsControl(barret->GetOwner()))
		{
			barret->animator->SetBool(L"Attack_Param", false);
			barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SLEEP);
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
			return;
		}
		GameObject* _target = barret->GetTarget();
		if (_target != nullptr)
		{
			vec3 _front_dir = barret->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _target_dir = _target->GetTransform()->GetWorldPos() - barret->GetTransform()->GetWorldPos();
			float _angle = CalcYAxisFromVector(_front_dir, _target_dir);
			_target_dir.y = 0.0f;

			int _turn_type = 0;

			float _cam_rot = barret->GetPlayerCameraRotY();
			float _rot_y = barret->GetTransform()->GetRelativeRot().y;


			barret->LookTarget(false);

		}
		else
		{
			barret->animator->SetBool(L"Attack_Param", false);
			barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SLEEP);
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
			return;
		}
	}
	void BarretAIAttackState::Exit()
	{
		barret->weapon->ChangeState(BarretWeaponScript::BARRET_WEAPON_STATE::SLEEP);
		barret->animator->SetBool(L"Attack_Param", false);
	}
	void BarretAIAttackEnterState::Enter()
	{
		if (GameMgr::GetInst()->IsControl(barret->GetOwner()))
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
			return;
		}
		GameObject* _target = barret->GetTarget();
		if (_target != nullptr)
		{
			barret->animator->SetBool(L"Attack_Param", true);
			vec3 _front_dir = barret->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
			vec3 _target_dir = _target->GetTransform()->GetWorldPos() - barret->GetTransform()->GetWorldPos();
			_target_dir.y = 0.0f;
			float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

			rot_y = barret->GetTransform()->GetRelativeRot().y + _angle;


			//player_rot_y = fmodf(player_rot_y, 360.f);
			_angle = fmodf(_angle, XM_PI * 2.f);
			_angle = Rad2Degree(_angle);

			int _turn_type = (int)TURN_TYPE::END;
			if (fabs(_angle) <= 1.f)
			{
				_turn_type = (int)TURN_TYPE::NONE;
			}
			else if (fabs(_angle) <= 180.f)
			{
				if (_angle > 0)
				{
					_turn_type = (int)TURN_TYPE::R120;
					barret->animator->SetRot(_angle / 120.f, Degree2Rad(120.f));
				}
				else
				{
					_turn_type = (int)TURN_TYPE::L120;
					barret->animator->SetRot(_angle / 120.f, Degree2Rad(-1.f * 120.f));
				}
			}

			else if (fabs(_angle) <= 359.f)
			{
				if (_angle < 0)
				{
					_turn_type = (int)TURN_TYPE::R120;
					barret->animator->SetRot((360.f - _angle) / 120.f, Degree2Rad(120.f));
				}
				else
				{
					_turn_type = (int)TURN_TYPE::L120;
					barret->animator->SetRot((360.f - _angle) / 120.f, Degree2Rad(-1.f * 120.f));
				}
			}
			else
			{
				_turn_type = (int)TURN_TYPE::NONE;
			}

			barret->animator->SetInteger(L"N_Attack_Index_Param", _turn_type);
			barret->ChangeState(BarretMotionScript::BARRET_STATE::AI_ATTACK);
		}
		else
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
		}
	}
	void BarretAIAttackEnterState::Excute()
	{
	}
	void BarretAIAttackEnterState::Exit()
	{
		barret->animator->SetRot(1.0f, 0.0f);
		barret->GetTransform()->SetRelativeRotY(rot_y);
	}
	void BarretAIMoveState::Enter()
	{
		barret->SoundOn(0);

	}
	void BarretAIMoveState::Excute()
	{
		if (GameMgr::GetInst()->IsControl(barret->GetOwner()))
		{
			barret->animator->SetBool(L"Move_Param", false);
			return;
		}
		barret->animator->SetBool(L"Move_Param", true);
		barret->animator->CalcWorldDir();
		vec3 _front_dir = barret->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		vec3 _target_dir = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos() - barret->GetTransform()->GetWorldPos();
		_target_dir.y = 0.0f;
		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);
		_angle = fmodf(_angle, XM_2PI);




		if (_target_dir.Length() < 10.0f)
		{
			barret->SoundOff(0);

			barret->animator->SetBool(L"Move_Param", false);
		}
		else
		{
			float _rot_y = barret->GetTransform()->GetRelativeRot().y;
			barret->GetTransform()->SetRelativeRotY(_rot_y + _angle);
		}
	}
	void BarretAIMoveState::Exit()
	{
		barret->SoundOff(0);

	}
	void BarretAIMoveEnterState::Enter()
	{
		if (GameMgr::GetInst()->IsControl(barret->GetOwner()))
		{
			barret->animator->SetBool(L"Move_Param", false);
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
			return;
		}
		barret->animator->SetBool(L"Move_Param", true);
		barret->animator->CalcWorldDir();
		vec3 _front_dir = barret->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		vec3 _target_dir = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos() - barret->GetTransform()->GetWorldPos();
		_target_dir.y = 0.0f;
		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);
		_angle = fmodf(_angle, XM_2PI);



		if (_target_dir.Length() < 10.0f)
		{
			barret->animator->SetBool(L"Move_Param", false);
			barret->ChangeState(BarretMotionScript::BARRET_STATE::IDLE);
			return;
		}
		else
		{
			barret->ChangeState(BarretMotionScript::BARRET_STATE::AI_MOVE);
			float _rot_y = barret->GetTransform()->GetRelativeRot().y;
			barret->GetTransform()->SetRelativeRotY(_rot_y + _angle);
		}

	}
	void BarretAIMoveEnterState::Excute()
	{

	}
	void BarretAIMoveEnterState::Exit()
	{
	}
	void BarretHitState::Enter()
	{
		barret->animator->SetTrigger(L"Hit");
		barret->animator->SetBool(L"IsGround", false);
		barret->animator->SetInteger(L"Hit_Type", barret->hit_anim_idx);
		barret->animator->SetInteger(L"Hit_Dir", barret->hit_dir_idx);
	}
	void BarretHitState::Excute()
	{
	}
	void BarretHitState::Exit()
	{
	}
	void BarretItemState::Enter()
	{
		barret->animator->SetTrigger(L"Item_Param");
	}
	void BarretItemState::Excute()
	{
	}
	void BarretItemState::Exit()
	{
	}
}