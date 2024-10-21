#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class ChangeLevelScript : public Script
	{
	public:
		ChangeLevelScript();
		~ChangeLevelScript();

		CLONE(ChangeLevelScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

	};
}
