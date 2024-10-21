#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BreakEffectScript : public Script
	{
	public:
		BreakEffectScript();
		~BreakEffectScript();

		CLONE(BreakEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		vector<GameObject*> effects;

		float time;
		bool b;
	};
}
