#include "pch.h"

#include <Engine\MeshRender.h>

#include "EffectScript.h"
namespace ff7r
{
	EffectScript::EffectScript()
		: Script((UINT)SCRIPT_TYPE::EFFECTSCRIPT)
		, renderer(nullptr)
		, uv_x_range(vec2(0.0f, 1.0f))
		, uv_y_range(vec2(0.0f, 1.0f))
		, uv_y_discard_range(vec2(0.0f, 0.0f))
		, max_uv_len(1.0f)
		, increase_speed(1.0f)
		, intensity(1.0f)
		, changing_speed(1.0f)
		, is_change_value(false)
		, is_change_range{}
		, is_disable(false)
		, time(0.0f)
		, disable_time(0.f)
	{
	}
	EffectScript::~EffectScript()
	{
	}

	void EffectScript::Awake()
	{
		renderer = GetOwner()->GetRenderComponent();

		if (is_change_range[0])
		{
			uv_x_range = vec2(0.0f, 0.0f);
		}
		if (is_change_range[1])
		{
			uv_y_range = vec2(0.0f, 0.0f);
		}
	}

	void EffectScript::Begin()
	{
	}
	void EffectScript::OnEnable()
	{
		if (is_change_range[0])
		{
			uv_x_range = vec2(0.0f, 0.0f);
		}
		if (is_change_range[1])
		{
			uv_y_range = vec2(0.0f, 0.0f);
		}
		time = 0.0f;
	}

	void EffectScript::OnDisable()
	{
	}

	void EffectScript::Tick()
	{
		ChangeUVRange();

		if (is_change_value)
			uv_changing_value.x += DT * changing_speed;
		renderer->GetMaterial()->SetScalarParam(VEC2_0, &uv_x_range);
		renderer->GetMaterial()->SetScalarParam(VEC2_1, &uv_y_range);
		renderer->GetMaterial()->SetScalarParam(VEC2_2, &uv_y_discard_range);
		renderer->GetMaterial()->SetScalarParam(VEC2_3, &uv_changing_value);
		renderer->GetMaterial()->SetScalarParam(FLOAT_0, &intensity);

		if (is_disable)
		{
			time += DT;

			if (time >= disable_time)
			{
				GetOwner()->SetActive(false);
			}
		}
	}

	void EffectScript::ChangeUVRange()
	{
		if (is_change_range[0])
		{
			uv_x_range.y += DT * increase_speed;

			if (uv_x_range.y - uv_x_range.x > max_uv_len)
			{
				uv_x_range.x = uv_x_range.y - max_uv_len;
			}
		}
		if (is_change_range[1])
		{
			uv_y_range.y += DT * increase_speed;

			if (uv_y_range.y - uv_y_range.x > max_uv_len)
			{
				uv_y_range.x = uv_y_range.y - max_uv_len;
			}
		}
	}
}