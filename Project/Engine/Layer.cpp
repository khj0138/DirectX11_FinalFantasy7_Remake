#include "pch.h"
#include "Layer.h"

#include "GameObject.h"
#include "LevelMgr.h"
namespace ff7r
{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		for (auto iter = parent_objects.begin(); iter != parent_objects.end();)
		{
			if (!(*iter)->GetDestroy())
			{
				LevelMgr::GetInst()->SaveObject(*iter);
				iter = parent_objects.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		SafeDelVector(parent_objects);
	}

	void Layer::Begin()
	{
		for (size_t i = 0; i < parent_objects.size(); ++i)
		{
			parent_objects[i]->Begin();
		}
	}

	void Layer::Tick()
	{
		for (size_t i = 0; i < parent_objects.size(); ++i)
		{
			parent_objects[i]->Tick();
		}
	}

	void Layer::FinalTick()
	{
		vector<GameObject*>::iterator iter = parent_objects.begin();
		for (; iter != parent_objects.end(); )
		{
			(*iter)->FinalTick();

			if ((*iter)->IsDead())
			{
				iter = parent_objects.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	void Layer::AddGameObject(GameObject* _Object, bool _bMove)
	{
		parent_objects.push_back(_Object);

		// 소유하고 있는 모든 자식오브젝트가 있는지 검사
		static list<GameObject*> queue;
		queue.clear();

		queue.push_back(_Object);

		while (!queue.empty())
		{
			GameObject* _object = queue.front();
			queue.pop_front();

			for (size_t i = 0; i < _object->child.size(); ++i)
			{
				queue.push_back(_object->child[i]);
			}

			// 부모타입 or 소속 레이어가 없는경우 or 부모와 같이 이동하는 경우
			if (nullptr == _object->parent || -1 == _object->layer_idx || _bMove)
				_object->layer_idx = layer_idx;
		}

		_Object->Awake();
	}


	void Layer::RemoveFromParentList(GameObject* _Obj)
	{
		vector<GameObject*>::iterator iter = parent_objects.begin();

		for (; iter != parent_objects.end(); ++iter)
		{
			if ((*iter) == _Obj)
			{
				parent_objects.erase(iter);
				return;
			}
		}

		// assert(nullptr);
	}

	void Layer::AddParentList(GameObject* _Obj)
	{
		assert(!_Obj->parent);
		parent_objects.push_back(_Obj);
	}
}