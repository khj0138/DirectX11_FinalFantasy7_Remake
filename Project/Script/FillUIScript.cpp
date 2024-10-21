#include "pch.h"

#include "PlayerDataScript.h"
#include "ImageUIScript.h"

#include "FillUIScript.h"
namespace ff7r
{
	FillUIScript::FillUIScript()
		: UIScript(SCRIPT_TYPE::FILLUISCRIPT)
		, target(nullptr)
		, type(HUD_TYPE::HP)
	{
	}
	FillUIScript::~FillUIScript()
	{
	}

	void FillUIScript::Awake()
	{
		UIScript::Awake();
		InitSetting();
	}

	void FillUIScript::Tick()
	{
		PlayerDataScript* _player = dynamic_cast<PlayerDataScript*>(target);
		if (_player)
		{
			switch (type)
			{
			case HUD_TYPE::HP:
			{
				SetFillAmount(_player->GetCurHP() / _player->GetMaxHP());
			}
			break;

			case HUD_TYPE::MP:
			{
				SetFillAmount(_player->GetCurMP() / _player->GetMaxMP());
			}
			break;

			case HUD_TYPE::ATB:
			{
				SetFillAmount(_player->GetAtbGauge() / 2.0f);
				if (_player->GetAtbGauge() >= 1.0f)
				{
					highlight[0]->SetActive(true);
				}
				else
				{
					highlight[0]->SetActive(false);
				}

				if (_player->GetAtbGauge() >= 2.0f)
				{
					highlight[1]->SetActive(true);
				}
				else
				{
					highlight[1]->SetActive(false);
				}
			}
			break;

			case HUD_TYPE::LIMIT:
			{
				SetFillAmount(_player->GetLimitGauge());
			}
			break;
			}
		}
		else if (target)
		{
			SetFillAmount((float)(target->GetCurHP()) / (float)(target->GetMaxHP()));
		}
	}

	void FillUIScript::SetFillColor(vec4 _color)
	{
		GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, _color);
	}

	void FillUIScript::SetFillAmount(float _amount)
	{
		GetMeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &_amount);
	}

	void FillUIScript::SetFillRect(vec2 _fill_lt, vec2 _fill_rb, vec2 _image_left_top, vec2 _size)
	{
		_fill_lt -= _image_left_top;
		_fill_rb -= _image_left_top;

		_fill_lt /= _size;

		_fill_rb += 1.0f;
		_fill_rb /= _size;

		GetMeshRender()->GetMaterial()->SetScalarParam(VEC2_2, &_fill_lt);
		GetMeshRender()->GetMaterial()->SetScalarParam(VEC2_3, &_fill_rb);
	}

	void FillUIScript::InitSetting()
	{
		PlayerDataScript* _player = dynamic_cast<PlayerDataScript*>(target);
		if (target)
		{
			switch (type)
			{
			case HUD_TYPE::HP:
			{
				SetLeftTop(vec2(753.f, 258.f));
				SetSize(vec2(74.f, 5.f), vec2(3.24324324f, 1.2f));
				SetFillColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
				GetTransform()->SetRelativePosXY(vec2(-53.f, -4.f));
				SetFillRect(vec2(756.f, 259.f), vec2(823.f, 261.f), vec2(753.f, 258.f), vec2(74.f, 5.f));
			}
			break;

			case HUD_TYPE::MP:
			{
				SetLeftTop(vec2(753.f, 258.f));
				SetSize(vec2(74.f, 5.f), vec2(1.162162f, 1.2f));
				SetFillColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
				GetTransform()->SetRelativePosXY(vec2(122.f, -4.f));
				SetFillRect(vec2(756.f, 259.f), vec2(823.f, 261.f), vec2(753.f, 258.f), vec2(74.f, 5.f));
			}
			break;

			case HUD_TYPE::LIMIT:
			{
				SetLeftTop(vec2(753.f, 258.f));
				SetSize(vec2(74.f, 5.f), vec2(1.162162f, 1.8f));
				SetFillColor(vec4(1.0f, 0.5f, 0.0f, 1.0f));
				GetTransform()->SetRelativePosXY(vec2(122.f, -33.f));
				SetFillRect(vec2(756.f, 259.f), vec2(823.f, 261.f), vec2(753.f, 258.f), vec2(74.f, 5.f));
			}
			break;

			case HUD_TYPE::ATB:
			{
				SetLeftTop(vec2(1474.f, 254.f));
				SetSize(vec2(216.f, 9.f), vec2(1.11111111f, 1.f));
				SetFillColor(vec4(2.f / 255.f, 201.f / 255.f, 237.f / 255.f, 1.0f));
				GetTransform()->SetRelativePosXY(vec2(-53.f, -33.f));
				SetFillRect(vec2(1477.f, 255.f), vec2(1686.f, 261.f), vec2(1474.f, 254.f), vec2(210.f, 7.f));


				GameObject* _highlight_0 = new GameObject;
				_highlight_0->AddComponent(new Transform);
				_highlight_0->AddComponent(new MeshRender);
				_highlight_0->AddComponent(new ImageUIScript);
				_highlight_0->SetLayerIndex(31);

				_highlight_0->GetTransform()->SetRelativePosXY(vec2(-60.f, -1.f));
				_highlight_0->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
				_highlight_0->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
				_highlight_0->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
				_highlight_0->SetName(L"ATB high light image 0");

				ImageUIScript* _image = _highlight_0->GetScript<ImageUIScript>();
				_image->SetSize(vec2(59.f, 4.f), vec2(1.5f, 1.0f));
				_image->SetLeftTop(vec2(692.f, 258.f));
				_image->SetImageEffectType(ImageUIScript::ImageEffectType::ALPHA_CHANGE);
				_image->SetAlphaFactor(3.0f);

				_image->SetActive(false);
				highlight.push_back(_image);
				GetOwner()->AddChild(_highlight_0);

				_highlight_0 = new GameObject;
				_highlight_0->AddComponent(new Transform);
				_highlight_0->AddComponent(new MeshRender);
				_highlight_0->AddComponent(new ImageUIScript);
				_highlight_0->SetLayerIndex(31);

				_highlight_0->GetTransform()->SetRelativePosXY(vec2(60.f, -1.f));
				_highlight_0->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
				_highlight_0->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
				_highlight_0->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
				_highlight_0->SetName(L"ATB high light image 1");

				_image = _highlight_0->GetScript<ImageUIScript>();
				_image->SetSize(vec2(59.f, 4.f), vec2(1.5f, 1.0f));
				_image->SetLeftTop(vec2(692.f, 258.f));
				_image->SetImageEffectType(ImageUIScript::ImageEffectType::ALPHA_CHANGE);
				_image->SetAlphaFactor(3.0f);

				_image->SetActive(false);
				highlight.push_back(_image);
				GetOwner()->AddChild(_highlight_0);
			}
			break;

			default: break;
			}
			SetFillAmount(1.0f);

		}
		else
		{
			SetLeftTop(vec2(910.f, 853.f));
			SetSize(vec2(365.f, 52.f), vec2(0.6f, 0.25f));
			SetFillColor(vec4(239.f / 255.f, 255.f / 255.f, 198.f / 255.f, 1.0f));
			GetTransform()->SetRelativePosXY(vec2(0.f, 120.f));
			SetFillRect(vec2(917.f, 860.f), vec2(1267.f, 897.f), vec2(910.f, 853.f), vec2(351.f, 38.f));
			SetFillAmount(0.8f);

		}
		//SetFillAmount(1.0f);
	}
}