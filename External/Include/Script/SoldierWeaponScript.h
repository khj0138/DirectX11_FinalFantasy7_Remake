#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class EffectScript;
	class SoldierWeaponScript : public Script
	{
	public:
		enum class SOLDIER_WEAPON_STATE
		{
			SLEEP,
			NORMAL_ATTACK,
			END
		};
		SoldierWeaponScript();
		~SoldierWeaponScript();

		CLONE(SoldierWeaponScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		void Attack();
		void SoundOn(int _idx);
		void SoundOff(int _idx);

		GameObject* GetTarget() { return target; }
		void SetTarget(GameObject* _target) { target = _target; }
		void ChangeState(SOLDIER_WEAPON_STATE _next_state);
		vec3 CalcMuzzlePos();

		int test;
	private:
		vector<int> sound_buf;


		vector<class SoldierWeaponState*> states;

		GameObject* target;

		class StateMachine* state_machine;

		SOLDIER_WEAPON_STATE state;



		friend class SoldierSleepState;
		friend class SoldierNormalAtkState;



	};
}
