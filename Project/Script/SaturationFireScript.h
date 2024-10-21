#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class SaturationFireScript : public Script
	{
	public:
		SaturationFireScript();
		~SaturationFireScript();

		CLONE(SaturationFireScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		bool Shoot(vec3 _target_pos);

	private:
		enum class FIRE_STATE
		{
			SLEEP,
			FIRE,
			REST,
			END
		};

		FIRE_STATE	state;

		GameObject* muzzle_effect;

		float effect_time;
		float rest_time;
	};
}

