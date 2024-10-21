#include "pch.h"
#include "EventMgr.h"

#include "LevelMgr.h"
#include "Level.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "RenderMgr.h"
#include "Prefab.h"

namespace ff7r
{
	EventMgr::EventMgr()
		: is_level_changed(false)
	{

	}

	EventMgr::~EventMgr()
	{

	}

	void EventMgr::Tick()
	{
		is_level_changed = false;

		GC_Clear();

		for (size_t i = 0; i < events.size(); ++i)
		{
			switch (events[i].type)
			{
				// wParam : GameObject, lParam : Layer Index
			case EVENT_TYPE::CREATE_OBJECT:
			{
				GameObject* NewObject = (GameObject*)events[i].w_param;
				int iLayerIdx = (int)events[i].l_param;

				//LevelMgr::GetInst()->GetCurLevel()->AddGameObject(NewObject, iLayerIdx, false);
				LevelMgr::GetInst()->GetLevel(0)->AddGameObject(NewObject, iLayerIdx, false);

				if (LevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
				{
					NewObject->Begin();
				}

				is_level_changed = true;
			}
			break;
			case EVENT_TYPE::DELETE_OBJECT:
			{
				GameObject* DeleteObject = (GameObject*)events[i].w_param;

				if (false == DeleteObject->is_dead)
				{
					DeleteObject->is_dead = true;
					delete_objects.push_back(DeleteObject);
				}
			}
			break;
			case EVENT_TYPE::CREATE_PREFAB:
			{
				//Prefab* NewPrefab = new Prefab();
				//NewPrefab->RegisterProtoObject((GameObject*)events[i].w_param);

				wstring* _prefab_name = (wstring*)events[i].l_param;

				ResourceMgr::GetInst()->CreatePrefab(*_prefab_name, (GameObject*)events[i].w_param);
			}
			break;
			case EVENT_TYPE::ADD_CHILD:
				// wParam : ParentObject, lParam : ChildObject
			{
				GameObject* pDestObj = (GameObject*)events[i].w_param;
				GameObject* pSrcObj = (GameObject*)events[i].l_param;

				// 부모로 지정된 오브젝트가 없으면, Child 오브젝트가 최상위 부모 오브젝트가 된다.
				if (nullptr == pDestObj)
				{
					if (pSrcObj->GetParent())
					{
						// 기존 부모와의 연결 해제
						pSrcObj->DisconnectFromParent();
						// 최상위 부모 오브젝트로, 소속 레이어에 등록
						pSrcObj->AddParentList();
					}
				}
				else
				{
					pDestObj->AddChild(pSrcObj);
				}

				is_level_changed = true;
			}



			break;
			case EVENT_TYPE::DELETE_RESOURCE:
				// wParam : RES_TYPE, lParam : Resource Adress
			{
				RES_TYPE type = (RES_TYPE)events[i].w_param;
				Resource* pRes = (Resource*)events[i].l_param;
				ResourceMgr::GetInst()->DeleteRes(type, pRes->GetKey());
			}

			break;
			case EVENT_TYPE::LEVEL_CHANGE:
			{
				// wParam : Level Adress
				Level* _level = (Level*)events[i].w_param;
				LevelMgr::GetInst()->ChangeLevel(_level);
				//RenderMgr::GetInst()->ClearCamera();
				is_level_changed = true;
			}
			break;

			case EVENT_TYPE::LEVLE_CHANGE_ANOTHER:
			{
				// wParam : Level Adress
				Level* _level = (Level*)events[i].w_param;
				LevelMgr::GetInst()->ChangeLevelNotDelete(_level);
				// RenderMgr::GetInst()->ClearCamera();
				is_level_changed = true;
			}
			break;
			}
		}

		events.clear();
	}


	void EventMgr::GC_Clear()
	{
		for (size_t i = 0; i < delete_objects.size(); ++i)
		{
			if (nullptr != delete_objects[i])
			{
				// 자식 타입 오브젝트인 경우
				if (delete_objects[i]->GetParent())
					delete_objects[i]->DisconnectFromParent();

				delete delete_objects[i];

				is_level_changed = true;
			}
		}
		delete_objects.clear();
	}
}