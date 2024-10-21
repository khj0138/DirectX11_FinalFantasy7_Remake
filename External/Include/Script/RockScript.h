#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class RockScript : public Script
	{
	public:
		RockScript();
		~RockScript();

		CLONE(RockScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;
		virtual void OnEnable() override;

	private:
		float time;
		bool first_tick;
	};
}
