#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BarretSmackDownEffectScript : public Script
	{
	public:
		BarretSmackDownEffectScript();
		~BarretSmackDownEffectScript();

		CLONE(BarretSmackDownEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
