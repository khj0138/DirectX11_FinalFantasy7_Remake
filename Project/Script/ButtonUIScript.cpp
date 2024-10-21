#include "pch.h"
#include "ButtonUIScript.h"
#include "ButtonEffectScript.h"
#include "UIMgr.h"

namespace ff7r
{
	ButtonUIScript::ButtonUIScript()
		: UIScript((UINT)SCRIPT_TYPE::BUTTONUISCRIPT)
		, select_key(KEY::END)
		, highlight_time(0.25f)
		, tap_offset_pos(-10.f, 0.0f)
	{
		SetEnableTime(1.0f);
	}

	ButtonUIScript::~ButtonUIScript()
	{
	}

	void ButtonUIScript::Awake()
	{
		GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));

		SetSize(vec2(343.f, 40.f), vec2(1.0f, 1.0f));
		SetLeftTop(vec2(561.f, 1160.f));

		origin_pos = GetTransform()->GetRelativePosXY();

		effect = new GameObject;
		effect->AddComponent(new Transform);
		effect->AddComponent(new MeshRender);
		effect->AddComponent(new ButtonEffectScript);
		
		effect->SetLayerIndex(31);
		effect->SetActive(false);

		ButtonEffectScript* _eff_script = effect->GetScript<ButtonEffectScript>();
		_eff_script->SetButtonData(313.f, highlight_time);

		GetOwner()->AddChild(effect);

		UIScript::Awake();
	}

	void ButtonUIScript::Tick()
	{
		if ((select_key != KEY::END) && KEY_TAP(select_key))
		{
			if (!is_tap)
			{
				TapButton();
			}
		}

		if (is_tap)
		{
			cur_time += REAL_DT;

			if (cur_time >= highlight_time)
			{
				is_tap = false;
				ChangeButtonTexture(is_tap);
				cur_time = 0.0f;
				// GetTransform()->SetRelativePosXY(origin_pos);
			}
			//else
			//{
			//	float _half_time = highlight_time / 2.f;

			//	vec2 _offset = cur_time <= _half_time ? vec2::Lerp(origin_pos, origin_pos + tap_offset_pos, cur_time / 2.0f)
			//		: vec2::Lerp(origin_pos + tap_offset_pos, origin_pos, cur_time - (cur_time / 2.0f));

			//	GetTransform()->SetRelativePosXY(_offset);
			//}
		}
	}

	void ButtonUIScript::EnableFunc()
	{
	}

	void ButtonUIScript::DisableFunc()
	{
	}

	void ButtonUIScript::TickFunc()
	{
	}


	void ButtonUIScript::SetFocused()
	{
	}

	void ButtonUIScript::TapButton()
	{
		is_tap = true;
		effect->SetActive(true);
		ChangeButtonTexture(is_tap);
	}

	void ButtonUIScript::ChangeButtonTexture(bool _is_tap)
	{
		if (_is_tap)
		{
			SetSize(vec2(343.f, 40.f), vec2(1.0f, 1.0f));
			SetLeftTop(vec2(561.f, 1202.f));
		}
		else
		{
			SetSize(vec2(343.f, 40.f), vec2(1.0f, 1.0f));
			SetLeftTop(vec2(561.f, 1160.f));
		}
	}

}
