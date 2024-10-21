#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class HealEffectScript : public Script
	{
	public:
		HealEffectScript();
		~HealEffectScript();

		CLONE(HealEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;

	private:
		class EffectScript* effect;
		float time;

	};
}
