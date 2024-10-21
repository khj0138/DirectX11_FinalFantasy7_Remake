#pragma once

#include "UIScript.h"

namespace ff7r
{
	class CharacterDataScript;
	class ImageUIScript;
	class FillUIScript : public UIScript
	{
	public:
		FillUIScript();
		~FillUIScript();

		CLONE(FillUIScript);

		enum class HUD_TYPE
		{
			HP,
			MP,
			LIMIT,
			ATB
		};

		virtual void Awake() override;
		virtual void Tick() override;

		void SetTarget(CharacterDataScript* _target) { target = _target; }
		void SetHUDType(HUD_TYPE _type) { type = _type; }
		void SetFillColor(vec4 _color);
		void SetFillAmount(float _amount);
		void SetFillRect(vec2 _fill_lt, vec2 _fill_rb, vec2 _image_left_top, vec2 _size);

	private:
		void InitSetting();

		CharacterDataScript* target;
		
		vector<ImageUIScript*> highlight;

		HUD_TYPE type;
	};
}

