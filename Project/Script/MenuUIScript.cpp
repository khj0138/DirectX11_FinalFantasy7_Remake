#include "pch.h"
#include "UIMgr.h"

#include <Engine\RenderMgr.h>
#include "ObjectPoolMgr.h"
#include "GameMgr.h"

#include "CloudMoveScript.h"
#include "BarretMotionScript.h"
#include "CharacterDataScript.h"

#include "ImageUIScript.h"
#include "SelectUIScript.h"

#include "MenuUIScript.h"
namespace ff7r
{
	MenuUIScript::MenuUIScript()
		: UIScript((UINT)SCRIPT_TYPE::MENUUISCRIPT)
		, player(nullptr)
		, portrait(nullptr)
		, focused_idx(0)
		, active_select_cnt(0)
	{
		type = MENU_TYPE::END;
	}

	MenuUIScript::~MenuUIScript()
	{
	}

	void MenuUIScript::Awake()
	{
		UIScript::Awake();

		CreateChild();

	}

	void MenuUIScript::Tick()
	{
		if (KEY_TAP(KEY::ESC))
		{
			ui_stack.pop();
			if (ui_stack.empty())
			{
				UIMgr::GetInst()->ReturnUI();
				return;
			}

			ChangeType(ui_stack.top());
		}
		else if (KEY_TAP(KEY::ENTER))
		{
			PushNextType();
		}
		else if (KEY_TAP(KEY::DOWN) || KEY_TAP(KEY::UP))
		{
			if (active_select_cnt > 0)
			{
				int _prev_idx = focused_idx;
				if (KEY_TAP(KEY::DOWN))
				{
					focused_idx = (focused_idx + 1) < active_select_cnt ? focused_idx + 1 : 0;
				}
				else if (KEY_TAP(KEY::UP))
				{
					focused_idx = (focused_idx - 1) >= 0 ? focused_idx - 1 : active_select_cnt - 1;
				}

				select_ui[_prev_idx]->SetFocus(false); 
				select_ui[focused_idx]->SetFocus(true);
			}
		}
		else if (KEY_TAP(KEY::X))
		{
			player = GameMgr::GetInst()->GetOtherPlayer(player);
			if (player->GetName() == L"Cloud")
			{
				UIScript* _ui = portrait->GetScript<UIScript>();
				_ui->SetLeftTop(vec2(673.f, 1858.f));
			}
			else if (player->GetName() == L"Barret")
			{
				UIScript* _ui = portrait->GetScript<UIScript>();
				_ui->SetLeftTop(vec2(673.f, 1906.f));
			}

			while (ui_stack.top() != MENU_TYPE::COMMAND)
			{
				ui_stack.pop();
				ChangeType(ui_stack.top());
			}
		}
	}

	void MenuUIScript::OnEnable()
	{
		while (!ui_stack.empty())
		{
			assert(nullptr);
			ui_stack.pop();
		}

		ui_stack.push(MENU_TYPE::COMMAND);
		ChangePlayer();
		ChangeType(ui_stack.top());
		focused_idx = 0;

		if (portrait)
		{
			portrait->SetActive(true);
		}

		TimeMgr::GetInst()->SetTimeSpeed(0.05f);
		RenderMgr::GetInst()->GetEditorCam()->SetPostProcess(Camera::CameraPostProcess::GRAYSCALE, true);
	}

	void MenuUIScript::OnDisable()
	{
		if (portrait)
		{
			portrait->SetActive(false);
		}
		for (auto select : select_ui)
		{
			select->SetActive(false);
		}
		type = MENU_TYPE::END;

		TimeMgr::GetInst()->SetTimeSpeed(1.0f);
		RenderMgr::GetInst()->GetEditorCam()->SetPostProcess(Camera::CameraPostProcess::GRAYSCALE, false);
	}

	void MenuUIScript::CreateChild()
	{
		portrait = new GameObject;
		portrait->AddComponent(new Transform);
		portrait->AddComponent(new MeshRender);
		portrait->AddComponent(new ImageUIScript);
		portrait->SetLayerIndex(31);

		portrait->GetTransform()->SetRelativePosXY(vec2(100.f, 93.f));
		portrait->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		portrait->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		portrait->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		portrait->SetName(L"Portrait");

		UIScript* _ui = portrait->GetScript<UIScript>();
		_ui->SetSize(vec2(110.f, 45.f), vec2(1.0f, 1.0f));
		_ui->SetLeftTop(vec2(673.f, 1858.f));

		GetOwner()->AddChild(portrait);

		for (int i = 0; i < 5; i++)
		{
			GameObject* _select_ui = new GameObject;
			_select_ui->AddComponent(new Transform);
			_select_ui->AddComponent(new MeshRender);
			_select_ui->AddComponent(new SelectUIScript);
			_select_ui->SetLayerIndex(31);

			_select_ui->GetTransform()->SetRelativePosXY(vec2(20.f, 40.f - i * 40.f));
			_select_ui->SetName(L"Select UI_" + to_wstring(i));

			select_ui.push_back(_select_ui->GetScript<SelectUIScript>());
			GetOwner()->AddChild(_select_ui);
			_select_ui->SetActive(false);
		}
	}

