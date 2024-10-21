#include "pch.h"

#include "Script\ScriptMgr.h"

#include "Script.h"
#include "GameObject.h"
#include "Component.h"
#include "PathMgr.h"

#include "Prefab.h"
namespace ff7r
{
	Prefab::Prefab()
		: Resource(RES_TYPE::PREFAB)
		, proto_obj(nullptr)
		, layer_idx(-1)
	{
	}

	Prefab::~Prefab()
	{
		if (nullptr != proto_obj)
		{
			delete proto_obj;
			proto_obj = nullptr;
		}
	}

	GameObject* Prefab::Instantiate()
	{
		if (proto_obj->GetLayerIndex() == -1)
		{
			layer_idx = 0;
		}
		else
		{
			layer_idx = proto_obj->GetLayerIndex();
		}

		GameObject* _clone_object = proto_obj->Clone();
		_clone_object->SetLayerIndex(layer_idx);
		SpawnGameObject(_clone_object, vec3(0.f, 0.f, 0.0f), layer_idx);
		return _clone_object;
	}

	void Prefab::RegisterProtoObject(GameObject* _Proto)
	{
		// 원본 오브젝트는 레벨 소속이 아니여야 한다.
		//assert(-1 == _Proto->GetLayerIndex());
		if (_Proto == nullptr)
		{
			proto_obj = new GameObject();
		}
		else
		{
			proto_obj = new GameObject(*_Proto);
			layer_idx = _Proto->GetLayerIndex();
		}
	}

	int Prefab::Save(const wstring& _relative_path)
	{
		if (!proto_obj)
		{
			return E_FAIL;
		}

		wstring _path = PathMgr::GetInst()->GetContentPath();
		_path += GetKey();

		FILE* _file = nullptr;

		_wfopen_s(&_file, _path.c_str(), L"wb");

		if (_file)
		{
			SaveObject(proto_obj, _file);

			fclose(_file);

			return S_OK;
		}

		return E_FAIL;
	}

	void Prefab::SaveObject(GameObject* _object, FILE* _file)
	{
		SaveWString(_object->GetName(), _file);

		int _layer_idx = _object->GetLayerIndex();

		if (_layer_idx == -1)
		{
			_layer_idx = 0;
		}

		wstring _layer_name = LevelMgr::GetInst()->GetCurLevel()->FindLayerNameByIdx(_layer_idx);
		SaveWString(_layer_name, _file);

		for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (i == (UINT)COMPONENT_TYPE::END)
			{
				fwrite(&i, sizeof(UINT), 1, _file);
				break;
			}

			Component* _component = _object->GetComponent((COMPONENT_TYPE)i);
			if (nullptr == _component)
			{
				continue;
			}

			fwrite(&i, sizeof(UINT), 1, _file);

			_component->SaveToLevelFile(_file);
		}

		const vector<Script*>& _scripts = _object->GetScripts();
		size_t _script_cnt = _scripts.size();
		fwrite(&_script_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < _scripts.size(); ++i)
		{
			wstring _script_name = ScriptMgr::GetScriptName(_scripts[i]);
			SaveWString(_script_name, _file);
			_scripts[i]->SaveToLevelFile(_file);
		}

		const vector<GameObject*>& _child = _object->GetChild();
		size_t _child_cnt = _child.size();
		fwrite(&_child_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < _child_cnt; ++i)
		{
			SaveObject(_child[i], _file);
		}
	}


	int Prefab::Load(const wstring& _file_path)
	{
		wstring _path = _file_path;

		FILE* _file = nullptr;

		_wfopen_s(&_file, _path.c_str(), L"rb");

		if (nullptr == _file)
		{
			return E_FAIL;
		}

		proto_obj = LoadObject(_file);

		fclose(_file);

		return S_OK;
	}

	GameObject* Prefab::LoadObject(FILE* _file)
	{
		GameObject* _object = new GameObject;

		wstring _name;
		LoadWString(_name, _file);
		_object->SetName(_name);

		wstring _layer_name;
		LoadWString(_layer_name, _file);
		layer_idx = LevelMgr::GetInst()->GetCurLevel()->FindLayerIdxByName(_layer_name);
		_object->SetLayerIndex(layer_idx);

		while (true)
		{
			UINT _comp_type = 0;
			fread(&_comp_type, sizeof(UINT), 1, _file);

			if ((UINT)COMPONENT_TYPE::END == _comp_type)
			{
				break;
			}

			Component* _component = nullptr;

			switch ((COMPONENT_TYPE)_comp_type)
			{
			case COMPONENT_TYPE::TRANSFORM:			_component = new Transform;			break;
			case COMPONENT_TYPE::COLLIDER2D:		_component = new Collider2D;		break;
			case COMPONENT_TYPE::COLLIDER3D:		_component = new Collider3D;		break;
			case COMPONENT_TYPE::ANIMATOR2D:		_component = new Animator2D;		break;
			case COMPONENT_TYPE::ANIMATOR3D:		_component = new Animator3D;		break;
			case COMPONENT_TYPE::LIGHT2D:			_component = new Light2D;			break;
			case COMPONENT_TYPE::LIGHT3D:			_component = new Light3D;			break;
			case COMPONENT_TYPE::CAMERA:			_component = new Camera;			break;
			case COMPONENT_TYPE::REFLECTIONCAPTURE: _component = new ReflectionCapture; break;
			case COMPONENT_TYPE::SOCKET:			_component = new Socket;			break;
			case COMPONENT_TYPE::MESHRENDER:		_component = new MeshRender;		break;
			case COMPONENT_TYPE::PARTICLESYSTEM:	_component = new ParticleSystem;	break;
			case COMPONENT_TYPE::SKYBOX:			_component = new SkyBox;			break;
			default: break;
			}

			if (_component == nullptr)
			{
				continue;
			}

			_component->LoadFromLevelFile(_file);
			_object->AddComponent(_component);
		}

		size_t _script_cnt = 0;
		fread(&_script_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < _script_cnt; ++i)
		{
			wstring _script_name;
			LoadWString(_script_name, _file);
			Script* _script = ScriptMgr::GetScript(_script_name);
			_object->AddComponent(_script);
			_script->LoadFromLevelFile(_file);
		}

		size_t _child_cnt = 0;
		fread(&_child_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < _child_cnt; ++i)
		{
			GameObject* _child = LoadObject(_file);
			_object->AddChild(_child);
		}

		return _object;
	}
}