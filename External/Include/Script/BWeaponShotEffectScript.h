#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BWeaponShotEffectScript : public Script
	{
	public:
		BWeaponShotEffectScript();
		~BWeaponShotEffectScript();

		CLONE(BWeaponShotEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		void SetDir(vec3 _dir);

	private:
		float time;
	};
}
