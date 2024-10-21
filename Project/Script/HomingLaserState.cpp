#include "pch.h"

#include <Engine\TimeMgr.h>
#include "GameMgr.h"
#include "HomingLaserState.h"
namespace ff7r
{
	void HomingLaserSleepState::Enter()
	{
	}
	void HomingLaserSleepState::Excute()
	{
	}
	void HomingLaserSleepState::Exit()
	{
	}

	void HomingLaserLaunchState::Enter()
	{
		time = 0.0f;
	}
	void HomingLaserLaunchState::Excute()
	{
		time += DT;

		if (time >= homing_laser->luanch_time)
		{
			homing_laser->ChangeState(HomingLaserScript::HOMINGLASER_STATE::SHOOT);
		}
		else
		{
			homing_laser->Move(homing_laser->luanch_speed * homing_laser->dir);
		}
	}
	void HomingLaserLaunchState::Exit()
	{
	}

	void HomingLaserShootState::Enter()
	{
		time = 0.f;
		orgin = homing_laser->dir;
		//homing_laser->dir = (GameMgr::GetInst()->GetCurPlayer()->GetTransform()->GetWorldPos()  -homing_laser->GetTransform()->GetWorldPos()).Normalize();
	}
	void HomingLaserShootState::Excute()
	{
		time += DT;
		if (time >= 1.0f)
		{
			time = 1.0f;
		}

		homing_laser->dir = vec3::Lerp(orgin, (GameMgr::GetInst()->GetCurPlayer()->GetCollider3D()->GetWorldPos() - homing_laser->GetTransform()->GetWorldPos()).Normalize(), time / 1.0f);
		homing_laser->Move(homing_laser->shoot_speed * homing_laser->dir);
	}
	void HomingLaserShootState::Exit()
	{
	}
}