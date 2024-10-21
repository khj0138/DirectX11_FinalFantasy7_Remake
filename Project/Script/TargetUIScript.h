#pragma once

#include "UIScript.h"

namespace ff7r
{
	class TextUIScript;
	class TargetUIScript : public UIScript
	{
	public:
		TargetUIScript();
		~TargetUIScript();

		CLONE(TargetUIScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetTarget(GameObject* _target);

	private:
		void CreateChild();
		void CalcUIPos();

		vector<GameObject*> sticks;
		vector<vec2>		stick_pos;

		GameObject*			target;
		GameObject*			hp_bar;
		TextUIScript*		name_text;

		float time;
		float time_coeffi;

		bool enable_action;
	};
}
