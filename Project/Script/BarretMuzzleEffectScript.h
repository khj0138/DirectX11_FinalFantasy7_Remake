#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BarretMuzzleEffectScript : public Script
	{
	public:
		BarretMuzzleEffectScript();
		~BarretMuzzleEffectScript();

		CLONE(BarretMuzzleEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