	void MenuUIScript::ChangePlayer()
	{
		player = GameMgr::GetInst()->GetCurPlayer();

		if (player->GetName() == L"Cloud")
		{
			UIScript* _ui = portrait->GetScript<UIScript>();
			_ui->SetLeftTop(vec2(673.f, 1858.f));
		}
		else if (player->GetName() == L"Barret")
		{
			UIScript* _ui = portrait->GetScript<UIScript>();
			_ui->SetLeftTop(vec2(673.f, 1906.f));
		}
	}

	void MenuUIScript::ChangeType(MENU_TYPE _type)
	{
		// 이전 타입과 같을시 종료
		if (type == _type)
		{
			return;
		}

		// select_ui 초기화
		for (auto select : select_ui)
		{
			select->SetActive(false);
			select->SetFocus(false);
		}
		focused_idx = 0;
		active_select_cnt = 0;
		type = _type;

		switch (type)
		{
		case MENU_TYPE::COMMAND:
		{
			select_ui[0]->SetText("ABILITY");
			select_ui[1]->SetText("MAGIC");
			select_ui[2]->SetText("ITEM");
			select_ui[3]->SetText("LIMIT");

			float _limit_gauge = player->GetScript<PlayerDataScript>()->GetLimitGauge();
			if (_limit_gauge < 1.0f)
			{
				select_ui[3]->SetValid(false);
			}

			select_ui[0]->SetActive(true);
			select_ui[1]->SetActive(true);
			select_ui[2]->SetActive(true);
			select_ui[3]->SetActive(true);

			active_select_cnt = 4;
			select_ui[0]->SetFocus(true);
		}
		break;

		case MENU_TYPE::ABILITY: 
		{
			auto& ability_data = player->GetScript<PlayerDataScript>()->GetAbilityData();
			float _atb_gauge = player->GetScript<PlayerDataScript>()->GetAtbGauge();

			active_select_cnt = ability_data.size();
			for (size_t i = 0; i < active_select_cnt; i++)
			{
				if (ability_data[i].cost > _atb_gauge)
				{
					select_ui[i]->SetValid(false);
				}

				select_ui[i]->SetCost(ability_data[i].cost);
				select_ui[i]->SetText(WStrToStr(ability_data[i].name));
				select_ui[i]->SetActive(true);
			}
			if (active_select_cnt > 0)
			{
				select_ui[0]->SetFocus(true);
			}
		}
		break;

		case MENU_TYPE::MAGIC:
		{
			auto& magic_data = player->GetScript<PlayerDataScript>()->GetMagicData();
			float _mp = player->GetScript<PlayerDataScript>()->GetCurMP();

			active_select_cnt = magic_data.size();
			for (size_t i = 0; i < active_select_cnt; i++)
			{
				if (magic_data[i].cost > _mp)
				{
					select_ui[i]->SetValid(false);
				}

				select_ui[i]->SetCost(magic_data[i].cost);
				select_ui[i]->SetText(WStrToStr(magic_data[i].name));
				select_ui[i]->SetActive(true);
			}
			if (active_select_cnt > 0)
			{
				select_ui[0]->SetFocus(true);
			}
		}
		break;

		case MENU_TYPE::ITEM:
		{
			auto& magic_data = player->GetScript<PlayerDataScript>()->GetItemData();
			float _mp = player->GetScript<PlayerDataScript>()->GetCurMP();

			active_select_cnt = magic_data.size();
			for (size_t i = 0; i < active_select_cnt; i++)
			{
				select_ui[i]->SetValid(true);
				select_ui[i]->SetCost(1);
				select_ui[i]->SetText(WStrToStr(magic_data[i].name));
				select_ui[i]->SetActive(true);
			}
			if (active_select_cnt > 0)
			{
				select_ui[0]->SetFocus(true);
			}
		}
		break;

		case MENU_TYPE::LIMIT:
		{
			auto& _limit_data = player->GetScript<PlayerDataScript>()->GetLimitData();

			active_select_cnt = _limit_data.size();
			for (size_t i = 0; i < active_select_cnt; i++)
			{
				select_ui[i]->SetValid(true);
				select_ui[i]->SetCost(1);
				select_ui[i]->SetText(WStrToStr(_limit_data[i].name));
				select_ui[i]->SetActive(true);
			}
			if (active_select_cnt > 0)
			{
				select_ui[0]->SetFocus(true);
			}
		}
		break;

		case MENU_TYPE::TARGET:
		{

			Level* _level = LevelMgr::GetInst()->GetCurLevel();
			Layer* _layer = _level->FindLayerByName(L"Enemy");

			float _max_distance = 100.f;
			for (GameObject* enemy : _layer->GetParentObject())
			{
				if (!enemy->GetActive() && enemy->GetCollider3D() && enemy->GetCollider3D()->IsCollision())
				{
					continue;
				}
				float _distance = vec3::Distance(player->GetTransform()->GetWorldPos(), enemy->GetTransform()->GetWorldPos());

				if (_distance < _max_distance)
				{
					select_ui[active_select_cnt]->SetTarget(enemy);
					select_ui[active_select_cnt]->SetText(WStrToStr(enemy->GetName()));
					select_ui[active_select_cnt]->SetActive(true);
					active_select_cnt++;

					for (GameObject* _obj : enemy->GetChild())
					{
						AddChildTarget(_obj, _level->FindLayerIdxByName(L"Enemy"));
					}
				}
			}
			if (active_select_cnt > 0)
			{
				select_ui[0]->SetFocus(true);
			}
		}
		break;

		default: break;
		}
	}

