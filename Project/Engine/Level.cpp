#include "pch.h"
#include "Level.h"

#include "Layer.h"
#include "GameObject.h"

#include "RenderMgr.h"

namespace ff7r
{
	Level::Level()
		: layers{}
		, state(LEVEL_STATE::STOP)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i] = new Layer;
			layers[i]->layer_idx = i;
		}

		GetLayer(0)->SetName(L"Default");
		GetLayer(1)->SetName(L"Tile");
		GetLayer(2)->SetName(L"Player");
		GetLayer(3)->SetName(L"Enemy");
		GetLayer(4)->SetName(L"PlayerProjectile");
		GetLayer(5)->SetName(L"EnemyProjectile");
		GetLayer(6)->SetName(L"Skybox");
		GetLayer(7)->SetName(L"Weapon");
		GetLayer(8)->SetName(L"Effect");
		GetLayer(9)->SetName(L"Player Range");
		GetLayer(10)->SetName(L"Manager");
		GetLayer(11)->SetName(L"Wall");
		GetLayer(13)->SetName(L"InteractObject");
		GetLayer(30)->SetName(L"Capture");
		GetLayer(31)->SetName(L"ViewPort UI");

		
	}

	Level::~Level()
	{
		SafeDelArray(layers);
	}


	void Level::Begin()
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->Begin();
		}
	}

	void Level::Tick()
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->Tick();
		}
	}

	void Level::FinalTick()
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->FinalTick();
		}
	}

	void Level::Test()
	{
		
		{
			delete layers[1];
			layers[1] = new Layer;
			layers[1]->layer_idx = 1;
			layers[1]->SetName(L"Tile");
		}
	}

	Layer* Level::FindLayerByName(const wstring& _strName)
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (layers[i]->GetName() == _strName)
				return layers[i];
		}

		return nullptr;
	}

	int Level::FindLayerIdxByName(const wstring& _name)
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (layers[i]->GetName() == _name)
			{
				return i;
			}
		}

		return MAX_LAYER;
	}

	wstring Level::FindLayerNameByIdx(int _idx)
	{
		return layers[_idx]->GetName();
	}


	void Level::AddGameObject(GameObject* _Object, int _iLayerIdx, bool _bMove)
	{
		if (state == LEVEL_STATE::PAUSE)
		{
			spawn_objects.push_back({ _iLayerIdx , _Object ,_bMove });
		}
		else
		{
		layers[_iLayerIdx]->AddGameObject(_Object, _bMove);

		}
	}

	void Level::AddGameObject(GameObject* _Object, const wstring& _LayerName, bool _Move)
	{
		Layer* pLayer = FindLayerByName(_LayerName);
		assert(pLayer);

		pLayer->AddGameObject(_Object, _Move);
	}

	void Level::MoveGameObject(GameObject* _object)
	{
		layers[_object->GetLayerIndex()]->AddParentList(_object);
	}

	void Level::ChangeState(LEVEL_STATE _State)
	{
		if (state == LEVEL_STATE::PAUSE && _State != LEVEL_STATE::PAUSE)
		{
			for (auto spawn : spawn_objects)
			{
				layers[std::get<0>(spawn)]->AddGameObject(std::get<1>(spawn), std::get<2>(spawn));
			}
		}
		spawn_objects.clear();

		state = _State;


		if (LEVEL_STATE::PLAY == state)
		{
			RenderMgr::GetInst()->SetRenderFunc(true);
			Begin();
		} 
		else if(LEVEL_STATE::STOP == state)
		{
			RenderMgr::GetInst()->SetRenderFunc(false);
		}
	}

	void Level::SetDestroy()
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (layers[i])
			{
				vector<GameObject*> _gameObjects = layers[i]->GetParentObject();
				for (int j = 0; j < _gameObjects.size(); j++)
				{
					_gameObjects[j]->SetDestroy(true);
				}
			}
		}
	}


	void Level::Clear()
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->objects.clear();
		}
	}
	GameObject* Level::FindParentObjectByName(const wstring& _Name)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			const vector<GameObject*>& vecObjects = layers[i]->GetParentObject();
			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				if (_Name == vecObjects[j]->GetName())
					return vecObjects[j];
			}
		}

		return nullptr;
	}

	void Level::FindParentObjectByName(const wstring& _Name, vector<GameObject*>& _Out)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			const vector<GameObject*>& vecObjects = layers[i]->GetParentObject();
			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				if (_Name == vecObjects[j]->GetName())
					_Out.push_back(vecObjects[j]);
			}
		}
	}

	GameObject* Level::FindObjectByName(const wstring& _Name)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			const vector<GameObject*>& vecObjects = layers[i]->GetObjects();
			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				if (_Name == vecObjects[j]->GetName())
					return vecObjects[j];
			}
		}

		return nullptr;
	}

	void Level::FindObjectByName(const wstring& _Name, vector<GameObject*>& _Out)
	{
			for (UINT i = 0; i < MAX_LAYER; ++i)
			{
				const vector<GameObject*>& vecObjects = layers[i]->GetParentObject();
				for (size_t j = 0; j < vecObjects.size(); ++j)
				{
					if (_Name == vecObjects[j]->GetName())
						_Out.push_back(vecObjects[j]);
					FindObjectByNameT(_Name, _Out, vecObjects[j]);
	

				}
			}

	}
	void Level::FindObjectByNameT(const wstring& _name, vector<GameObject*>& _result, GameObject* _t)
	{
		for (int i = 0; i < _t->GetChild().size(); i++)
		{
			if (_name == _t->GetChild()[i]->GetName())
				_result.push_back(_t->GetChild()[i]);

			FindObjectByNameT(_name, _result, _t->GetChild()[i]);
		}
	}
	//void Level::EraseObjectByName(const wstring& _name)
	//{
	//}
}