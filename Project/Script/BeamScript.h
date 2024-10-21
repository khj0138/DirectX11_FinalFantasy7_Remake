#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BeamScript : public Script
	{
	public:
		BeamScript();
		~BeamScript();

		CLONE(BeamScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;

		void SetRot(vec3 _rot);
	private:
		vec3 rot;
		float time;
	};
}
