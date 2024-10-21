#include "pch.h"
#include "UIMgr.h"

#include <Engine\components.h>

#include <Engine\ResourceMgr.h>
#include <Engine\RenderMgr.h>
#include <Engine\TimeMgr.h>
#include <Engine\KeyMgr.h>
#include <Engine\Device.h>

#include "UIScript.h"
#include "DefaultUIScript.h"
#include "ImageUIScript.h"
#include "MenuUIScript.h"
#include "ButtonUIScript.h"
#include "TextUIScript.h"
#include "CharacterUIScript.h"
#include "WorldTextUIScript.h"
#include "TargetUIScript.h"
#include "FindEnemyScript.h"

#include "GameMgr.h"

#include <Engine\ConstBuffer.h>
#include <Engine\LevelMgr.h>
#include <Engine\FontMgr.h>
#include <Engine\GameObject.h>
#include <Engine\TimeMgr.h>

namespace ff7r
{
	GameObject* UIMgr::canvas = nullptr;
	UIMgr::UIMgr()
		: ui_objects{}
		, ui_stack()
	{

	}

	UIMgr::~UIMgr()
	{
		//SafeDelVector(UI_objects);
		//delete canvas;
		//canvas = nullptr;
	}

	void UIMgr::Init()
	{
		
		canvas = new GameObject;
		canvas->AddComponent(new Transform);
		canvas->SetName(L"canvas");
		SpawnGameObject((GameObject*)canvas, vec3(0.f, 0.f, 0.f), L"ViewPort UI");
		canvas->SetDestroy(false);


		GameObject* _default_ui = new GameObject;
		_default_ui->AddComponent(new Transform);
		_default_ui->AddComponent(new MeshRender);
		_default_ui->AddComponent(new DefaultUIScript);
		_default_ui->GetTransform()->SetRelativePosXY(vec2(-588.f, -370.f));
		_default_ui->SetName(L"Default UI");
		
		AddUIObject(_default_ui->GetName(), _default_ui->GetScript<UIScript>());
		ui_stack.push_back(_default_ui->GetScript<UIScript>());

		canvas->AddChild(_default_ui);

	
		GameObject* _command_menu_ui = new GameObject;
		_command_menu_ui->AddComponent(new Transform);
		_command_menu_ui->AddComponent(new MeshRender);
		_command_menu_ui->AddComponent(new MenuUIScript);

		_command_menu_ui->GetTransform()->SetRelativePosXY(vec2(-569.f, -250.f));
		_command_menu_ui->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_command_menu_ui->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_command_menu_ui->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_command_menu_ui->SetName(L"Command Menu UI");

		UIScript* _ui = _command_menu_ui->GetScript<UIScript>();
		_ui->SetSize(vec2(344.f, 250.f), vec2(1.0f, 1.0f));
		_ui->SetLeftTop(vec2(405.f, 3.f));

		_command_menu_ui->SetActive(false);
		AddUIObject(_command_menu_ui->GetName(), _command_menu_ui->GetScript<UIScript>());
		canvas->AddChild(_command_menu_ui);


		GameObject* _hud_ui = new GameObject;
		_hud_ui->AddComponent(new Transform);
		_hud_ui->AddComponent(new MeshRender);
		_hud_ui->AddComponent(new CharacterUIScript);

		_hud_ui->GetTransform()->SetRelativePosXY(vec2(587.f, -374.f));
		_hud_ui->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_hud_ui->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_hud_ui->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_hud_ui->SetName(L"Character HUD");

		_ui = _hud_ui->GetScript<UIScript>();
		_ui->SetSize(vec2(362.f, 50.f), vec2(1.0f, 2.0f));
		_ui->SetLeftTop(vec2(1414.f, 800.f));

		CharacterUIScript* _character_ui = _hud_ui->GetScript<CharacterUIScript>();
		_character_ui->SetCharacter(GameMgr::GetInst()->GetCurPlayer());

		_hud_ui->SetActive(true);
		canvas->AddChild(_hud_ui);

		GameObject* _hud_ui_2 = new GameObject;
		_hud_ui_2->AddComponent(new Transform);
		_hud_ui_2->AddComponent(new MeshRender);
		_hud_ui_2->AddComponent(new CharacterUIScript);

		_hud_ui_2->GetTransform()->SetRelativePosXY(vec2(587.f, -266.f));
		_hud_ui_2->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_hud_ui_2->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		_hud_ui_2->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_hud_ui_2->SetName(L"Character2 HUD");

		_ui = _hud_ui_2->GetScript<UIScript>();
		_ui->SetSize(vec2(362.f, 50.f), vec2(1.0f, 2.0f));
		_ui->SetLeftTop(vec2(1414.f, 800.f));

		_character_ui = _hud_ui_2->GetScript<CharacterUIScript>();
		_character_ui->SetCharacter(GameMgr::GetInst()->GetOtherPlayer());

		_hud_ui_2->SetActive(true);
		canvas->AddChild(_hud_ui_2);

		GameObject* _target_ui = new GameObject;
		_target_ui->AddComponent(new Transform);
		_target_ui->AddComponent(new MeshRender);
		_target_ui->AddComponent(new TargetUIScript);
		_target_ui->SetName(L"Target UI");
		_target_ui->SetLayerIndex(31);

		_target_ui->SetActive(false);
		canvas->AddChild(_target_ui);
	}



