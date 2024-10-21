#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class CWeaponSparkScript : public Script
	{
	public:
		CWeaponSparkScript();
		~CWeaponSparkScript();

		CLONE(CWeaponSparkScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetSpawnPosition(vec3 _pos, vec3 _dir);

	private:
		float time;
		bool is_first;
	};
}
