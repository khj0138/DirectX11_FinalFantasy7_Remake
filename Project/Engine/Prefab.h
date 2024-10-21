#pragma once
#include "Resource.h"

namespace ff7r
{
	class GameObject;
	class Prefab : public Resource
	{
	public:
		Prefab();
		~Prefab();

		GameObject* Instantiate();

		void RegisterProtoObject(GameObject* _Proto);

		virtual int Save(const wstring& _relative_path) override;
		void SaveObject(GameObject* _object, FILE* _file);
	private:
		virtual void UpdateData() override {}

		virtual int Load(const wstring& _file_path) override;
		GameObject* LoadObject(FILE* _file);

		GameObject* proto_obj;
		int			layer_idx;
	};

}