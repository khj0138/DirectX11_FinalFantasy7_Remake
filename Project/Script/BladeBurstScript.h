#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BladeBurstScript : public Script
	{
	public:
		BladeBurstScript();
		~BladeBurstScript();

		CLONE(BladeBurstScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		float	time;
		float	blade_duration;

		int		blade_idx;
	};
}
