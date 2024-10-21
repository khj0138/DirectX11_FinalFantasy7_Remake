#pragma once

#include "UIScript.h"

namespace ff7r
{
	class ImageUIScript : public UIScript
	{
	public:
		ImageUIScript();
		~ImageUIScript();

		CLONE(ImageUIScript);

		virtual void Awake() override;
		virtual void Tick() override;
		virtual void OnEnable() override;

		enum class ImageEffectType
		{
			ALPHA_CHANGE,
			NONE
		};

		void SetImageEffectType(ImageEffectType _type) { type = _type; }
		void SetTimeSpeed(float _speed) { time_speed = _speed; }
		void SetAlphaFactor(float _factor) { alpha_factor = _factor; }

	private:
		ImageEffectType type;

		float alpha_factor;
		float time_speed;
		float time;
	};
}
