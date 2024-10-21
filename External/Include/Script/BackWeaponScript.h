#pragma once

#include <Engine\Script.h>
namespace ff7r
{
	class BackWeaponScript : public Script
	{
	public:
		BackWeaponScript();
		~BackWeaponScript();

		CLONE(BackWeaponScript);

		virtual void Awake() override;
		virtual void Begin() override;
		virtual void Tick() override;

		void Shoot(int _index);

	private:
		vector<GameObject*> lazer_sockets;
	};
}

