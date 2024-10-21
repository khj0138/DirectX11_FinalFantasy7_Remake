#include "pch.h"

#include <Engine\RenderMgr.h>
#include "WorldTextUIScript.h"
namespace ff7r
{
#define EFFECT_TIME 0.2f
#define DELAY_TIME 0.3f
#define LIFE_TIME 0.4f

	WorldTextUIScript::WorldTextUIScript()
		: UIScript(SCRIPT_TYPE::WORLDTEXTUISCRIPT)
		, t(0.0f)
		, effect_time(0.f)
		, is_damage_text(false)
		, is_effect(false)
		, life_time(0.2f)
		, scale_factor(1.0f)
	{
	}

	WorldTextUIScript::~WorldTextUIScript()
	{
	}

	void WorldTextUIScript::Awake()
	{
		int i = 1;
		float alpha_factor = 1.0f;
		UIScript::Awake();
		GetFontRenderer()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\English_Font.png"));
		GetFontRenderer()->GetMaterial()->SetScalarParam(INT_0, &i);
		GetFontRenderer()->GetMaterial()->SetScalarParam(FLOAT_0, &alpha_factor);

		world_pos = GetTransform()->GetRelativePos();
		world_scale = GetTransform()->GetRelativeScale();
		CalcPos();
		GetTransform()->FinalTick();
	}

	void WorldTextUIScript::Start()
	{

		//GetTransform()->FinalTick();
		//Matrix world_mat = GetTransform()->GetWorldMat();
		//// RenderMgr::GetInst()->GetMainCam();

		//Camera* main_cam = RenderMgr::GetInst()->GetEditorCam();

		//Matrix wvp = world_mat * main_cam->GetViewMat() * main_cam->GetProjMat();
		//
		//vec4 pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//vec4 clip_pos = XMVector4Transform(pos, wvp);

		//vec3 ndc_pos;
		//ndc_pos.x = clip_pos.x / clip_pos.w;
		//ndc_pos.y = clip_pos.y / clip_pos.w;
		//ndc_pos.z = clip_pos.z / clip_pos.w;

		//float screenX = (ndc_pos.x + 1.0f) * 0.5f * 1600.f;
		//float screenY = (1.0f - ndc_pos.y) * 0.5f * 900.f;
		//screenX -= 800.f;
		//screenY -= 450.f;
		//screenY *= -1.f;
		//GetTransform()->SetRelativePosXY(vec2(screenX, screenY));
	}

	void WorldTextUIScript::Tick()
	{
		if (is_effect)
		{ 
			effect_time += REAL_DT;


			if (effect_time < EFFECT_TIME / 2.f)
			{
				scale_factor = std::lerp(1.f, 1.5f, effect_time * (1.f / (EFFECT_TIME / 2.f)));
			}
			else
			{
				scale_factor = std::lerp(1.5f, 1.f, (effect_time - (EFFECT_TIME / 2.f)) * (1.f / (EFFECT_TIME / 2.f)));
			}

			if (effect_time > EFFECT_TIME)
			{
				scale_factor = 1.0f;

				is_effect = false;

				effect_time = DELAY_TIME;
			}
		}
		else if (is_damage_text)
		{
			if (effect_time > 0.f)
			{
				effect_time -= REAL_DT;
			}
			else
			{
				life_time -= REAL_DT;

				float _alpha_factor = life_time / LIFE_TIME;
				GetFontRenderer()->GetMaterial()->SetScalarParam(FLOAT_0, &_alpha_factor);
				t += REAL_DT * 120.f;

				if (life_time < 0.f)
				{
					DestroyObject(GetOwner());
				}
			}
		}



		CalcPos();
	}

	void WorldTextUIScript::SetText(string _text, bool _is_text, bool _is_damage_text)
	{
		is_damage_text = _is_damage_text;
		is_effect = _is_damage_text;
		GetFontRenderer()->SetTexts(_text, _is_text);

		if (!_is_text)
		{
			GetFontRenderer()->GetMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\Damage_Font.png"));
		}
	}
	void WorldTextUIScript::CalcPos()
	{
		GetTransform()->SetRelativePos(world_pos);
		GetTransform()->FinalTick();
		Matrix world_mat = GetTransform()->GetWorldMat();
		// RenderMgr::GetInst()->GetMainCam();

		Camera* main_cam = RenderMgr::GetInst()->GetEditorCam();

		Matrix wv = world_mat * main_cam->GetViewMat();
		Matrix wvp = wv * main_cam->GetProjMat();


		vec4 pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		vec4 clip_pos = XMVector4Transform(pos, wvp);

		if (clip_pos.z <= 0.f)
		{
			GetFontRenderer()->SetRender(false);
			return;
		}
		else
		{
			GetFontRenderer()->SetRender(true);
		}
		vec4 view_pos = XMVector4Transform(pos, wv);

		vec3 ndc_pos;
		ndc_pos.x = clip_pos.x / clip_pos.w;
		ndc_pos.y = clip_pos.y / clip_pos.w;
		ndc_pos.z = clip_pos.z / clip_pos.w;

		float _distance_scale;

		if (view_pos.z <= 0.0f)
		{
			_distance_scale = 0.0f;
		}
		else
		{
			view_pos.z = MIN(view_pos.z, 30.f);
			
			_distance_scale = lerp(1.5f, 0.5f, view_pos.z / 30.f);
			//_distance_scale = lerp(1.5f, 0.5f, ndc_pos.z);
		}

		float screenX = (ndc_pos.x + 1.0f) * 0.5f * 1600.f;
		float screenY = (1.0f - ndc_pos.y) * 0.5f * 900.f;
		screenX -= 800.f;
		screenY -= 450.f;
		screenY *= -1.f;

		GetTransform()->SetRelativePosXY(vec2(screenX, screenY + t));
		GetTransform()->SetRelativeScale(world_scale * _distance_scale * scale_factor);
	}
}