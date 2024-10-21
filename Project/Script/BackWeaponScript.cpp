#include "pch.h"

#include <Engine\Socket.h>
#include <Engine\KeyMgr.h>
#include <Engine\ParticleSystem.h>
#include <Engine\Transform.h>

#include "ObjectPoolMgr.h"
#include "HomingLaserScript.h"

#include "BackWeaponScript.h"
namespace ff7r
{
	BackWeaponScript::BackWeaponScript()
		: Script(SCRIPT_TYPE::BACKWEAPONSCRIPT)
	{
	}

	BackWeaponScript::~BackWeaponScript()
	{
	}

	void BackWeaponScript::Awake()
	{
		GetMeshRender()->SetFrustumCheck(false);

		const vector<GameObject*>& child = GetOwner()->GetChild();

		for (auto& obj : child)
		{
			if (obj->GetSocket())
			{
				lazer_sockets.push_back(obj);
			}
		}
	}

	void BackWeaponScript::Begin()
	{
	}

	void BackWeaponScript::Tick()
	{
	}
	void BackWeaponScript::Shoot(int _index)
	{
		if (_index >= lazer_sockets.size())
		{
			_index -= lazer_sockets.size();
		}

		vec3 _pos = GetTransform()->GetWorldPos();
		vec3 _dir = lazer_sockets[_index]->GetTransform()->GetWorldDir(DIR_TYPE::UP);

		GameObject* _lazer = ObjectPoolMgr::GetInst()->GetGameObject(L"Hundred Homing Laser");
		_lazer->GetTransform()->SetRelativePos(_pos);
		_lazer->GetScript<HomingLaserScript>()->SetDir(_dir.Normalize());
		_lazer->GetScript<HomingLaserScript>()->Shoot();
		_lazer->GetTransform()->FinalTick();
	}
}