#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class CWeaponHitEffectScript : public Script
	{
	public:
		CWeaponHitEffectScript();
		~CWeaponHitEffectScript();

		CLONE(CWeaponHitEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;

		void SetDir(vec3 _dir);

	private:
		float time;
	};
}
