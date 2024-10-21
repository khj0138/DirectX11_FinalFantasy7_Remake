#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class CWeaponFlashScript : public Script
	{
	public:
		CWeaponFlashScript();
		~CWeaponFlashScript();

		CLONE(CWeaponFlashScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		float time;
		float flash_time;
	};
}
