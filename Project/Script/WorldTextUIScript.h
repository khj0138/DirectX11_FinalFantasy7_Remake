#pragma once

#include "UIScript.h"

namespace ff7r
{
	class WorldTextUIScript : public UIScript
	{
	public:
		WorldTextUIScript();
		~WorldTextUIScript();

		CLONE(WorldTextUIScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		void SetText(string _text, bool _is_text, bool _is_damage_text);

	private:
		void CalcPos();

		vec3 world_pos;
		vec3 world_scale;

		float scale_factor;
		float effect_time;
		float life_time;
		float t;

		bool is_effect;
		bool is_damage_text;
	};
}
