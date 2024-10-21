#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BarretNormalAtkEffectScript : public Script
	{
	public:
		BarretNormalAtkEffectScript();
		~BarretNormalAtkEffectScript();

		CLONE(BarretNormalAtkEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
