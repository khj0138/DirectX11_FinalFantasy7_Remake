#include "pch.h"

#include <Engine\SoundMgr.h>

#include "SaturationFireScript.h"
#include "BackWeaponScript.h"
#include "HundredGunnerRightArmScript.h"

#include "HundredGunnerState.h"

namespace ff7r
{
	void HundredGunnerSleepState::Enter()
	{
	}
	void HundredGunnerSleepState::Excute()
	{
		if (hunredgunner->target)
		{
			if (hunredgunner->first_start)
			{
				vec3 _front_dir = hunredgunner->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
				vec3 _target_dir = hunredgunner->target->GetTransform()->GetWorldPos() - hunredgunner->GetTransform()->GetWorldPos();
				float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

				_angle = fmodf(_angle, XM_2PI);
				//_angle = Rad2Degree(_angle);

				int _turn_type = 0;

				float _rot_y = hunredgunner->GetTransform()->GetRelativeRot().y;

				if (_rot_y)
				{
					hunredgunner->GetTransform()->SetRelativeRotY(_rot_y);
				}

			}
		}

	}
	void HundredGunnerSleepState::Exit()
	{
	}

	void HundredGunnerTurnState::Enter()
	{
		vec3 _front_dir = hunredgunner->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 _target_dir = hunredgunner->target->GetTransform()->GetWorldPos() - hunredgunner->GetTransform()->GetWorldPos();

		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

		_angle = Rad2Degree(_angle);

		int _turn_type = 0;

		if (fabs(_angle) <= 91.f)
		{
			_turn_type = _angle > 0 ? (int)TURN_TYPE::R90 : (int)TURN_TYPE::L90;
		}
		else
		{
			_turn_type = _angle > 0 ? (int)TURN_TYPE::R180 : (int)TURN_TYPE::L180;
		}

		hunredgunner->animator->SetTrigger(L"Turn");
		hunredgunner->animator->SetInteger(L"Turn_Index", _turn_type);
	}
	void HundredGunnerTurnState::Excute()
	{
		vec3 _front_dir = hunredgunner->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 _target_dir = hunredgunner->target->GetTransform()->GetWorldPos() - hunredgunner->GetTransform()->GetWorldPos();

		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

		_angle = Rad2Degree(_angle);

		if (fabs(_angle) <= 1.0f)
		{
			hunredgunner->ChangeState(HundredGunnerScript::HUNDREDGUNNER_STATE::IDLE);
		}
	}
	void HundredGunnerTurnState::Exit()
	{
		hunredgunner->animator->SetInteger(L"Turn_Index", (int)TURN_TYPE::END);
		hunredgunner->turn_end = true;
	}

	void HundredGunnerAttackLazerState::Enter()
	{
		hunredgunner->animator->SetTrigger(L"ATK_Laser");
		lazer_time = 0.0f;
		lazer_index = 0;
	}
	void HundredGunnerAttackLazerState::Excute()
	{
		if (hunredgunner->shoot_laser)
		{
			lazer_time += DT;
			float _duration = hunredgunner->shoot_lazer_duration;
			if (lazer_time >= _duration)
			{
				lazer_time -= _duration;

				hunredgunner->back_weapon->Shoot(lazer_index);
				lazer_index++;
			}

			if (lazer_index > 13)
			{
				hunredgunner->animator->SetTrigger(L"ATK_Laser_End");
				hunredgunner->shoot_laser = false;
			}
		}
	}
	void HundredGunnerAttackLazerState::Exit()
	{
	}

	void HundredGunnerAttackFireState::Enter()
	{
		hunredgunner->animator->SetTrigger(L"ATK_Fire");
	}
	void HundredGunnerAttackFireState::Excute()
	{

	}
	void HundredGunnerAttackFireState::Exit()
	{
	}

	void HundredGunnerAttackShootState::Enter()
	{
		hunredgunner->animator->SetTrigger(L"ATK_Shoot");
		bfirst = true;
	}
	void HundredGunnerAttackShootState::Excute()
	{
		vec3 _front_dir = hunredgunner->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 _target_dir = hunredgunner->target->GetTransform()->GetWorldPos() - hunredgunner->GetTransform()->GetWorldPos();

		float _angle = CalcYAxisFromVector(_front_dir, _target_dir);

		vec3 _rot = hunredgunner->GetTransform()->GetRelativeRot();
		hunredgunner->GetTransform()->SetRelativeRotY(_rot.y + _angle);

		if (hunredgunner->shoot_minigun)
		{
			if (bfirst)
			{
				SoundMgr::GetInst()->Play(L"sound\\HundredGunner\\fire.mp3", SOUND_OBJECT_TYPE::_3D, hunredgunner->GetOwner(), 0.35f);
				bfirst = false;
			}

			int cnt = hunredgunner->minigun.size();
			for (int i = 0; i < cnt; i++)
			{
				if (hunredgunner->minigun[i]->Shoot(hunredgunner->target->GetTransform()->GetWorldPos()))
				{
					break;
				}
			}
		}
	}
	void HundredGunnerAttackShootState::Exit()
	{
	}

	void HundredGunnerAttackBodyState::Enter()
	{
		hunredgunner->animator->SetTrigger(L"ATK_Body");
	}
	void HundredGunnerAttackBodyState::Excute()
	{
	}
	void HundredGunnerAttackBodyState::Exit()
	{
	}

	void HundredGunnerAttackBeamState::Enter()
	{
		hunredgunner->animator->SetTrigger(L"ATK_Beam");
	}

	void HundredGunnerAttackBeamState::Excute()
	{
	}

	void HundredGunnerAttackBeamState::Exit()
	{
	}

}