	void MenuUIScript::PushNextType()
	{
		switch (ui_stack.top())
		{
		case MENU_TYPE::COMMAND:
		{
			if (select_ui[focused_idx]->GetValid())
			{
				MENU_TYPE _type = StrToType(select_ui[focused_idx]->GetText());
				ui_stack.push(_type);
				ChangeType(ui_stack.top());
			}
		}
		break;

		case MENU_TYPE::ABILITY:
		{
			if (select_ui[focused_idx]->GetValid())
			{
				ui_stack.push(MENU_TYPE::TARGET);
				prev_choice = select_ui[focused_idx]->GetText();
				ChangeType(ui_stack.top());
			}
		}
		break;

		case MENU_TYPE::MAGIC:
		{
			if (select_ui[focused_idx]->GetValid())
			{
				ui_stack.push(MENU_TYPE::TARGET);
				prev_choice = select_ui[focused_idx]->GetText();
				ChangeType(ui_stack.top());
			}
		}
		break;

		case MENU_TYPE::ITEM:
		{
			if (select_ui[focused_idx]->GetValid())
			{
				CommandData* _data = new CommandData();
				_data->name = L"Heal";

				PlayerDataScript* _player = player->GetScript<PlayerDataScript>();
				if (_player != nullptr)
				{
					_player->ReciveCommandData(_data);
				}

				while (!ui_stack.empty())
				{
					ui_stack.pop();
				}
				UIMgr::GetInst()->ReturnUI();
			}
		}
		break;

		case MENU_TYPE::LIMIT:
		{
			if (select_ui[focused_idx]->GetValid())
			{
				ui_stack.push(MENU_TYPE::TARGET);
				prev_choice = select_ui[focused_idx]->GetText();
				ChangeType(ui_stack.top());
			}
		}
		break;

		case MENU_TYPE::TARGET:
		{
			DeliverCommandData();
			while (!ui_stack.empty())
			{
				ui_stack.pop();
			}
			UIMgr::GetInst()->ReturnUI();
		}
		break;

		default: break;
		}
	}

	void MenuUIScript::DeliverCommandData()
	{
		SelectUIScript* _select = select_ui[focused_idx];
		if (_select->GetOwner()->GetActive())
		{
			CommandData* _data = new CommandData();
			_data->name = StrToWStr(prev_choice);
			_data->target = _select->GetTarget();
			_data->cost = _select->GetCost();

			ui_stack.pop();
			_data->type = (CommandData::COMMAND_TYPE)((UINT)ui_stack.top() - 1);

			PlayerDataScript* _player = player->GetScript<PlayerDataScript>();
			if (_player != nullptr)
			{
				_player->ReciveCommandData(_data);
			}
		}
	}

