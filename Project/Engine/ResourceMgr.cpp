#include "pch.h"
#include "ResourceMgr.h"

#include "PathMgr.h"

namespace ff7r
{
	ResourceMgr::ResourceMgr()
		: is_change(false)
		, resource_thread(nullptr)
	{
	}

	ResourceMgr::~ResourceMgr()
	{
		if (resource_thread)
		{
			delete resource_thread;
			resource_thread = nullptr;
		}
	}

	void test()
	{
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (SUCCEEDED(hr))
		{
			// COM 기반 라이브러리 사용
			// 예: DirectXTex 함수 호출

			// 사용 완료 후 COM 라이브러리 정리
			//CoUninitialize();
		}
	}

	void ResourceMgr::Init()
	{
		//resource_thread = new ThreadPool((UINT)RES_TYPE::END);
		resource_thread = new ThreadPool(16);
		resource_thread->Enqueue([=]() {test(); });

		for (int i = 0; i < (UINT)RES_TYPE::END; i++)
		{
			resource_mutex.push_back(std::make_shared<std::mutex>());
		}
		
		InitSound();

		CreateDefaultMesh();
		CreateDefaultGraphicsShader();
		CreateDefaultMaterial();
		CreateDefaultTexture();
		CreateDefaultComputeShader();
	}



	void ResourceMgr::InitSound()
	{
		FMOD::System_Create(&Sound::g_fmod_system);

		if (nullptr == Sound::g_fmod_system)
		{
			assert(nullptr);
		}

		// 32개 채널 생성
		Sound::g_fmod_system->init(32, FMOD_DEFAULT, nullptr);
	}


	void ResourceMgr::Tick()
	{
		is_change = false;
	}

	Ptr<Texture> ResourceMgr::CreateTexture(const wstring& _key, UINT _width, UINT _height
		, DXGI_FORMAT _pixelformat, UINT _bind_flag, D3D11_USAGE _usage)
	{
		Ptr<Texture> _tex = FindRes<Texture>(_key);

		assert(nullptr == _tex);

		_tex = new Texture(true);
		if (FAILED(_tex->Create(_width, _height, _pixelformat, _bind_flag, _usage)))
		{
			assert(nullptr);
		}

		AddRes<Texture>(_key, _tex);

		return _tex;
	}

	Ptr<Prefab> ResourceMgr::CreatePrefab(const wstring& _key, GameObject* _proto)
	{
		wstring _path = L"Prefab\\" + _key + L".pref";
		Ptr<Prefab> _prefab = FindRes<Prefab>(_path);
		assert(nullptr == _prefab);

		_prefab = new Prefab();

		_prefab->RegisterProtoObject(_proto);

		AddRes<Prefab>(_path, _prefab);

		return _prefab;
	}

	Ptr<AnimatorController> ResourceMgr::CreateAnimatorController(const wstring& _key)
	{
		Ptr<AnimatorController> _controller = FindRes<AnimatorController>(_key);
		assert(_controller == nullptr);

		_controller = new AnimatorController();
		_controller->CreateAnimatorController();

		AddRes<AnimatorController>(_key, _controller);

		return _controller;
	}

	Ptr<Texture> ResourceMgr::CreateTexture(const wstring& _key, D3D11_TEXTURE2D_DESC _desc)
	{
		Ptr<Texture> _tex = FindRes<Texture>(_key);

		assert(nullptr == _tex);

		_tex = new Texture(true);

		if (_desc.MiscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE)
		{
			if (FAILED(_tex->CreateCube(_desc)))
			{
				assert(nullptr);
			}
		}
		else
		{
			if (FAILED(_tex->Create(_desc.Width, _desc.Height, _desc.Format, _desc.BindFlags, _desc.Usage)))
			{
				assert(nullptr);
			}
		}

		AddRes<Texture>(_key, _tex);

		return _tex;
	}

	Ptr<Texture> ResourceMgr::CreateCubeTexture(const wstring& _key, D3D11_TEXTURE2D_DESC _desc, bool _use_generate_mips)
	{
		Ptr<Texture> _tex = FindRes<Texture>(_key);

		assert(nullptr == _tex);

		_tex = new Texture(true);

		if (FAILED(_tex->CreateCube(_desc, _use_generate_mips)))
		{
			assert(nullptr);
		}
		return _tex;
	}

	Ptr<MeshData> ResourceMgr::LoadGLTF(const wstring& _gltf_path, wstring _test)
	{
		wstring original_folder_name = _test + L"FF7R Resource";
		wstring user_file_path = path(_gltf_path).parent_path().parent_path().wstring();
		user_file_path.replace(user_file_path.find(original_folder_name), original_folder_name.length(), _test + L"User Resource");

		wstring _meshdata_path = user_file_path + L"\\MeshData\\" + path(_gltf_path).stem().wstring() + L".mdat";

		Ptr<MeshData> _meshdata = FindRes<MeshData>(_meshdata_path);

		if (nullptr != _meshdata)
		{
			return _meshdata;
		}

		_meshdata = MeshData::LoadFromGLTF(_gltf_path);
		_meshdata->SetKey(_meshdata_path);
		_meshdata->SetRelativePath(_meshdata_path);

		resources[(UINT)RES_TYPE::MESHDATA].insert(make_pair(_meshdata_path, _meshdata.Get()));

		// meshdata 를 실제파일로 저장
		_meshdata->Save(_meshdata_path);

		return _meshdata;
	}

	Ptr<Texture> ResourceMgr::CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _tex2d)
	{
		Ptr<Texture> _tex = FindRes<Texture>(_key);

		assert(nullptr == _tex);

		_tex = new Texture(true);
		if (FAILED(_tex->Create(_tex2d)))
		{
			assert(nullptr);
		}

		AddRes<Texture>(_key, _tex);

		return _tex;
	}


	void ResourceMgr::DeleteRes(RES_TYPE _type, const wstring& _key)
	{
		map<wstring, Ptr<Resource>>::iterator _iter = resources[(UINT)_type].find(_key);

		assert(!(_iter == resources[(UINT)_type].end()));

		resources[(UINT)_type].erase(_iter);

		is_change = true;
	}
}