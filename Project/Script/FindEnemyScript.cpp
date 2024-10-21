#include "pch.h"

#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>

#include "PlayerDataScript.h"

#include "FindEnemyScript.h"
namespace ff7r
{
	FindEnemyScript::FindEnemyScript()
		: Script((UINT)SCRIPT_TYPE::FINDENEMYSCRIPT)
		, max_distance(100.f)
	{
	}

	FindEnemyScript::~FindEnemyScript()
	{
	}

	void FindEnemyScript::Awake()
	{
	}

	void FindEnemyScript::Tick()
	{
	}


	GameObject* FindEnemyScript::FindClosestEnemy()
	{
		
		//return nullptr;
		Level* _level = LevelMgr::GetInst()->GetCurLevel();
		Layer* _layer = _level->FindLayerByName(L"Enemy");

		GameObject* _closest_enemy = nullptr;

		float _closest_distance = max_distance;
		
		for (GameObject* enemy : _layer->GetParentObject())
		{
			if (!enemy->GetActive() || !enemy->GetCollider3D() || !enemy->GetCollider3D()->GetCollision())
			{
				continue;
			}

			vec3 _enemy_pos = enemy->GetTransform()->GetWorldPos();
			vec3 _pos = GetTransform()->GetWorldPos();
			_enemy_pos.y = 0.0f;
			_pos.y = 0.0f;
			//float _distance = vec3::Distance(GetTransform()->GetWorldPos(), enemy->GetTransform()->GetWorldPos());
			float _distance = vec3::Distance(_pos ,_enemy_pos);

			if (_distance < _closest_distance)
			{
				_closest_enemy = enemy;
				_closest_distance = _distance;
			}

			for (GameObject* _obj : enemy->GetChild())
			{
				FindChildEnemy(_obj, _closest_enemy, _closest_distance);
			}
		}

		GetOwner()->GetScript<PlayerDataScript>()->SetTarget(_closest_enemy);
		return _closest_enemy;
	}

	void FindEnemyScript::FindChildEnemy(GameObject* _enemy, GameObject*& _close, float& _closest_distance)
	{
		if (!_enemy->GetActive())
		{
			return;
		}

		float _distance = vec3::Distance(GetTransform()->GetWorldPos(), _enemy->GetTransform()->GetWorldPos());

		if (_enemy->GetCollider3D() && _enemy->GetCollider3D()->GetCollision() && _distance < _closest_distance)
		{
			_close = _enemy;
			_closest_distance = _distance;
		}

		for (GameObject* _obj : _enemy->GetChild())
		{
			FindChildEnemy(_obj, _close, _closest_distance);
		}
	}

	bool FindEnemyScript::CheckEnemyDistance(GameObject* _enemy)
	{
		if (_enemy == nullptr)
			return false;
		float _distance = vec3::Distance(GetTransform()->GetWorldPos(), _enemy->GetTransform()->GetWorldPos());

		if (_distance > max_distance)
		{
			return false;
		}

		return true;
	}
}