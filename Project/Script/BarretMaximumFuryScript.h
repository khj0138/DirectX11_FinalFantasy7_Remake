#pragma once

#include <Engine\\Script.h>
#include "AttackDataScript.h"

namespace ff7r
{
	class BarretMaximumFuryScript : public AttackDataScript
	{
	public:
		BarretMaximumFuryScript();
		~BarretMaximumFuryScript();

		CLONE(BarretMaximumFuryScript);
		virtual void Awake() override;
		virtual void Tick() override;
		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetBoom(bool _boom) { boom = _boom; }


		//virtual void OnEnable() override;


	private:
		float time;
		bool boom;

	};
}