	void MenuUIScript::AddChildTarget(GameObject* _obj, int _layer_idx)
	{
		if (active_select_cnt >= select_ui.size() || !_obj->GetActive())
		{
			return;
		}

		if (_layer_idx == _obj->GetLayerIndex() && _obj->GetCollider3D() && _obj->GetCollider3D()->GetCollision())
		{
			float _max_distance = 100.f;
			float _distance = vec3::Distance(player->GetTransform()->GetWorldPos(), _obj->GetTransform()->GetWorldPos());

			if (_distance < _max_distance)
			{
				select_ui[active_select_cnt]->SetTarget(_obj, true);
				select_ui[active_select_cnt]->SetText(WStrToStr(_obj->GetName()));
				select_ui[active_select_cnt]->SetActive(true);
				active_select_cnt++;
			}
		}

		for (GameObject* _obj : _obj->GetChild())
		{
			AddChildTarget(_obj, _layer_idx);
		}
	}

	MenuUIScript::MENU_TYPE MenuUIScript::StrToType(const string& _str)
	{
		if (_str == "COMMAND")		return MENU_TYPE::COMMAND;
		else if (_str == "ABILITY") return MENU_TYPE::ABILITY;
		else if (_str == "MAGIC")	return MENU_TYPE::MAGIC;
		else if (_str == "ITEM")	return MENU_TYPE::ITEM;
		else if (_str == "LIMIT")	return MENU_TYPE::LIMIT;
		else if (_str == "TARGET")	return MENU_TYPE::TARGET;

		return MENU_TYPE::END;
	}

	void MenuUIScript::EnableFunc()
	{
		time_val += TimeMgr::GetInst()->GetDeltaTime();
		if (time_val > GetEnableTime())
		{
			time_val = 0.0f;
			SetUIFunc(UI_FUNC::TICK);
		}
	}
	void MenuUIScript::DisableFunc()
	{
		time_val += TimeMgr::GetInst()->GetDeltaTime();
		if (time_val > GetEnableTime())
		{
			time_val = 0.0f;
			GetOwner()->SetActive(false);
		}
	}
	void MenuUIScript::TickFunc()
	{
		/*if (KEY_PRESSED(KEY::ESC))
		{
			SetUIFunc(UI_FUNC::RETURN);
			return;
		}
		else if (KEY_PRESSED(KEY::SPACE))
		{
 			if (childs.size() > 0)
			{
				wstring _target = childs[focused_idx]->GetTarget();
				if (_target == L"")
				{
					SetUIFunc(UI_FUNC::RESET);
					return;
				}
				else
				{ 
					SetUIFunc(UI_FUNC::SELECT);
					return;
				}
			}
		}
		else if (KEY_PRESSED(KEY::TAB))
		{
			SetUIFunc(UI_FUNC::CALL);
			return;
		}
		const vector<GameObject*>& _childs = GetOwner()->GetChild();

		if (_childs.size() > 0)
		{
			if (KEY_PRESSED(KEY::DOWN))
			{
				focused_idx = (focused_idx + 1) < _childs.size() ? focused_idx + 1 : focused_idx;
			}
			else if (KEY_PRESSED(KEY::UP))
			{
				focused_idx = (focused_idx - 1) >= 0 ? focused_idx - 1 : focused_idx;
			}
		}*/
	}

	void MenuUIScript::CallFunc()
	{
		time_val += TimeMgr::GetInst()->GetDeltaTime();
		if (time_val > GetEnableTime())
		{
			time_val = 0.0f;
			if (GetTarget() == L"")
			{
				SetUIFunc(UI_FUNC::TICK);
			}
			UIMgr::GetInst()->CallUI(GetTarget());
		}
	}
	void MenuUIScript::SelectFunc()
	{
		//time_val += TimeMgr::GetInst()->GetDeltaTime();
		//if (time_val > GetEnableTime())
		//{
		//	time_val = 0.0f;
		//	UIMgr::GetInst()->CallUI(childs[focused_idx]->GetTarget());
		//}
	}

	void MenuUIScript::ResetFunc()
	{
		int a = 0;
	}

	void MenuUIScript::ReturnFunc()
	{
		time_val += TimeMgr::GetInst()->GetDeltaTime();
		if (time_val > GetEnableTime())
		{
			time_val = 0.0f;
			UIMgr::GetInst()->ReturnUI();
		}
	}

	void MenuUIScript::UpdateChilds()
	{
		/*childs.clear();
		const vector<GameObject*>& _child_objects = GetOwner()->GetChild();
		for (int i = 0; i < _child_objects.size(); i++)
		{
			UIScript* _child = _child_objects[i]->GetScript<UIScript>();
			if (_child != nullptr)
			{
				childs.push_back(_child);
			}
		}*/
	}


}
