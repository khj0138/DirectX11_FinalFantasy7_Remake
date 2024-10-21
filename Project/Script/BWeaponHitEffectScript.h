#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BWeaponHitEffectScript : public Script
	{
	public:
		BWeaponHitEffectScript();
		~BWeaponHitEffectScript();

		CLONE(BWeaponHitEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		void SetDir(vec3 _dir);

	private:
		float time;
		float duration;
	};
}
