#pragma once

#include <Engine\Script.h>
#include "CharacterDataScript.h"
#include "HundredGunnerRightArmScript.h"

namespace ff7r
{
	class HundredGunnerScript : public CharacterDataScript
	{
	public:
		HundredGunnerScript();
		~HundredGunnerScript();

		CLONE(HundredGunnerScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

		virtual void Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos = vec3(0.0f, 0.0f, 0.0f)) override;
	private:
		enum class HUNDREDGUNNER_STATE
		{
			IDLE,
			TURN,
			ATK_LASER,
			ATK_FIRE,
			ATK_SHOOT,
			ATK_BODY,
			ATK_BEAM,
			END
		};

		void ChangeState(HUNDREDGUNNER_STATE _next_state);
		void ChangeIdleState() { ChangeState(HUNDREDGUNNER_STATE::IDLE); }
		void ShootFire() { right_arm->Fire(); }
		void ReadyFire() { right_arm->ReadyFire(); }
		void ShootLaser() { shoot_laser = true; }
		void ShootMinigun() { shoot_minigun = true; }
		void ShootMinigunEnd() { shoot_minigun = false; }
		void ShootBeam();
		void DestroyPillar();

		vector<class HundredGunnerState*>	states;
		vector<class SaturationFireScript*> minigun;

		class Animator3D*					animator;
		class StateMachine*					state_machine;
		HundredGunnerRightArmScript*		right_arm;
		class BackWeaponScript*				back_weapon;

		HUNDREDGUNNER_STATE	state;

		GameObject* target;

		float	shoot_lazer_duration;
		float	time;
		float	next_state_time;
		int		overlap_wall_cnt;
		int		next;
		int		idx;
		int		show_idx;
		bool	shoot_minigun;
		bool	shoot_laser;
		bool	shoot_beam;
		bool	turn_end;
		bool	first_start;
		bool	is_dead;

		friend class HundredGunnerSleepState;
		friend class HundredGunnerTurnState;
		friend class HundredGunnerAttackLazerState;
		friend class HundredGunnerAttackFireState;
		friend class HundredGunnerAttackShootState;
		friend class HundredGunnerAttackBodyState;
		friend class HundredGunnerAttackBeamState;
	};
}

