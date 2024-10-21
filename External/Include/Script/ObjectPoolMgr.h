#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class GameObject;
	// �ϴ� �� ��ũ��Ʈ ������ �����ϸ� ���� ����� ����. ���߿� ����
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
