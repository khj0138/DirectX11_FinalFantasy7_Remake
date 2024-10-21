#include "pch.h"
#include "InspectorUI.h"

#include <Engine\GameObject.h>
#include <Engine\Transform.h>
#include <Engine\LevelMgr.h>
#include <Engine\KeyMgr.h>

#include <Script\ScriptMgr.h>

#include "ImGuiMgr.h"
#include "OutlinerUI.h"

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Collider3DUI.h"
#include "CameraUI.h"
#include "Animator2DUI.h"
#include "Animator3DUI.h"
#include "TileMapUI.h"
#include "Light2DUI.h"
#include "CaptureUI.h"
#include "Light3DUI.h"

#include "MeshDataUI.h"
#include "TextureUI.h"
#include "MeshUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MaterialUI.h"
#include "ScriptUI.h"
#include "ButtonUI.h"
#include "ListUI.h"


namespace ff7r
{

	InspectorUI::InspectorUI()
		: UI("##Inspector")
		, target_object(nullptr)
		, component_UIs{}
		, resource_UIs{}
	{
		SetName("Inspector");

		component_UIs[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
		component_UIs[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::TRANSFORM]);

		component_UIs[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
		component_UIs[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::MESHRENDER]);

		component_UIs[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
		component_UIs[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::CAMERA]);

		component_UIs[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
		component_UIs[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::COLLIDER2D]);

		component_UIs[(UINT)COMPONENT_TYPE::COLLIDER3D] = new Collider3DUI;
		component_UIs[(UINT)COMPONENT_TYPE::COLLIDER3D]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::COLLIDER3D]);

		component_UIs[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
		component_UIs[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

		component_UIs[(UINT)COMPONENT_TYPE::ANIMATOR3D] = new Animator3DUI;
		component_UIs[(UINT)COMPONENT_TYPE::ANIMATOR3D]->SetSize(0.f,250.0f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::ANIMATOR3D]);

		component_UIs[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
		component_UIs[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::LIGHT2D]);

		component_UIs[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
		component_UIs[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::TILEMAP]);

		component_UIs[(UINT)COMPONENT_TYPE::REFLECTIONCAPTURE] = new CaptureUI;
		component_UIs[(UINT)COMPONENT_TYPE::REFLECTIONCAPTURE]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::REFLECTIONCAPTURE]);

		component_UIs[(UINT)COMPONENT_TYPE::LIGHT3D] = new Light3DUI;
		component_UIs[(UINT)COMPONENT_TYPE::LIGHT3D]->SetSize(0.f, 150.f);
		AddChildUI(component_UIs[(UINT)COMPONENT_TYPE::LIGHT3D]);

		// ResUI
		resource_UIs[(UINT)RES_TYPE::MESHDATA] = new MeshDataUI;
		resource_UIs[(UINT)RES_TYPE::MESHDATA]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::MESHDATA]);

		resource_UIs[(UINT)RES_TYPE::MESH] = new MeshUI;
		resource_UIs[(UINT)RES_TYPE::MESH]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::MESH]);

		resource_UIs[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
		resource_UIs[(UINT)RES_TYPE::TEXTURE]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::TEXTURE]);

		resource_UIs[(UINT)RES_TYPE::GRAPHICS_SHADER] = new GraphicsShaderUI;
		resource_UIs[(UINT)RES_TYPE::GRAPHICS_SHADER]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::GRAPHICS_SHADER]);

		resource_UIs[(UINT)RES_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
		resource_UIs[(UINT)RES_TYPE::COMPUTE_SHADER]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::COMPUTE_SHADER]);

		resource_UIs[(UINT)RES_TYPE::PREFAB] = new PrefabUI;
		resource_UIs[(UINT)RES_TYPE::PREFAB]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::PREFAB]);

		resource_UIs[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
		resource_UIs[(UINT)RES_TYPE::MATERIAL]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::MATERIAL]);

		resource_UIs[(UINT)RES_TYPE::SOUND] = new SoundUI;
		resource_UIs[(UINT)RES_TYPE::SOUND]->SetSize(0.f, 0.f);
		AddChildUI(resource_UIs[(UINT)RES_TYPE::SOUND]);

		add_comp_button = new ButtonUI("##Add Component");
		add_comp_button->SetSize(200.0f, 500.0f);
		add_comp_button->SetName("Add Component");
		add_comp_button->AddDynamic_Button(this, (UI_DELEGATE)&InspectorUI::ShowCompList);
		add_comp_button->SetActive(false);
		AddChildUI(add_comp_button);

		component_list = new ListUI;
		component_list->SetName("Component");
		component_list->SetSize(300.0f, 300.0f);
		component_list->SetActive(false);
		component_list->AddDynamic_Select(this, (UI_DELEGATE_1)&InspectorUI::AddComponent);
		component_list->SetPopup(true);
		component_list->SetFilter(true);
		add_comp_button->AddChildUI(component_list);
	}

	InspectorUI::~InspectorUI()
	{

	}

	void InspectorUI::Init()
	{
		SetTargetObject(nullptr);
	}

	void InspectorUI::Tick()
	{

	}

	int InspectorUI::RenderUpdate()
	{

		return TRUE;
	}

	void InspectorUI::SetTargetObject(GameObject* _target)
	{
		ClearTargetResource();
		component_list->Clear();

		// 타겟오브젝트 정보 노출
		target_object = _target;

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == component_UIs[i])
				continue;

			else
			{
				if (!(component_UIs[i]->SetTarget(target_object)))
					component_list->AddItem(COMPONENT_TYPE_STR[i]);

				//comp_list.push_back(COMPONENT_TYPE_STR[i]);
				//component_list->AddItem(std::to_string(i));
			}
		}

		// 타겟 오브젝트가 nullptr 이면
		// 스크립트UI 들을 전부 비활성화 시킨다.
		if (nullptr == target_object)
		{
			for (size_t i = 0; i < script_UIs.size(); ++i)
			{
				script_UIs[i]->SetActive(false);
			}
			return;
		}

		vector<wstring> _script_name_list;

		ScriptMgr::GetScriptInfo(_script_name_list);

		// 오브젝트의 스크립트 목록을 받아온다.
		const vector<Script*>& _vec_script = target_object->GetScripts();

		for (auto script : _vec_script)
		{
			for (size_t j = 0; j < _script_name_list.size(); j++)
			{
				if (script->GetScriptType() == j)
				{
					_script_name_list[j] = L"";
					break;
				}
			}
		}

		// 스크립트UI 가 스크립트 수 보다 적으면 그만큼 추가해준다.
		if (script_UIs.size() < _vec_script.size())
		{
			UINT iDiffer = _vec_script.size() - script_UIs.size();
			DeleteChildUI(add_comp_button);
			for (UINT i = 0; i < iDiffer; ++i)
			{
				ScriptUI* UI = new ScriptUI;
				UI->SetSize(0.0f, 150.0f);
				script_UIs.push_back(UI);
				AddChildUI(UI);
				UI->SetActive(true);
			}
			AddChildUI(add_comp_button);
		}

		// ScriptUI 반복문 돌면서 오브젝트의 스크립트수 만큼만 활성화 시킨다.
		for (size_t i = 0; i < script_UIs.size(); ++i)
		{
			if (_vec_script.size() <= i)
			{
				script_UIs[i]->SetActive(false);
				continue;
			}

			// 스크립트를 스크립트UI 에게 알려준다.
			script_UIs[i]->SetTarget(target_object);
			script_UIs[i]->SetScript(_vec_script[i]);
			script_UIs[i]->SetActive(true);

			for (size_t j = 0; j < _script_name_list.size(); j++)
			{
				if (_vec_script[i]->GetScriptType() == j)
				{
					_script_name_list[j] = L"";
					break;
				}
			}
		}

		for (auto script_name : _script_name_list)
		{
			if (script_name == L"")
				continue;
			string _temp;
			component_list->AddItem(_temp.assign(script_name.begin(), script_name.end()));
		}

		add_comp_button->SetActive(true);
	}

	void InspectorUI::SetTargetResource(Ptr<Resource> _Res)
	{
		ClearTargetObject();

		for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
		{
			if (resource_UIs[i])
			{
				resource_UIs[i]->SetActive(false);
			}
		}

		target_resource = _Res;

		if (nullptr == target_resource)
			return;

		RES_TYPE type = _Res->GetType();

		resource_UIs[(UINT)type]->SetActive(true);
		resource_UIs[(UINT)type]->SetTargetRes(_Res);
	}
	void InspectorUI::AddComponent(COMPONENT_TYPE _type)
	{
	}

	void InspectorUI::AddComponent(DWORD_PTR _type)
	{
		component_list->SetActive(false);
		const char* _type_name = (char*)_type;
		//string type = std::to_string(_type);
		COMPONENT_TYPE _comp_type = COMPONENT_TYPE::END;
		for (int i = 0; i < (int)COMPONENT_TYPE::END; i++)
		{
			int _comp_size = strlen(_type_name) < strlen(COMPONENT_TYPE_STR[i]) ? strlen(_type_name) : strlen(COMPONENT_TYPE_STR[i]);

			if (std::strncmp(COMPONENT_TYPE_STR[i], _type_name, _comp_size) == 0)
			{
				_comp_type = (COMPONENT_TYPE)i;
				break;
			}
		}
		if (_comp_type == COMPONENT_TYPE::END)
		{
			wstring _script_name = wstring(_type_name, &_type_name[strlen(_type_name)]);
			AddScript(_script_name);
			return;
		}
		OutlinerUI* _outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
		InspectorUI* _inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

		// 선택된 오브젝트를 가져와서 ComponentType 에 맞는 컴포넌트를 생성해서 추가한다.
		GameObject* _selected_object = _outliner->GetSelectedObject();

		if (nullptr == _selected_object)
			return;

		switch (_comp_type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			_selected_object->AddComponent(new Transform);
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			_selected_object->AddComponent(new Collider2D);
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			_selected_object->AddComponent(new Collider3D);
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			_selected_object->AddComponent(new Animator2D);
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			_selected_object->AddComponent(new Animator3D);
			break;
		case COMPONENT_TYPE::LIGHT2D:
			_selected_object->AddComponent(new Light2D);
			break;
		case COMPONENT_TYPE::LIGHT3D:
			_selected_object->AddComponent(new Light3D);
			break;
		case COMPONENT_TYPE::CAMERA:
			_selected_object->AddComponent(new Camera);
			break;
		case COMPONENT_TYPE::MESHRENDER:
			_selected_object->AddComponent(new MeshRender);
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			_selected_object->AddComponent(new ParticleSystem);
			break;
		case COMPONENT_TYPE::TILEMAP:
			_selected_object->AddComponent(new TileMap);
			break;
		case COMPONENT_TYPE::REFLECTIONCAPTURE:
			_selected_object->AddComponent(new ReflectionCapture);
			break;
		case COMPONENT_TYPE::DECAL:
			//_selected_object->AddComponent(new CDecal);
			break;
		}

		// Inspector 에 새롭게 추가된 컴포넌트를 알리기 위해서 타겟을 다시 알려준다.
		_inspector->SetTargetObject(_selected_object);
	}

	void InspectorUI::AddScript(const wstring& _script_name)
	{
		// Outliner 와 Inspector 를 가져온다.
		OutlinerUI* _outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
		InspectorUI* _inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

		// 선택된 오브젝트를 가져와서 ComponentType 에 맞는 컴포넌트를 생성해서 추가한다.
		GameObject* _selected_object = _outliner->GetSelectedObject();

		if (nullptr == _selected_object)
			return;

		Script* _script = ScriptMgr::GetScript(_script_name);
		if (_script == nullptr)
		{
			return;
		}
		_selected_object->AddComponent(_script);

		_inspector->SetTargetObject(_selected_object);
	}

	void InspectorUI::ClearTargetObject()
	{
		// 타겟오브젝트 정보 노출
		target_object = nullptr;
		add_comp_button->SetActive(false);

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == component_UIs[i])
				continue;

			component_UIs[i]->SetTarget(nullptr);
		}
	}

	void InspectorUI::ClearTargetResource()
	{
		target_resource = nullptr;

		for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
		{
			if (nullptr != resource_UIs[i])
			{
				resource_UIs[i]->SetTargetRes(nullptr);
				resource_UIs[i]->SetActive(false);
			}
		}
	}
	void InspectorUI::ShowCompList()
	{
		component_list->SetActive(true);
	}
}