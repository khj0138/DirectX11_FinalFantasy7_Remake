#pragma once

#include "AttackDataScript.h"

namespace ff7r
{
	class HundredBulletScript : public AttackDataScript
	{
	public:
		HundredBulletScript();
		~HundredBulletScript();

		CLONE(HundredBulletScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;
		
		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);

		virtual void OnEnable() override;

		void SetTargetPos(vec3 pos);

	private:
		vec3	dir;

		float	speed;
		float	active_time;
	};
}

