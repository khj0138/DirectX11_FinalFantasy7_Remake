#pragma once

#include "AttackDataScript.h"

namespace ff7r
{
	class HomingLaserScript : public AttackDataScript
	{
	public:
		HomingLaserScript();
		~HomingLaserScript();

		CLONE(HomingLaserScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);

		void Shoot() { ChangeState(HOMINGLASER_STATE::LAUNCH); }
		void SetDir(vec3 _dir);
		void Move(vec3 _velocity);
		void MoveCurve(vec3 _origin, float _time);

	private:
		enum class HOMINGLASER_STATE
		{
			SLEEP,
			LAUNCH,
			SHOOT,
			END
		};

		void ChangeState(HOMINGLASER_STATE _next_state);

		vector<class HomingLaserState*> states;
		class StateMachine*				state_machine;

		HOMINGLASER_STATE	state;

		vec3				dir;

		float				luanch_time;
		float				luanch_speed;
		float				shoot_speed;
		float				life_time;

		friend class HomingLaserSleepState;
		friend class HomingLaserLaunchState;
		friend class HomingLaserShootState;
	};
}

