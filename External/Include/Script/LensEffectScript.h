#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class LensEffectScript : public Script
	{
	public:
		LensEffectScript();
		~LensEffectScript();

		CLONE(LensEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

	private:

	};
}

