#include "pch.h"

#include "PathMgr.h"
#include "ResourceMgr.h"

#include "Animator3D.h"
#include "Animation3D.h"
#include "GameObject.h"
#include "GLTFLoader.h"
#include "Transform.h"
#include "MeshRender.h"

#include "MeshData.h"
namespace ff7r
{
	MeshData::MeshData(bool _is_engine)
		: Resource(RES_TYPE::MESHDATA, _is_engine)
	{
	}

	MeshData::~MeshData()
	{
	}

	MeshData* MeshData::LoadFromGLTF(const wstring& _file_path)
	{
		GLTFLoader loader;
		loader.LoadGLTF(_file_path);
		
		Mesh* _mesh = nullptr;
		_mesh = Mesh::CreateFromGLTF(loader);
		
		if (_mesh != nullptr)
		{
			wstring original_folder_name = L"FF7R Resource";
			wstring user_file_path = path(_file_path).parent_path().parent_path().wstring();
			user_file_path.replace(user_file_path.find(original_folder_name), original_folder_name.length(), L"User Resource");

			wstring _path = user_file_path + L"\\Mesh\\";
			_path += path(_file_path).stem();
			_path += L".mesh";
			ResourceMgr::GetInst()->AddRes<Mesh>(_path, _mesh);
		
			_mesh->Save(_path);
		}
				
		vector<Ptr<Material>> _materials;
		
		for (const auto& material : loader.GetContainer().materials)
		{
			//Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(material.name);
			Ptr<Material> _mtrl = ResourceMgr::GetInst()->Load<Material>(material.name, material.name);
			assert(_mtrl.Get());
		
			_materials.push_back(_mtrl);
		}
		
		MeshData* _mesh_data = new MeshData(true);
		_mesh_data->mesh = _mesh;
		_mesh_data->materials = _materials;

		vector<Ptr<Animation3D>> _animations;
		if (!loader.GetAnimClip().empty())
		{
			for (const auto& clip : loader.GetAnimClip())
			{
				//Ptr<Animation3D> _ani = ResourceMgr::GetInst()->FindRes<Animation3D>(clip->name);
				Ptr<Animation3D> _ani = ResourceMgr::GetInst()->FindRes<Animation3D>(clip->name);

				assert(_ani.Get());

				_animations.push_back(_ani);
			}
			_mesh_data->animations = _animations;
		}

		return _mesh_data;
	}

	int MeshData::Save(const wstring& _file_path)
	{
		SetRelativePath(_file_path);

		wstring strFilePath = PathMgr::GetInst()->GetContentPath() + _file_path;

		path dirPath = path(strFilePath).parent_path();
		create_directories(dirPath);

		FILE* _file = nullptr;
		errno_t err = _wfopen_s(&_file, strFilePath.c_str(), L"wb");
		assert(_file);

		SaveResRef(mesh.Get(), _file);

		// material 정보 저장
		UINT _mtrl_cnt = (UINT)materials.size();
		fwrite(&_mtrl_cnt, sizeof(UINT), 1, _file);

		UINT i = 0;
		wstring _mtrl_path = PathMgr::GetInst()->GetContentPath();
		_mtrl_path += L"material\\";

		for (; i < _mtrl_cnt; ++i)
		{
			if (materials[i] == nullptr)
			{
				continue;
			}

			fwrite(&i, sizeof(UINT), 1, _file);
			SaveResRef(materials[i].Get(), _file);
		}
		i = -1;
		fwrite(&i, sizeof(UINT), 1, _file);

		UINT _anim_cnt = (UINT)animations.size();
		fwrite(&_anim_cnt, sizeof(UINT), 1, _file);

		i = 0;
		wstring _anim_path = PathMgr::GetInst()->GetContentPath();
		_anim_path += L"animation\\";

		for (; i < _anim_cnt; ++i)
		{
			if (animations[i] == nullptr)
			{
				continue;
			}

			fwrite(&i, sizeof(UINT), 1, _file);
			SaveResRef(animations[i].Get(), _file);
		}

		i = -1;
		fwrite(&i, sizeof(UINT), 1, _file);

		fclose(_file);

		return S_OK;
	}

	int MeshData::Load(const wstring& _file_path)
	{
		FILE* _file = NULL;
		_wfopen_s(&_file, _file_path.c_str(), L"rb");

		assert(_file);

		// Mesh Load
		LoadResRef<Mesh>(mesh, _file);
		assert(mesh.Get());

		// material 정보 읽기
		UINT _mtrl_cnt = 0;
		fread(&_mtrl_cnt, sizeof(UINT), 1, _file);

		materials.resize(_mtrl_cnt);

		for (UINT i = 0; i < _mtrl_cnt; ++i)
		{
			UINT idx = -1;
			fread(&idx, sizeof(UINT), 1, _file);

			if (idx == -1)
			{
				break;
			}

			Ptr<Material> _mtrl;
			LoadResRef<Material>(_mtrl, _file);

			materials[i] = _mtrl;
		}

		for (;;)
		{
			int a = 0;
			fread(&a, sizeof(int), 1, _file);

			if (a == -1)
			{
				break;
			}
		}

		UINT _anim_cnt = 0;
		fread(&_anim_cnt, sizeof(UINT), 1, _file);

		animations.resize(_anim_cnt);

		for (UINT i = 0; i < _anim_cnt; ++i)
		{
			UINT idx = -1;
			fread(&idx, sizeof(UINT), 1, _file);

			if (idx == -1)
			{
				break;
			}

			Ptr<Animation3D> _anim;
			LoadResRef<Animation3D>(_anim, _file);

			animations[i] = _anim;
		}

		fclose(_file);

		return S_OK;
	}

	GameObject* MeshData::Instantiate()
	{
		GameObject* _obj = new GameObject;
		_obj->AddComponent(new Transform);
		_obj->AddComponent(new MeshRender);

		_obj->GetMeshRender()->SetMesh(mesh);

		for (UINT i = 0; i < materials.size(); ++i)
		{
			_obj->GetMeshRender()->SetMaterial(materials[i], i);
		}

		if (animations.size() == 0)
			return _obj;

		Animator3D* _animator = new Animator3D;
		_obj->AddComponent(_animator);

		_animator->SetAnimClip(animations);
		return _obj;
	}
}