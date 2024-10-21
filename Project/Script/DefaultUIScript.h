#pragma once

#include "UIScript.h"

namespace ff7r
{
	class DefaultUIScript : public UIScript
	{
	public:
		DefaultUIScript();
		~DefaultUIScript();

		CLONE(DefaultUIScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		void CreateChild();

		GameObject* n_attack_button;
		GameObject* command_ui;
	};
}

