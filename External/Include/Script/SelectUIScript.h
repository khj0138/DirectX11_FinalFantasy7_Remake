#pragma once

#include "UIScript.h"

namespace ff7r
{
	class TextUIScript;
	class ImageUIScript;
	class SelectUIScript : public UIScript
	{
	public:
		SelectUIScript();
		~SelectUIScript();

		CLONE(SelectUIScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetValid(bool _valid) { is_valid = _valid; }
		bool GetValid() { return is_valid; }

		GameObject* GetTarget() { return target; }
		string GetText() { return text; }
		
		int GetCost() { return cost; }

		void SetCost(int _cost) { cost = _cost; }
		void SetFocus(bool _is_focus);
		void SetText(string _text);
		void SetTarget(GameObject* _target, bool _is_child = false);

		//dddd
		void SetDamageText(string _text);

	private:
		GameObject*		target;
		TextUIScript*	text_ui;
		ImageUIScript*	child_ui;

		string			text;
		vec2			origin_scale;
		vec2			origin_pos;

		float			time;

		int				cost;

		bool			is_fade_effect;
		bool			is_focus;
		bool			is_valid;
	};
}
