#pragma once

#include "UIScript.h"

namespace ff7r
{
	class ButtonEffectScript : public UIScript
	{
	public:
		ButtonEffectScript();
		~ButtonEffectScript();

		CLONE(ButtonEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetButtonData(float _x_length, float _time) { length = _x_length; highlight_time = _time; }

	private:
		float	length;
		float	highlight_time;
		float	cur_time;
	};
}
