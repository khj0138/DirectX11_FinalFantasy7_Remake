#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class ElectricEffectScript : public Script
	{
	public:
		ElectricEffectScript();
		~ElectricEffectScript();

		CLONE(ElectricEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void Next();

	private:

	};
}
