#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class GameObject;
	// 일단 이 스크립트 여러번 생성하면 문제 생길거 같음. 나중에 수정
	class ObjectPoolMgr : public Script
	{
	public:
		ObjectPoolMgr();
		~ObjectPoolMgr();

		CLONE(ObjectPoolMgr);

		virtual void Awake() override;

		void RegisterObject(GameObject* _obj, wstring _key, int i=0);
		GameObject* GetGameObject(wstring _key);

		static ObjectPoolMgr* GetInst() { return g_inst; }
		static void Destroy() {}

	private:
		static ObjectPoolMgr* g_inst;
		static map<wstring, vector<GameObject*>> g_object_pool;
	};
}
