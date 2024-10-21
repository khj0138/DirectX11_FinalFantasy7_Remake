#pragma once

#include "AttackDataScript.h"

namespace ff7r
{
	class BladeScript : public AttackDataScript
	{
	public:
		BladeScript();
		~BladeScript();

		CLONE(BladeScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);

		void SetBladeIdx(int _idx);

	private:
		vec3	scale;

		float	time;
	};
}
