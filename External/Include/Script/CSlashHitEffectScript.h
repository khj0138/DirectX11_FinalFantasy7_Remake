#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class CSlashHitEffectScript : public Script
	{
	public:
		CSlashHitEffectScript();
		~CSlashHitEffectScript();

		CLONE(CSlashHitEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

	private:
		float time;
		vec2 scale;
	};
}
