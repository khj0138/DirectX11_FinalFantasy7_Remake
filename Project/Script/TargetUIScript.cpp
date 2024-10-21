#include "pch.h"

#include <Engine\RenderMgr.h>
#include "ImageUIScript.h"

#include "TargetUIScript.h"
#include "FillUIScript.h"
#include "CharacterDataScript.h"
#include "TextUIScript.h"
namespace ff7r
{
	TargetUIScript::TargetUIScript()
		: UIScript((UINT)SCRIPT_TYPE::TARGETUISCRIPT)
		, stick_pos{ {10.f, 10.f}, {-10.f, 10.f}, {-10.f, -10.f}, {10.f, -10.f } }
		, time(0.f)
		, time_coeffi(3.0f)
		, enable_action(false)
		, target(nullptr)
	{
	}

	TargetUIScript::~TargetUIScript()
	{
	}

	void TargetUIScript::Awake()
	{
		UIScript::Awake();

		GetOwner()->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetOwner()->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		GetOwner()->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas04.png"));

		SetSize(vec2(53.f, 53.f), vec2(1.5f, 1.5f));
		SetLeftTop(vec2(1122.f, 1448.f));

		CreateChild();
	}

	void TargetUIScript::Tick()
	{
		if (!target || !target->GetCollider3D())
		{
			return;
		}
		if (hp_bar)
		{
			hp_bar	->GetScript<FillUIScript>()->SetTarget(target->GetScript<CharacterDataScript>());
		}
		if (name_text)
		{
			GameObject* _target = name_text->GetOwner();
			if (_target)
				name_text->GetOwner()->GetTransform()->SetRelativePosXY(vec2((target->GetName()).size() * -6.0f, 140.0f));
		}
		vec3 _world_pos = target->GetTransform()->GetWorldPos() + target->GetCollider3D()->GetOffsetPos();
		GetTransform()->SetRelativePos(_world_pos);
		CalcUIPos();

		time += DT * time_coeffi;

		if (enable_action)
		{
			float _t = time * 2.0f;
			float _alpha = _t;
			float _scale_factor = (1.0f - _t);
			if (_t >= 1.0f)
			{
				_alpha = 0.0f;
				enable_action = false;

				for (size_t i = 0; i < 4; i++)
				{
					sticks[i]->GetTransform()->SetRelativeScaleXY(16.5f, 6.f);
				}
				GetTransform()->SetRelativeScaleXY((53.f * 1.5f), (53.f * 1.5f));
				GetOwner()->GetMeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &_alpha);
			}
			else
			{
				for (size_t i = 0; i < 4; i++)
				{
					sticks[i]->GetTransform()->SetRelativeScaleXY(16.5f * (1.f + _scale_factor), 6.f * (1.f + _scale_factor));
				}
				GetTransform()->SetRelativeScaleXY((53.f * 15.f) * (1.f + _scale_factor), (53.f * 15.f) * (1.f + _scale_factor));
				GetOwner()->GetMeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &_alpha);
			}
		}

		if (time < 0.0f)
		{
			time = 0.0f;
			time_coeffi *= -1.0f;
		}
		else if (time >= 1.0f)
		{
			time = 1.0f;
			time_coeffi *= -1.0f;
		}

		for (size_t i = 0; i < 4; i++)
		{
			sticks[i]->GetTransform()->SetRelativePosXY(stick_pos[i] + stick_pos[i] * time * 0.5f);
		}
	}

	void TargetUIScript::OnEnable()
	{
		time = 0.0f;
		time_coeffi = 3.0f;
		enable_action = true;
	}

	void TargetUIScript::OnDisable()
	{
	}

	void TargetUIScript::SetTarget(GameObject* _target)
	{
		if (_target)
		{
			target = _target;
			if (hp_bar)
			{
				hp_bar->GetScript<FillUIScript>()->SetTarget(target->GetScript<CharacterDataScript>());
			}
			if (name_text)
			{
				wstring _name = target->GetName();
				name_text->SetText(string(_name.begin(), _name.end()));
			}

			SetActive(true);
		}
		else
		{
			target = _target;
			if (hp_bar)
			{
				hp_bar->GetScript<FillUIScript>()->SetTarget(nullptr);
			}
			SetActive(false);
		}
	}

	void TargetUIScript::CreateChild()
	{
		GameObject* _target_point = new GameObject;
		_target_point->AddComponent(new Transform);
		_target_point->AddComponent(new MeshRender);
		_target_point->AddComponent(new ImageUIScript);

		_target_point->SetLayerIndex(31);
		_target_point->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_target_point->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_target_point->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas04.png"));
		_target_point->SetName(L"Target Point UI");

		UIScript* _ui = _target_point->GetScript<UIScript>();
		_ui->SetSize(vec2(1.f, 1.f), vec2(5.0f, 5.0f));
		_ui->SetLeftTop(vec2(297.f, 1198.f));

		GetOwner()->AddChild(_target_point);

		for (size_t i = 0; i < 4; i++)
		{
			GameObject* _stick = new GameObject;
			_stick->AddComponent(new Transform);
			_stick->AddComponent(new MeshRender);
			_stick->AddComponent(new ImageUIScript);

			_stick->GetTransform()->SetRelativePosXY(stick_pos[i]);
			_stick->GetTransform()->SetRelativeRotZ(Degree2Rad(i * 90.f - 135.f));
			_stick->SetLayerIndex(31);
			_stick->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
			_stick->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
			_stick->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas04.png"));
			_stick->SetName(L"Stick UI" + to_wstring(i));

			UIScript* _ui = _stick->GetScript<UIScript>();
			_ui->SetSize(vec2(11.f, 4.f), vec2(1.5f, 1.5f));
			_ui->SetLeftTop(vec2(1116.f, 1948.f));
			 
			sticks.push_back(_stick);

			GetOwner()->AddChild(_stick);
		}
		for (size_t i = 0; i < 2; i++)
		{
			GameObject* _hp_arrow = new GameObject;
			_hp_arrow->AddComponent(new Transform);
			_hp_arrow->AddComponent(new MeshRender);
			_hp_arrow->AddComponent(new ImageUIScript);

			_hp_arrow->SetLayerIndex(31);
			_hp_arrow->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
			_hp_arrow->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
			_hp_arrow->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
			_hp_arrow->SetName(L"HP Arrow UI");

			UIScript* _ui = _hp_arrow->GetScript<UIScript>();
			_ui->SetSize(vec2(27.f, 18.f), vec2(1.5f, 1.5f) );
			_ui->SetLeftTop(vec2(2027.f, 739.f));

			GetOwner()->AddChild(_hp_arrow);
			if (i == 0)
			{
				_ui->GetTransform()->SetRelativePosXY(vec2(125.f, 118.f));
				_ui->GetTransform()->SetRelativeRot(vec3(0.f, 0.f, XM_PI));
			}
			else
			{
				_ui->GetTransform()->SetRelativePosXY(vec2(-125.f, 118.f));
			}
		}

		hp_bar = new GameObject;
		hp_bar->AddComponent(new Transform);
		hp_bar->AddComponent(new MeshRender);
		hp_bar->AddComponent(new FillUIScript);

		hp_bar->SetLayerIndex(31);
		hp_bar->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		hp_bar->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"FillUIMtrl"));
		hp_bar->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas02.png"));
		hp_bar->SetName(L"Monster HP UI");


		FillUIScript* _fill_ui = hp_bar->GetScript<FillUIScript>();
		_fill_ui->SetSize(vec2(365.f, 52.f), vec2(1.0f, 1.0f) );
		_fill_ui->SetLeftTop(vec2(910.f, 853.f));
		_fill_ui->SetHUDType(FillUIScript::HUD_TYPE::HP);
		if(target)
			_fill_ui->SetTarget(target->GetScript<CharacterDataScript>());

		GetOwner()->AddChild(hp_bar);

		GameObject* _name = new GameObject;
		_name->AddComponent(new Transform);
		_name->AddComponent(new FontRenderer);
		_name->AddComponent(new TextUIScript);

		_name->SetLayerIndex(31);
		_name->SetName(L"Enemy HP Text");
		_name->GetTransform()->SetRelativePosXY(0.f, 140.f);
		_name->GetTransform()->SetRelativeScaleXY(0.4f, 0.4f);

		name_text = _name->GetScript<TextUIScript>();
		name_text->CreateDynamicMtrl();
		name_text->SetText("");
		name_text->SetColor(vec4(180.f / 255.f, 180.f / 255.f, 180.f / 255.f, 1.0f));

		GetOwner()->AddChild(_name);
		
	}

	void TargetUIScript::CalcUIPos()
	{
		GetTransform()->FinalTick();
		Matrix world_mat = GetTransform()->GetWorldMat();
		// RenderMgr::GetInst()->GetMainCam();

		Camera* main_cam = RenderMgr::GetInst()->GetEditorCam();

		Matrix wvp = world_mat * main_cam->GetViewMat() * main_cam->GetProjMat();

		vec4 pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		vec4 clip_pos = XMVector4Transform(pos, wvp);

		if (clip_pos.z <= 0.f)
		{
			GetMeshRender()->SetRender(false);
			return;
		}
		else
		{
			GetMeshRender()->SetRender(true);
		}

		vec3 ndc_pos;
		ndc_pos.x = clip_pos.x / clip_pos.w;
		ndc_pos.y = clip_pos.y / clip_pos.w;
		ndc_pos.z = clip_pos.z / clip_pos.w;

		float screenX = (ndc_pos.x + 1.0f) * 0.5f * 1600.f;
		float screenY = (1.0f - ndc_pos.y) * 0.5f * 900.f;
		screenX -= 800.f;
		screenY -= 450.f;
		screenY *= -1.f;

		GetTransform()->SetRelativePosXY(vec2(screenX, screenY));
	}
}