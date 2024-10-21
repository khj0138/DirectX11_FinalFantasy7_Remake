#include "pch.h"

#include <Engine\GameObject.h>

#include "ObjectPoolMgr.h"
namespace ff7r
{
	map<wstring, vector<GameObject*>> ObjectPoolMgr::g_object_pool = {};
	ObjectPoolMgr* ObjectPoolMgr::g_inst = nullptr;

	ObjectPoolMgr::ObjectPoolMgr()
		: Script(SCRIPT_TYPE::OBJECTPOOLMGR)
	{
		if (g_inst)
		{
			delete g_inst;
			g_inst = nullptr;
		}

		g_inst = this;
	}

	ObjectPoolMgr::~ObjectPoolMgr()
	{
	}

	void ObjectPoolMgr::Awake()
	{

	}

	void ObjectPoolMgr::RegisterObject(GameObject* _obj, wstring _key, int i)
	{
		vector<GameObject*> object;
		if (g_object_pool.find(_key) == g_object_pool.end())
		{
			object.push_back(_obj);
			g_object_pool.insert({ _key, object });

			//_obj->Awake();
			_obj->SetActive(false);
			GetOwner()->AddChild(_obj);
		}

		for (int j = 0; j < i; j++)
		{
			GameObject* new_obj = _obj->Clone();
			object.push_back(new_obj);
			GetOwner()->AddChild(new_obj);

			new_obj->Awake();
			new_obj->SetActive(false);
			new_obj->SetLayerIndex(_obj->GetLayerIndex());
		}
	}

	GameObject* ObjectPoolMgr::GetGameObject(wstring _key)
	{
		if (g_object_pool.find(_key) != g_object_pool.end())
		{
			vector<GameObject*>& objects = g_object_pool.find(_key)->second;

			for (size_t i = 1; i < objects.size(); i++)
			{
				if (!objects[i]->GetActive())
				{
					objects[i]->SetActive(true);
					return objects[i];
				}
			}

			GameObject* new_obj = objects[0]->Clone();
			objects.push_back(new_obj);
			GetOwner()->AddChild(new_obj);

			new_obj->Awake();
			new_obj->SetActive(false);
			new_obj->SetActive(true);
			new_obj->SetLayerIndex(objects[0]->GetLayerIndex());

			return new_obj;
		}

		return nullptr;
	}

}