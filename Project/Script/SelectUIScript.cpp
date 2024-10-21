#include "pch.h"

#include "TextUIScript.h"
#include "ImageUIScript.h"

#include "SelectUIScript.h"
namespace ff7r
{
	SelectUIScript::SelectUIScript()
		: UIScript(SCRIPT_TYPE::SELECTUISCRIPT)
		, text_ui(nullptr)
		, target(nullptr)
		, is_focus(false)
		, is_valid(true)
		, cost(0)
	{
	}

	SelectUIScript::~SelectUIScript()
	{
	}

	void SelectUIScript::Awake()
	{
		UIScript::Awake();

		GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));

		SetSize(vec2(343.f, 38.f), vec2(1.0f, 1.0f));
		SetLeftTop(vec2(561.f, 1328.f));

		vec3 _trans = GetTransform()->GetRelativeScale();
		origin_scale = vec2(_trans.x, _trans.y);

		_trans = GetTransform()->GetRelativePos();
		origin_pos = vec2(_trans.x, _trans.y);

		GameObject* _text_obj = new GameObject;
		_text_obj->AddComponent(new Transform);
		_text_obj->AddComponent(new FontRenderer);
		_text_obj->AddComponent(new TextUIScript);
		_text_obj->SetLayerIndex(31);
		_text_obj->SetName(L"Text");
		
		_text_obj->GetTransform()->SetRelativePosXY(vec2(-153.f, 0.f));
		_text_obj->GetTransform()->SetRelativeScaleXY(vec2(0.6f, 0.6f));
		text_ui = _text_obj->GetScript<TextUIScript>();

		GetOwner()->AddChild(_text_obj);

		GameObject* _child_obj = new GameObject;
		_child_obj->AddComponent(new Transform);
		_child_obj->AddComponent(new MeshRender);
		_child_obj->AddComponent(new ImageUIScript);
		_child_obj->SetLayerIndex(31);

		_child_obj->GetTransform()->SetRelativePosXY(vec2(-140.f, 5.f));
		_child_obj->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_child_obj->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_child_obj->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_child_obj->SetName(L"Child Target Image");

		child_ui = _child_obj->GetScript<ImageUIScript>();
		child_ui->SetSize(vec2(12.f, 26.f), vec2(1.0f, 1.0f));
		child_ui->SetLeftTop(vec2(547.f, 1696.f));

		_child_obj->SetActive(false);

		GetOwner()->AddChild(_child_obj);
	}

	void SelectUIScript::Tick()
	{
		if (is_fade_effect)
		{
			time += REAL_DT * 5.0f;

			if (time >= 1.0f)
			{
				GetTransform()->SetRelativeScaleXY(origin_scale);
				GetTransform()->SetRelativePosXY(origin_pos);
				float _alpha = 1.0f;
				GetMeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &_alpha);
				text_ui->SetAlphaFactor(_alpha);
				is_fade_effect = false;
			}
			else
			{
				GetTransform()->SetRelativeScaleXY(origin_scale * 0.8f + (origin_scale * time) * 0.2f);
				GetTransform()->SetRelativePosXY(origin_pos.x - (25.f * (1 - time)), origin_pos.y - (25.f * (1 - time)));
				GetMeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &time);
				text_ui->SetAlphaFactor(time);
			}
		}
	}

	void SelectUIScript::OnEnable()
	{
		is_fade_effect = true;
		time = 0.0f;

		if (is_valid)
		{
			text_ui->SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			text_ui->SetColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));
		}
	}

	void SelectUIScript::OnDisable()
	{
		target = nullptr;
		is_focus = false;
		is_valid = true;

		if (text_ui)
		{
			text_ui->GetTransform()->SetRelativePosXY(vec2(-153.f, 0.f));
		}
		if (child_ui)
		{
			child_ui->SetActive(false);
		}
	}

	void SelectUIScript::SetFocus(bool _is_focus)
	{
		is_focus = _is_focus;

		if (is_focus)
		{
			SetLeftTop(vec2(561.f, 1410.f));
		}
		else
		{
			SetLeftTop(vec2(561.f, 1328.f));
		}
	}

	void SelectUIScript::SetText(string _text)
	{
		text_ui->SetText(_text);
		text = _text;
	}

	void SelectUIScript::SetTarget(GameObject* _target, bool _is_child)
	{
		target = _target;

		if (_is_child)
		{
			text_ui->GetTransform()->SetRelativePosXY(vec2(-132.f, 0.f));
			child_ui->SetActive(true);
		}
	}
	void SelectUIScript::SetDamageText(string _text)
	{
		text_ui->SetDamageText(_text);
		text = _text;
	}
}