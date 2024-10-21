#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class FindEnemyScript : public Script
	{
	public:
		FindEnemyScript();
		~FindEnemyScript();

		CLONE(FindEnemyScript);

		virtual void Awake() override;
		virtual void Tick() override;

		GameObject* FindClosestEnemy();
		void FindChildEnemy(GameObject* _enemy, GameObject*& _close, float& _closest_distance);

		bool CheckEnemyDistance(GameObject* _enemy);

		void SetMaxDistance(float _distance) { max_distance = _distance; }

	private:
		float max_distance;
	};
}
