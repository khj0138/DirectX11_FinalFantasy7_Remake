#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BarretUpperCutEffectScript : public Script
	{
	public:
		BarretUpperCutEffectScript();
		~BarretUpperCutEffectScript();

		CLONE(BarretUpperCutEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
