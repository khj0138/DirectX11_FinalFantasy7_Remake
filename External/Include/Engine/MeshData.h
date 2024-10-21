#pragma once

#include "ptr.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation3D.h"

namespace ff7r
{
	class MeshData : public Resource
	{
	public:
		MeshData(bool _is_engine = false);
		virtual ~MeshData();

		CLONE_DISABLE(MeshData)

		static MeshData* LoadFromGLTF(const wstring& _file_path);

		virtual int Save(const wstring& _file_path) override;
		virtual int Load(const wstring& _file_path) override;

		GameObject* Instantiate();
	private:
		Ptr<Mesh>				mesh;
		vector<Ptr<Material>>	materials;
		vector<Ptr<Animation3D>>			animations;
	};
}
