#pragma once

#include <Engine\\Script.h>
#include "AttackDataScript.h"
namespace ff7r
{
	class BarretLimitAtkScript : public AttackDataScript
	{
	public:
		BarretLimitAtkScript();
		~BarretLimitAtkScript();

		CLONE(BarretLimitAtkScript);
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;
		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;


		void SetVelocity(vec3 _vel) { velocity = _vel; }

		virtual void OnEnable() override;
		virtual void OnDisable() override;
		//virtual void OnEnable() override;


	private:
		float time;
		float max_scale;
		vec3 velocity;
		int boom_cnt;
	};
}



