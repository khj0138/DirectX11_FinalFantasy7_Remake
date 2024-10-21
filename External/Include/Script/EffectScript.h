#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class EffectScript : public Script
	{
	public:
		EffectScript();
		~EffectScript();

		CLONE(EffectScript);

		virtual void Awake() override;
		virtual void Begin() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual void Tick() override;

		void SetDisableEffect(float _time) { disable_time = _time; is_disable = true; }
		void SetIntensity(float _intensity) { intensity = _intensity; }
		void SetRangeX(vec2 _range_x) { uv_x_range = _range_x; }
		void SetRangeY(vec2 _range_y) { uv_y_range = _range_y; }
		void SetDiscardRangeY(vec2 _range_y) { uv_y_discard_range = _range_y; }
		void SetIncreaseSpeed(float _speed) { increase_speed = _speed; }
		void SetMaxUVLength(float _len) { max_uv_len = _len; }
		void IsChangeRange(bool _is_x, bool _is_y) { is_change_range[0] = _is_x; is_change_range[1] = _is_y; }

		void SetChange(bool _change) { is_change_value = _change; }
	private:
		void ChangeUVRange();

		class RenderComponent* renderer;

		vec2	uv_x_range;
		vec2	uv_y_range;
		vec2	uv_y_discard_range;
		vec2	uv_changing_value;


		float	max_uv_len;
		float	increase_speed;
		float	intensity;
		float	disable_time;
		float	time;
		float	changing_speed;

		bool	is_disable;
		bool	is_change_range[2];
		bool	is_change_value;


	};
}

