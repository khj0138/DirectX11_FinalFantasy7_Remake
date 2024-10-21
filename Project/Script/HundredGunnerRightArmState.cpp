#include "pch.h"

#include <Engine\TimeMgr.h>
#include <Engine\SoundMgr.h>

#include "PrimaryFireScript.h"

#include "HundredGunnerRightArmState.h"
namespace ff7r
{
	void HundredGunnerRightArmSleepState::Enter()
	{
	}
	void HundredGunnerRightArmSleepState::Excute()
	{
	}
	void HundredGunnerRightArmSleepState::Exit()
	{
	}

	void HundredGunnerRightArmReadyState::Enter()
	{
		right_arm->muzzle_object->SetActive(true);
	}
	void HundredGunnerRightArmReadyState::Excute()
	{
	}
	void HundredGunnerRightArmReadyState::Exit()
	{
		right_arm->muzzle_object->SetActive(false);
	}

	void HundredGunnerRightArmFireState::Enter()
	{
		GameObject* fire = new GameObject;
		fire->AddComponent(new Transform);
		fire->AddComponent(new PrimaryFireScript);
		GameObject* socket = right_arm->muzzle_object->GetParent();
		SpawnGameObject(fire, socket->GetTransform()->GetWorldPos() + socket->GetTransform()->GetRelativeDir(DIR_TYPE::RIGHT) * -2.f, 5);
		time = 0.0f;
	}
	float _time = 1.0f;
	void HundredGunnerRightArmFireState::Excute()
	{
		time += DT;

		if (time >= _time)
		{
			right_arm->ChangeState(HundredGunnerRightArmScript::RIGHTARM_STATE::SLEEP);
		}
	}
	void HundredGunnerRightArmFireState::Exit()
	{
		//right_arm->primary_fire->SetActive(false);
	}
}