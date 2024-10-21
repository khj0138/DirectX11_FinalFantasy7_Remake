#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class MagicFireScript : public Script
	{
	public:
		MagicFireScript();
		~MagicFireScript();

		CLONE(MagicFireScript);

		virtual void Awake() override;
		virtual void Tick() override;

		void SetTargetPos(vec3 _pos);

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
			
	private:
		vec3	velocity;

		float	speed;
		float	life_time;
		float	trail_effect_time;
	};
}
