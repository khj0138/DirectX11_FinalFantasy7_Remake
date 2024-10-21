#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BarretMaxFuryEffectScript : public Script
	{
	public:
		BarretMaxFuryEffectScript();
		~BarretMaxFuryEffectScript();

		CLONE(BarretMaxFuryEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
