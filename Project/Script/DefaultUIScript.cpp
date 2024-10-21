#include "pch.h"

#include "UIMgr.h"
#include "ButtonUIScript.h"
#include "TextUIScript.h"
#include "ImageUIScript.h"

#include "DefaultUIScript.h"
namespace ff7r
{
	DefaultUIScript::DefaultUIScript()
		: UIScript(SCRIPT_TYPE::DEFAULTUISCRIPT)
	{
	}
	DefaultUIScript::~DefaultUIScript()
	{
	}

	void DefaultUIScript::Awake()
	{
		UIScript::Awake();

		GetOwner()->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetOwner()->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		GetOwner()->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));

		SetSize(vec2(306.f, 40.f), vec2(1.0f, 3.0f));
		SetLeftTop(vec2(563.f, 1492.f));

		CreateChild();		
	}

	void DefaultUIScript::Tick()
	{
		if (KEY_TAP(KEY::SPACE))
		{
			UIMgr::GetInst()->CallUI(L"Command Menu UI");
		}
	}
	void DefaultUIScript::OnEnable()
	{
	}
	void DefaultUIScript::OnDisable()
	{
	}

	void DefaultUIScript::CreateChild()
	{
		n_attack_button = new GameObject;
		n_attack_button->AddComponent(new Transform);
		n_attack_button->AddComponent(new MeshRender);
		n_attack_button->AddComponent(new ButtonUIScript);

		n_attack_button->SetLayerIndex(31);
		n_attack_button->GetTransform()->SetRelativePosXY(vec2(31.f, 25.f));
		n_attack_button->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		n_attack_button->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		n_attack_button->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		n_attack_button->SetName(L"Attack Select Button");

		ButtonUIScript* t_ui = n_attack_button->GetScript<ButtonUIScript>();
		t_ui->SetKeyType(KEY::LBTN);

		GameObject* _text_obj = new GameObject;
		_text_obj->AddComponent(new Transform);
		_text_obj->AddComponent(new FontRenderer);
		_text_obj->AddComponent(new TextUIScript);
		_text_obj->SetLayerIndex(31);
		_text_obj->SetName(L"Text");

		_text_obj->GetTransform()->SetRelativePosXY(vec2(-117.f, -2.f));
		_text_obj->GetTransform()->SetRelativeScaleXY(vec2(0.6f, 0.6f));
		TextUIScript* text_ui = _text_obj->GetScript<TextUIScript>();
		text_ui->SetText("Attack");
		n_attack_button->AddChild(_text_obj);

		GetOwner()->AddChild(n_attack_button);

		GameObject* _image_ui = new GameObject;
		_image_ui->AddComponent(new Transform);
		_image_ui->AddComponent(new MeshRender);
		_image_ui->AddComponent(new ImageUIScript);
		_image_ui->SetLayerIndex(31);

		_image_ui->GetTransform()->SetRelativePosXY(vec2(-147.f, 0.f));
		_image_ui->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_image_ui->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_image_ui->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas02.png"));
		_image_ui->SetName(L"Image");

		UIScript* _ui = _image_ui->GetScript<UIScript>();
		_ui->SetSize(vec2(43.f, 54.f), vec2(0.5f, 0.5f));
		_ui->SetLeftTop(vec2(514.f, 634.f));
		
		n_attack_button->AddChild(_image_ui);
		// =============================================================================

		command_ui = new GameObject;
		command_ui->AddComponent(new Transform);
		command_ui->AddComponent(new MeshRender);
		command_ui->AddComponent(new ButtonUIScript);

		command_ui->SetLayerIndex(31);
		command_ui->GetTransform()->SetRelativePosXY(vec2(31.f, -25.f));
		command_ui->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		command_ui->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		command_ui->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		command_ui->SetName(L"Attack Select Button");

		t_ui = command_ui->GetScript<ButtonUIScript>();
		t_ui->SetKeyType(KEY::SPACE);


		_text_obj = new GameObject;
		_text_obj->AddComponent(new Transform);
		_text_obj->AddComponent(new FontRenderer);
		_text_obj->AddComponent(new TextUIScript);
		_text_obj->SetLayerIndex(31);
		_text_obj->SetName(L"Text");

		_text_obj->GetTransform()->SetRelativePosXY(vec2(-117.f, -2.f));
		_text_obj->GetTransform()->SetRelativeScaleXY(vec2(0.6f, 0.6f));
		text_ui = _text_obj->GetScript<TextUIScript>();
		text_ui->SetText("Commands Menu");
		command_ui->AddChild(_text_obj);

		GetOwner()->AddChild(command_ui);

		_image_ui = new GameObject;
		_image_ui->AddComponent(new Transform);
		_image_ui->AddComponent(new MeshRender);
		_image_ui->AddComponent(new ImageUIScript);
		_image_ui->SetLayerIndex(31);

		_image_ui->GetTransform()->SetRelativePosXY(vec2(-147.f, 0.f));
		_image_ui->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_image_ui->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_image_ui->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas02.png"));
		_image_ui->SetName(L"Image");

		_ui = _image_ui->GetScript<UIScript>();
		_ui->SetSize(vec2(46.f, 46.f), vec2(0.5f, 0.5f));
		_ui->SetLeftTop(vec2(1127.f, 563.f));

		command_ui->AddChild(_image_ui);
	}
}