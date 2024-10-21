#include "pch.h"

#include "CloudState.h"
#include "GameMgr.h"
#include "ObjectPoolMgr.h"
#include <Engine\SoundMgr.h>

namespace ff7r
{
	// ============================= Idle State =============================
	void CloudIdleState::Enter()
	{
	}
	void CloudIdleState::Excute()
	{

	}
	void CloudIdleState::Exit()
	{
	}

	// ============================= Move State =============================
	void CloudMoveEnterState::Enter() 
	{
		cloud->animator->SetBool(L"Move_Param", true);

		float camera_rot_y = cloud->GetPlayerCameraRotY();
		float player_rot_y = cloud->GetTransform()->GetRelativeRot().y;

		cloud->rot_y_idx = 0;
		if (KEY_INPUT(KEY::A))
		{
			camera_rot_y += -XM_PI / 2.f;
			cloud->rot_y_idx = 3;
		}
		else if (KEY_INPUT(KEY::S))
		{
			camera_rot_y += XM_PI;
			cloud->rot_y_idx = 2;
		}
		else if (KEY_INPUT(KEY::D))
		{
			camera_rot_y += XM_PI / 2.f;
			cloud->rot_y_idx = 1;
		}

		camera_rot_y = fmodf(camera_rot_y, XM_2PI);
		player_rot_y = fmodf(player_rot_y, XM_2PI);

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
				cloud->animator->SetRot(rotY / 90.f, Degree2Rad(90.f));
			}
			else
			{
				_turn_type = (int)TURN_TYPE::L90;
				cloud->animator->SetRot(rotY / 90.f, Degree2Rad(-90.f));
			}
			//_turn_type = rotY > 0 ? (int)TURN_TYPE::R90 : (int)TURN_TYPE::L90;
		}
		else if (fabs(rotY) <= 185.f)
		{
			if (rotY > 0)
			{
				_turn_type = (int)TURN_TYPE::R180;
				cloud->animator->SetRot(rotY / 180.f, Degree2Rad(180.f));
			}
			else
			{
				_turn_type = (int)TURN_TYPE::R180;
				cloud->animator->SetRot(rotY / 180.f, Degree2Rad(180.f));
			}
			//_turn_type = rotY > 0 ? (int)TURN_TYPE::R180 : (int)TURN_TYPE::R180;
		}
		else if (fabs(rotY) <= 355.f)
		{
			if (rotY > 0)
			{
				_turn_type = (int)TURN_TYPE::L90;
				cloud->animator->SetRot((360.f - rotY) / 90.f, Degree2Rad(-90.f));
			}
			else
			{
				_turn_type = (int)TURN_TYPE::R90;
				cloud->animator->SetRot((360.f + rotY) / 90.f, Degree2Rad(90.f));
			}
		}
		else
		{
			_turn_type = (int)TURN_TYPE::NONE;
		}

		cloud->animator->SetInteger(L"Run_Index", _turn_type);
	}
	void CloudMoveEnterState::Excute()
	{
	}
	void CloudMoveEnterState::Exit()
	{
		cloud->animator->SetRot(1.0f, 0.0f);
		cloud->GetTransform()->SetRelativeRotY(rot_y);
	}	

	void CloudMoveState::Enter()
	{
		sound_idx = SoundMgr::GetInst()->Play(L"sound\\Cloud\\AS\\Run.wav", SOUND_OBJECT_TYPE::_3D, cloud->GetOwner(), 0.6f,vec2(1.0f, 20.f),0 );
		prev_dir = (DIR_TYPE)(cloud->rot_y_idx * 2);
	}
	void CloudMoveState::Excute()
	{
		if (cloud->animator->GetParameter(L"Move_Param")->data.b == false || is_turn)
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
			if (cloud->player_camera)
			{
				float _cam_rot = cloud->GetPlayerCameraRotY();
				//float _cur_rot = cloud->GetTransform()->GetRelativeRot().y;
				float _rot_y = cloud->GetTransform()->GetRelativeRot().y;

				//cloud->GetTransform()->SetRelativeRotY(_cam_rot - 90.f * 3.141592 / 180.f + rot_y[(UINT)cur_dir]);

				if (abs(diff) == 1)
				{
					_rot_y = diff > 0 ? Degree2Rad(-45.f) : Degree2Rad(45.f);
				}
				else
				{
					_rot_y = diff < 0 ? Degree2Rad(-45.f) : Degree2Rad(45.f);
				}
				cloud->RotateCloud(_cam_rot + rot_y[(UINT)prev_dir], _rot_y);
			}
		}
		else if (abs(diff) == 0)
		{
			if (!cloud->is_rot_y && cur_dir != DIR_TYPE::END)
			{
				float _cam_rot = cloud->GetPlayerCameraRotY();
				cloud->GetTransform()->SetRelativeRotY(_cam_rot + rot_y[(UINT)cur_dir]);
				cloud->animator->CalcWorldDir();
			}
		}
		else if (abs(diff) == 4)
		{
			if (cur_dir != DIR_TYPE::END)
			{
				cloud->animator->SetBool(L"Move_Turn_Param", true);
				is_turn = true;
			}
		}

		prev_dir = cur_dir;

		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::LBTN))
		{
			cloud->n_attack_idx++;
			cloud->ChangeState(CloudMoveScript::CLOUD_STATE::ATTACK);
		}
		else if (!(INPUT_MOVE_KEY))
		{
			SoundMgr::GetInst()->Stop(sound_idx, cloud->GetOwner());
			sound_idx = -1;
			cloud->animator->SetBool(L"Move_Param", false);
		}
	}
	void CloudMoveState::Exit()
	{
		SoundMgr::GetInst()->Stop(sound_idx, cloud->GetOwner());
		sound_idx = -1;
		cloud->animator->SetBool(L"Move_Param", false);
	}

	// ============================= Attack State =============================

	void CloudAttackState::Enter()
	{
		cloud->animator->SetTrigger(L"Attack");

		int _atk_param = 0;

		if (cloud->target)
		{
			float _distance = fabs(vec3(cloud->GetTransform()->GetWorldPos() - cloud->target->GetCollider3D()->GetWorldPos()).Length());
			if (_distance < 4.25f)
			{
				_atk_param = 0;
			}
			else if (_distance < 8.5f)
			{
				_atk_param = 1;
			}
			else if (_distance < 11.f)
			{
				_atk_param = 2;
			}
		}

		cloud->animator->SetInteger(L"Attack_Param", _atk_param);
		//cloud->SetWeaponType(CloudWeaponScript::WEAPON_TYPE::ATK0);
	}
	void CloudAttackState::Excute()
	{
		if (KEY_TAP(KEY::LBTN))
		{
			wstring _cur_state_name = cloud->animator->GetCurState()->GetName();
			
			if (_cur_state_name != attack_name)
			{
				attack_name = _cur_state_name;
				cloud->animator->SetInteger(L"N_Attack_Index_Param", cloud->n_attack_idx++);
			}
		}

		cloud->RotateCloudToTarget();

		//if (KEY_TAP(KEY::RBTN))
		//{
		//	cloud->ChangeState(CloudMoveScript::CLOUD_STATE::AVOID);
		//}
	}
	void CloudAttackState::Exit()
	{

		attack_name.clear();
		cloud->n_attack_idx = 0;
		cloud->animator->SetInteger(L"N_Attack_Index_Param", cloud->n_attack_idx);
	}

	// ============================= Hit State =============================

	void CloudHitState::Enter()
	{
		cloud->animator->SetTrigger(L"Hit");
		cloud->animator->SetBool(L"IsGround",false);
		cloud->animator->SetInteger(L"Hit_Type", cloud->hit_anim_idx);
		cloud->animator->SetInteger(L"Hit_Dir", cloud->hit_dir_idx);
	}
	void CloudHitState::Excute()
	{
	}
	void CloudHitState::Exit()
	{
	}

	// ============================= Avoid State =============================

	void CloudAvoidState::Enter()
	{
		cloud->animator->SetTrigger(L"Avoid_Param");
	}
	void CloudAvoidState::Excute()
	{
	}
	void CloudAvoidState::Exit()
	{
	}

	// ============================= Ability State =============================
	void CloudAbilityState::Enter()
	{
		cloud->RotateCloudToTarget();
		cloud->animator->SetTrigger(L"Ability");
	}
	void CloudAbilityState::Excute()
	{
	}
	void CloudAbilityState::Exit()
	{
	}

	// ============================= Magic State =============================
	void CloudMagicState::Enter()
	{
		cloud->RotateCloudToTarget();
		cloud->animator->SetTrigger(L"Magic");
	}
	void CloudMagicState::Excute()
	{
	}
	void CloudMagicState::Exit()
	{
	}

	// ============================= Limit State =============================
	void CloudLimitState::Enter()
	{
		cloud->RotateCloudToTarget();
		cloud->limit0_cam_idx = -1;
		cloud->animator->SetTrigger(L"Limit");
		cloud->animator->SetInteger(L"Limit_Param", 1);

	}
	void CloudLimitState::Excute()
	{
	}
	void CloudLimitState::Exit()
	{
		cloud->animator->SetInteger(L"Limit_Param", 0);
	}



	void CloudAIAttackState::Enter()
	{
		//return;
		cloud->animator->SetTrigger(L"Attack");

		int _atk_param = 0;

		if (cloud->target)
		{
			float _distance = fabs(vec3(cloud->GetTransform()->GetWorldPos() - cloud->target->GetCollider3D()->GetWorldPos()).Length());
			if (_distance < 4.25f)
			{
				_atk_param = 0;
			}
			else if (_distance < 8.5f)
			{
				_atk_param = 1;
			}
			else if (_distance < 11.f)
			{
				_atk_param = 2;
			}
		}

		cloud->animator->SetInteger(L"Attack_Param", _atk_param);
		cloud->RotateCloudToTarget();
	}

	void CloudAIAttackState::Excute()
	{
		cloud->animator->SetInteger(L"N_Attack_Index_Param",4);
		cloud->RotateCloudToTarget();
	}

	void CloudAIAttackState::Exit()
	{
		attack_name.clear();
		cloud->n_attack_idx = 0;
		cloud->animator->SetInteger(L"N_Attack_Index_Param", cloud->n_attack_idx);
	}

	void CloudAIMoveEnterState::Enter()
	{
		if (GameMgr::GetInst()->IsControl(cloud->GetOwner()))
		{
			cloud->animator->SetBool(L"Move_Param", false);
			cloud->ChangeState(CloudMoveScript::CLOUD_STATE::IDLE);
			return;
		}
		cloud->animator->SetBool(L"Move_Param", true);
		cloud->animator->CalcWorldDir();
		vec3 _front_dir = cloud->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		vec3 _target_dir = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos() - cloud->GetTransform()->GetWorldPos();
		_target_dir.y = 0.0f;
		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);
		_angle = fmodf(_angle, XM_2PI);
		if (_target_dir.Length() < 10.0f)
		{
			cloud->animator->SetBool(L"Move_Param", false);
			cloud->ChangeState(CloudMoveScript::CLOUD_STATE::IDLE);
			return;
		}
		else
		{
			cloud->ChangeState(CloudMoveScript::CLOUD_STATE::AI_MOVE);
			float _rot_y = cloud->GetTransform()->GetRelativeRot().y;
			cloud->GetTransform()->SetRelativeRotY(_rot_y + _angle);
		}

		
	}

	void CloudAIMoveEnterState::Excute()
	{
	}

	void CloudAIMoveEnterState::Exit()
	{

	}

	void CloudAIMoveState::Enter()
	{

	}

	void CloudAIMoveState::Excute()
	{
		if (GameMgr::GetInst()->IsControl(cloud->GetOwner()))
		{
			cloud->animator->SetBool(L"Move_Param", false);
			return;
		}
		cloud->animator->SetBool(L"Move_Param", true);
		cloud->animator->CalcWorldDir();
		vec3 _front_dir = cloud->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		vec3 _target_dir = GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos() - cloud->GetTransform()->GetWorldPos();
		_target_dir.y = 0.0f;
		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);
		_angle = fmodf(_angle, XM_2PI);



		

		if (_target_dir.Length() < 10.0f)
		{
			cloud->animator->SetBool(L"Move_Param", false);
			return;
		}
		else
		{
			float _rot_y = cloud->GetTransform()->GetRelativeRot().y;
			cloud->GetTransform()->SetRelativeRotY(_rot_y + _angle);
		}
	}

	void CloudAIMoveState::Exit()
	{
	}

	void CloudItemState::Enter()
	{
		cloud->animator->SetTrigger(L"Item_Param");
		ObjectPoolMgr::GetInst()->GetGameObject(L"Heal Particel Effect")->GetTransform()->SetRelativePos(cloud->GetTransform()->GetRelativePos());
	}

	void CloudItemState::Excute()
	{
	}

	void CloudItemState::Exit()
	{
	}

}