	void UIMgr::Progress()
	{
		GameObject* _player = GameMgr::GetInst()->GetCurPlayer();
		if (_player)
		{
			GameObject* _target = _player->GetScript<FindEnemyScript>()->FindClosestEnemy();

			canvas->GetScriptInChildren<TargetUIScript>()->SetTarget(_target);
		}
		//_frame_object->GetFontRenderer()->SetTexts(string("Button1"));

		//if (TimeMgr::GetInst()->GetAccTime() >= 0.95f && TimeMgr::GetInst()->GetAccTime() <= 0.96f)
		//{
		//	time_check = 0.0f;
		//	UINT _fps = TimeMgr::GetInst()->GetFramePerSecond();
		//	if (_fps >= 1000)
		//	{
		//		int a = 0;
		//	}
		//	string _str_fps = std::to_string(_fps);
		//	frame_object->GetFontRenderer()->SetTexts(_str_fps);
		//}
	}

	void UIMgr::AddUIObject(wstring _name, UIScript* _UI)
	{
		ui_objects.insert(std::make_pair(_name, _UI));
	}

	void UIMgr::CallUI(wstring _UI_name)
	{
		if (_UI_name == L"")
			return;
		ui_stack[ui_stack.size() - 1]->SetActive(false);
		ui_stack.push_back(ui_objects[_UI_name]);
		ui_stack[ui_stack.size() - 1]->SetActive(true);
	}

	void UIMgr::ReturnUI()
	{
		if (ui_stack.size() == 1)
		{
			ui_stack[ui_stack.size() - 1]->SetUIFunc(UIScript::UI_FUNC::TICK);

			return;
		}
		ui_stack[ui_stack.size() - 1]->SetActive(false);
		ui_stack.pop_back();
		ui_stack[ui_stack.size() - 1]->SetActive(true);
	}

	void UIMgr::ResetStack()
	{
		while (ui_stack.size() > 1)
		{
			ui_stack[ui_stack.size() - 1]->GetOwner()->SetActive(false);
			ui_stack.pop_back();
		}
		ui_stack[0]->GetOwner()->SetActive(true);
	}

	void UIMgr::PrintWorldTextUI(string _text, bool _is_text, bool _is_damage_text, vec3 _world_pos, vec3 _world_scale)
	{
		GameObject* _text_ui = new GameObject;
		_text_ui->AddComponent(new Transform);
		_text_ui->AddComponent(new FontRenderer);
		_text_ui->AddComponent(new WorldTextUIScript);
		_text_ui->GetTransform()->SetRelativePos(_world_pos);
		_text_ui->GetTransform()->SetRelativeScale(_world_scale);
		_text_ui->SetLayerIndex(31);
		_text_ui->Awake();
		_text_ui->SetName(L"World Text");
		WorldTextUIScript* _font = _text_ui->GetScript<WorldTextUIScript>();
		_font->SetText(_text, _is_text, _is_damage_text);

		canvas->AddChild(_text_ui);
	}



}