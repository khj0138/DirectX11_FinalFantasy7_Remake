#include "pch.h"
#include "UIMgr.h"

#include "TextUIScript.h"
#include "ButtonUIScript.h"

#include <Engine\FontMgr.h>

namespace ff7r
{
	TextUIScript::TextUIScript()
		: UIScript((UINT)SCRIPT_TYPE::TEXTUISCRIPT)
		, is_dynamic_mtrl(false)
	{
		//SetEnableTime(1.0f);
	}

	TextUIScript::~TextUIScript()
	{
	}

	void TextUIScript::Awake()
	{
		UIScript::Awake();

		CreateDynamicMtrl();
	}

	void TextUIScript::CreateDynamicMtrl()
	{
		if (!is_dynamic_mtrl)
		{
			GetFontRenderer()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\English_Font.png"));
			vec4 _none = vec4(0.0f, 0.0f, 0.0f, 0.0f);
			GetFontRenderer()->GetMaterial()->SetScalarParam(VEC4_0, _none);
			is_dynamic_mtrl = true;
		}
	}

	void TextUIScript::SetText(string _text)
	{
		GetFontRenderer()->SetTexts(_text);
		GetFontRenderer()->GetMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\English_Font.png"));
	}

	void TextUIScript::SetColor(vec4 _color)
	{
		GetFontRenderer()->GetMaterial()->SetScalarParam(VEC4_0, _color);
	}

	void TextUIScript::SetDamageText(string _text)
	{
		GetFontRenderer()->SetTexts(_text, false);
		GetFontRenderer()->GetMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\Damage_Font.png"));
	}

	void TextUIScript::SetAlphaFactor(float _factor)
	{
		GetFontRenderer()->GetMaterial()->SetScalarParam(FLOAT_0, &_factor);
	}

	void TextUIScript::RegisterText(string _sentence, vec2 _pos)
	{
		FontMgr::GetInst()->MakeSentence(_sentence, _pos);
	}

	void TextUIScript::UpdateChilds()
	{
		//childs.clear();
		//const vector<GameObject*>& _child_objects = GetOwner()->GetChild();
		//for (int i = 0; i < _child_objects.size(); i++)
		//{
		//	ButtonUIScript* _button = _child_objects[i]->GetScript<ButtonUIScript>();
		//	if (_button != nullptr)
		//	{
		//		childs.push_back(_button);
		//	}
		//}
	}


}
