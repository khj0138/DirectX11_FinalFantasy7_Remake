#pragma once

#include "UIScript.h"

namespace ff7r
{
	class FillUIScript;
	class TextUIScript;
	class CharacterUIScript : public UIScript
	{
	public:
		CharacterUIScript();
		~CharacterUIScript();

		CLONE(CharacterUIScript);

		virtual void Awake() override;
		virtual void Tick() override;

		void SetCharacter(GameObject* _character) { target = _character; }

	private:
		void CreateChild();
		void CreateFillUI();
		void CreateTextUI();

		GameObject* target;

		FillUIScript* hp;
		FillUIScript* mp;
		FillUIScript* limit;
		FillUIScript* atb;

		TextUIScript* name;
		TextUIScript* hp_text;
		TextUIScript* mp_text;
		TextUIScript* atb_text;
		TextUIScript* limit_text;
		TextUIScript* max_hp_text;
		TextUIScript* cur_hp_text;
		TextUIScript* cur_mp_text;
	};
}
