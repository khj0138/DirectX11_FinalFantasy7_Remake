#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BWeaponLimitEffectScript : public Script
	{
	public:
		BWeaponLimitEffectScript();
		~BWeaponLimitEffectScript();

		CLONE(BWeaponLimitEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;

		void SetDir(vec3 _dir);

	private:
		float time;
		bool is_init;
	};
}
