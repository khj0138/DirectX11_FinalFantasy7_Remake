#include "pch.h"

#include "UIMgr.h"

#include "UIScript.h"
namespace ff7r
{
	UIScript::UIScript(UINT _ScriptType)
		: Script(_ScriptType)
		, is_enable(false)
		, pivot(vec2(0.0f, 0.0f))
		, pos(vec2(0.0f, 0.0f))
		, scale(vec2(1.0f, 1.0f))
		, rot(vec3(0.0f, 0.0f, 0.0f))
		, time_val(0.0f)
		, enable_time(0.0f)
	{
		AddScriptParam(SCRIPT_PARAM::VEC2, &pos, "UI Pos");
		AddScriptParam(SCRIPT_PARAM::VEC2, &pivot, "UI Pivot");
		AddScriptParam(SCRIPT_PARAM::VEC2, &scale, "UI Scale");
		AddScriptParam(SCRIPT_PARAM::VEC3, &rot, "UI Rot");
	}

	UIScript::~UIScript()
	{
	}

	void UIScript::OnEnable()
	{
		
		is_enable = true;
		SetUIFunc(UI_FUNC::ENABLE);
	}

	void UIScript::OnDisable()
	{
		is_enable = false;
		SetUIFunc(UI_FUNC::DISABLE);
	}

	void UIScript::SetActive(bool _active)
	{
		GetOwner()->SetActive(_active);
		//if (is_enable != _active)
		//{
		//	if (_active)
		//	{
		//		GetOwner()->SetActive(true);
		//	}
		//	else
		//	{
		//		OnDisable();
		//	}
		//}
	}

	/*bool UIScript::CheckUIFunc(float _condition_val)
	{
		time_val += TimeMgr::GetInst()->GetDeltaTime();
		if (time_val > _condition_val)
		{
			time_val = 0.0f;
			return true;
		}
		return false;
	}*/

	void UIScript::SetSize(vec2 _size, vec2 _scale_ratio)
	{
		Ptr<Texture> _tex = GetMeshRender()->GetMaterial()->GetTexParam(TEX_0);

		vec2 _uv_size = vec2(_size.x / _tex->Width(), _size.y / _tex->Height());
		GetMeshRender()->GetMaterial()->SetScalarParam(VEC2_1, &_uv_size);
		vec2 _scale = _size * _scale_ratio;

		GetTransform()->SetRelativeScale(vec3(_scale.x, _scale.y, 1.0f));
	}

	void UIScript::SetLeftTop(vec2 _left_top)
	{
		Ptr<Texture> _tex = GetMeshRender()->GetMaterial()->GetTexParam(TEX_0);

		vec2 _uv_lt = vec2(_left_top.x / _tex->Width(), _left_top.y / _tex->Height());
		GetMeshRender()->GetMaterial()->SetScalarParam(VEC2_0, &_uv_lt);
	}

	void UIScript::SetUIFunc(UI_FUNC _func)
	{
		switch (_func)
		{
		case UI_FUNC::TICK:
		{
			UI_Func = &UIScript::TickFunc;
			break;
		}
		case UI_FUNC::ENABLE:
		{
			UI_Func = &UIScript::EnableFunc;
			break;
		}
		case UI_FUNC::DISABLE:
		{
			UI_Func = &UIScript::DisableFunc;
			break;
		}
		case UI_FUNC::SELECT:
		{
			UI_Func = &UIScript::SelectFunc;
			break;
		}
		case UI_FUNC::CALL:
		{
			UI_Func = &UIScript::CallFunc;
			break;
		}
		case UI_FUNC::RETURN:
		{
			UI_Func = &UIScript::ReturnFunc;
			break;
		}
		case UI_FUNC::RESET:
		{
			UI_Func = &UIScript::ResetFunc;
			break;
		}
		default:
			break;
		}
	}

	void UIScript::Awake()
	{
		if (GetOwner()->GetRenderComponent())
		{
			GetOwner()->GetRenderComponent()->SetFrustumCheck(false);
		}
	}

	void UIScript::Tick()
	{
		//(this->*UI_Func)();


		//GetTransform()->SetRelativePos(vec3(pos.x, pos.y, 1.0f));
		//GetTransform()->SetRelativeScale(vec3(scale.x, scale.y, 1.0f));
		//GetTransform()->SetRelativeRot(rot / 180.0f * XM_PI);
	}

}
