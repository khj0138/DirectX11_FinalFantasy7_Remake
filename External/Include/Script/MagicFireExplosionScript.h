#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class MagicFireExplosionScript : public Script
	{
	public:
		MagicFireExplosionScript();
		~MagicFireExplosionScript();
		
		CLONE(MagicFireExplosionScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
