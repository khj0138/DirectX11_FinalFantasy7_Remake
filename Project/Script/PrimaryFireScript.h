#pragma once

#include <Engine\Script.h>
#include "AttackDataScript.h"

namespace ff7r
{
	class PrimaryFireScript : public AttackDataScript
	{
	public:
		PrimaryFireScript();
		~PrimaryFireScript();

		CLONE(PrimaryFireScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos);

	private:
		float life_time;
	};
}

