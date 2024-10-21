#pragma once

#include <Engine\\Script.h>
#include "AttackDataScript.h"

namespace ff7r
{
	class BarretUpperCutScript : public AttackDataScript
	{
	public:
		BarretUpperCutScript();
		~BarretUpperCutScript();

		CLONE(BarretUpperCutScript);
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;
		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;


		virtual void OnEnable() override;
		virtual void OnDisable() override;

		//virtual void OnEnable() override;


	private:
		float _time;
		bool charging;
	};
}



