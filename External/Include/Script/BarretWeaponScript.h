#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class EffectScript;
	class BarretWeaponScript : public Script
	{
	public:
		enum class BARRET_WEAPON_STATE
		{
			SLEEP,
			NORMAL_ATTACK,
			UPPERCUT,
			SMACKDOWN,
			LIMIT_ATTACK,
			MAXIMUM_FURY,
			END
		};
		BarretWeaponScript();
		~BarretWeaponScript();

		CLONE(BarretWeaponScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		void Attack();
		void SoundOn(int _idx);
		void SoundOff(int _idx);

		GameObject* GetTarget() { return target; }
		void SetTarget(GameObject* _target) { target = _target; }
		void ChangeState(BARRET_WEAPON_STATE _next_state);
		vec3 CalcMuzzlePos();

		int test;
	private:
		vector<int> sound_buf;


		vector<class BarretWeaponState*> states;
		vector<UINT64> state_changes;

		vector<GameObject*> effect_objects;
		vector<GameObject*> attack_objects;

		GameObject* target;

		class StateMachine* state_machine;

		BARRET_WEAPON_STATE state;



		friend class BarretSleepState;
		friend class BarretNormalAtkState;
		friend class BarretUpperCutState;
		friend class BarretSmackDownState;
		friend class BarretLimitAtkState;
		friend class BarretMaximumFuryState;



	};
}
