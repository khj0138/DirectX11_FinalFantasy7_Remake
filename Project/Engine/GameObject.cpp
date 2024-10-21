#include "pch.h"
#include "GameObject.h"

#include "Component.h"
#include "MeshRender.h"
#include "Collider3D.h"

#include "Script.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"
#include "PhysXMgr.h"

#include "RenderComponent.h"
#include "EventMgr.h"


namespace ff7r
{
	GameObject::GameObject()
		: components{}
		, render_component(nullptr)
		, parent(nullptr)
		, layer_idx(-1)
		, is_dead(false)
		, life_time(0.f)
		, cur_life_time(0.f)
		, is_life_span(false)
		, is_active(true)
		, is_first_tick(true)
		, is_destroy(true)
	{
	}

	GameObject::GameObject(const GameObject& _Other)
		: Entity(_Other)
		, components{}
		, render_component(nullptr)
		, parent(nullptr)
		, layer_idx(_Other.layer_idx)
		, is_dead(false)
		, life_time(0.f)
		, cur_life_time(0.f)
		, is_life_span(_Other.is_life_span)
		, is_active(_Other.is_active)
		, is_first_tick(true)
		, is_destroy(true)
	{
		// Component 복사
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (_Other.components[i])
			{
				AddComponent(_Other.components[i]->Clone());
			}
		}

		// Script 복사
		for (size_t i = 0; i < _Other.scripts.size(); ++i)
		{
			AddComponent(_Other.scripts[i]->Clone());
		}

		// 자식 오브젝트 복사
		for (size_t i = 0; i < _Other.child.size(); ++i)
		{
			AddChild(_Other.child[i]->Clone());
		}
	}

	GameObject::~GameObject()
	{
		SafeDelArray(components);
		SafeDelVector(scripts);
		SafeDelVector(child);
	}

	void GameObject::Awake()
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr != components[i])
				components[i]->Awake();
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->Awake();
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->Awake();
		}
	}

	void GameObject::Start()
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr != components[i])
				components[i]->Start();
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->Start();
		}

		//for (size_t i = 0; i < child.size(); ++i)
		//{
		//	child[i]->Start();
		//}
	}

	void GameObject::Begin()
	{
		if (!is_active)
		{
			// return;
		}

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr != components[i])
				components[i]->Begin();
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->Begin();
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->Begin();
		}
	}

	void GameObject::Tick()
	{
		if (!is_active)
		{
			return;
		}

		if (is_first_tick)
		{
			is_first_tick = false;
			Start();
		}

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (components[i] && components[i]->enabled)
			{
				components[i]->Tick();
			}
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			if (scripts[i]->enabled)
			{
				scripts[i]->Tick();
			}
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->Tick();
		}
	}

	void GameObject::FinalTick()
	{
		if (!is_active)
		{
			return;
		}

		if (is_life_span)
		{
			cur_life_time += DT;
			if (life_time < cur_life_time)
			{
				DestroyObject(this);
			}
		}


		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (components[i] && components[i]->enabled)
			{
				components[i]->FinalTick();
			}
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->FinalTick();
		}

		// 소속 레이어가 없는데 finaltick 이 호출되었다.
		assert(-1 != layer_idx);

		// 레이어 등록
		Layer* pCurLayer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(layer_idx);
		pCurLayer->RegisterObject(this);
	}

	void GameObject::FinalTickModule()
	{
		if (!is_active)
		{
			return;
		}

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (components[i] && components[i]->enabled)
			{
				components[i]->FinalTick();
			}
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->FinalTickModule();
		}
	}

	void GameObject::Render()
	{
		if (!is_active)
		{
			return;
		}

		if (nullptr != render_component)
		{
			render_component->Render();
		}
	}

	void GameObject::RenderShadowMap()
	{
		if (!is_active)
		{
			return;
		}

		if (nullptr != render_component)
			render_component->RenderShadowmap();
	}

	void GameObject::OnEnable()
	{
		if (!is_active)
		{
			return;
		}



		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (components[i] && components[i]->enabled)
			{
				components[i]->OnEnable();
			}
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			if (scripts[i]->enabled)
			{
				scripts[i]->OnEnable();
			}
		}
	}

	void GameObject::OnDisable()
	{
		if (is_active)
		{
			return;
		}
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (components[i] && components[i]->enabled)
			{
				components[i]->OnDisable();
			}
		}
		for (size_t i = 0; i < scripts.size(); ++i)
		{
			if (scripts[i]->enabled)
			{
				scripts[i]->OnDisable();
			}
		}

	}

	void GameObject::AddComponent(Component* _Component)
	{
		_Component->owner = this;

		// 컴포넌트가 스크립트인 경우
		if (COMPONENT_TYPE::SCRIPT == _Component->GetType())
		{
			scripts.push_back((Script*)_Component);
		}

		// 스크립트를 제외한 일반 컴포넌트인 경우
		else
		{
			// 이미 보유하고 있는 컴포넌트인 경우
			assert(!components[(UINT)_Component->GetType()]);

			components[(UINT)_Component->GetType()] = _Component;

			// RenderComponent 확인
			RenderComponent* pRenderCom = dynamic_cast<RenderComponent*>(_Component);
			if (pRenderCom)
			{
				// 이미 1개 이상의 렌더컴포넌트를 보유하고 있다면 assert
				assert(!render_component);
				render_component = pRenderCom;
			}
		}
	}

	void GameObject::AddChild(GameObject* _Object)
	{
		if (_Object->parent)
		{
			// 기존 부모가 있으면 연결 해제 후 연결
			_Object->DisconnectFromParent();
		}

		else
		{
			// 기존 부모가 없으면, 소속 레이어에서 최상위부모 목록에서 제거된 후 연결
			_Object->ChangeToChildType();
		}


		// 부모 자식 연결
		_Object->parent = this;
		child.push_back(_Object);
	}

	void GameObject::SetLayerIndexByName(const wstring& name)
	{
		layer_idx = LevelMgr::GetInst()->GetCurLevel()->FindLayerIdxByName(name);
	}

	GameObject* GameObject::FindChildObject(wstring _name)
	{
		for (int i = 0; i < child.size(); i++)
		{
			if (child[i]->GetName() == _name)
			{
				return child[i];
			}
		}

		return nullptr;
	}

	void GameObject::SetActive(bool _active)
	{
		if (_active != is_active)
		{
			is_active = _active;

			if (_active)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
		}
	}

	void GameObject::SetLifeSpan(float _fTime)
	{
		if (is_life_span)
			return;

		life_time = _fTime;
		is_life_span = true;
	}

	bool GameObject::IsAncestor(GameObject* _Target)
	{
		GameObject* pParent = parent;
		while (pParent)
		{
			if (pParent == _Target)
			{
				return true;
			}
			pParent = pParent->parent;
		}

		return false;
	}

	void GameObject::DisconnectFromParent()
	{
		if (!parent)
			return;

		vector<GameObject*>::iterator iter = parent->child.begin();
		for (; iter != parent->child.end(); ++iter)
		{
			if (this == *iter)
			{
				parent->child.erase(iter);
				parent = nullptr;
				return;
			}
		}

		assert(nullptr);
	}

	void GameObject::ChangeToChildType()
	{
		assert(-1 <= layer_idx && layer_idx < MAX_LAYER);
	
		if (-1 != layer_idx)
		{
			Layer* pLayer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(layer_idx);
			pLayer->RemoveFromParentList(this);
		}
	}

	void GameObject::AddParentList()
	{
		Layer* pLayer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(layer_idx);
		pLayer->AddParentList(this);
	}
}