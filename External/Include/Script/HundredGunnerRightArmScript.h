#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class HundredGunnerRightArmScript : public Script
	{
	public:
		HundredGunnerRightArmScript();
		~HundredGunnerRightArmScript();

		CLONE(HundredGunnerRightArmScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		void ReadyFire() { ChangeState(RIGHTARM_STATE::READY); }
		void Fire() { ChangeState(RIGHTARM_STATE::FIRE); }

	private:
		void CreateFireObject();

		enum class RIGHTARM_STATE
		{
			SLEEP,
			READY,
			FIRE,
			END
		};
		void ChangeState(RIGHTARM_STATE _next_state);

		vector<class HundredGunnerRightArmState*> states;
		class StateMachine* state_machine;

		GameObject* muzzle_object;
		GameObject* primary_fire;

		RIGHTARM_STATE state;

		float ready_time;

		friend class HundredGunnerRightArmSleepState;
		friend class HundredGunnerRightArmReadyState;
		friend class HundredGunnerRightArmFireState;
	};
}

