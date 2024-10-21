#pragma once
#include "Singleton.h"

#include "ptr.h"
#include "ComputeShader.h"
#include "GraphicsShader.h"
#include "Mesh.h"
#include "MeshData.h"
#include "Material.h"
#include "Texture.h"
#include "Prefab.h"
#include "Sound.h"
#include "Animation3D.h"
#include "AnimatorController.h"
#include "ThreadPool.h"

#include "PathMgr.h"

namespace ff7r
{
    class ResourceMgr : public Singleton<ResourceMgr>
    {
    public:
        void Init();
        void Tick();

        Ptr<Texture> CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _tex2d);
        Ptr<Texture> CreateTexture(const wstring& _key, D3D11_TEXTURE2D_DESC _desc);
        Ptr<Texture> CreateCubeTexture(const wstring& _key, D3D11_TEXTURE2D_DESC _desc, bool _use_generate_mips = false);
        Ptr<Texture> CreateTexture(const wstring& _key, UINT _width, UINT _height
            , DXGI_FORMAT _pixelformat, UINT _bindflag, D3D11_USAGE _usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
        Ptr<Prefab> CreatePrefab(const wstring& _key, GameObject* _proto = nullptr);
        Ptr<AnimatorController> CreateAnimatorController(const wstring& _key);

        const map<wstring, Ptr<Resource>>& GetResources(RES_TYPE _type) { return resources[(UINT)_type]; }
        bool IsResourceChanged() { return is_change; }

        Ptr<MeshData> LoadGLTF(const wstring& _gltf_path, wstring _test = L"");

        std::shared_ptr<std::mutex> GetTextureMutex() { return resource_mutex[(UINT)RES_TYPE::TEXTURE]; }
        std::shared_ptr<std::mutex> GetSoundMutex() { return resource_mutex[(UINT)RES_TYPE::SOUND]; }

        void ResourceThreadWait() { resource_thread->Wait(); }
        bool ResourceThreadEmpty() { return resource_thread->IsEmpty(); }

        template<typename T> Ptr<T> FindRes(const wstring& _key);
        template<typename T> void   AddRes(const wstring& _key, Ptr<T> _res);
        template<typename T> Ptr<T> Load(const wstring& _key, const wstring& _path);

        template<typename T> void   MultiThreadLoad(const wstring& _key, const wstring& _path);
        template<typename T> Ptr<T> MultiFindRes(const wstring& _key);
        template<typename T> Ptr<T> MultiLoad(const wstring& _key, const wstring& _path);

        void Test();
    private:
        SINGLE(ResourceMgr);

        void InitSound();
        void CreateDefaultMesh();
        void CreateDefaultGraphicsShader();
        void CreateDefaultComputeShader();
        void CreateDefaultMaterial();
        void CreateDefaultTexture();

        void DeleteRes(RES_TYPE _type, const wstring& _strKey);

        map<wstring, Ptr<Resource>> resources[(UINT)RES_TYPE::END];
        bool                        is_change;

        ThreadPool* resource_thread;
        vector<std::shared_ptr<std::mutex>>     resource_mutex;
        // vector<std::unique_lock<std::mutex>*>   resource_lock;

        friend class EventMgr;
    };

    template<typename T>
    RES_TYPE GetResType()
    {
        const type_info& _mesh = typeid(Mesh);
        const type_info& meshdata = typeid(MeshData);
        const type_info& _material = typeid(Material);
        const type_info& _texture = typeid(Texture);
        const type_info& _sound = typeid(Sound);
        const type_info& _prefab = typeid(Prefab);
        const type_info& _animation = typeid(Animation3D);
        const type_info& _animator_controller = typeid(AnimatorController);
        const type_info& _gs = typeid(GraphicsShader);
        const type_info& _cs = typeid(ComputeShader);

        if (typeid(T).hash_code() == _mesh.hash_code())
            return RES_TYPE::MESH;
        if (typeid(T).hash_code() == meshdata.hash_code())
            return RES_TYPE::MESHDATA;
        if (typeid(T).hash_code() == _gs.hash_code())
            return RES_TYPE::GRAPHICS_SHADER;
        if (typeid(T).hash_code() == _cs.hash_code())
            return RES_TYPE::COMPUTE_SHADER;
        if (typeid(T).hash_code() == _texture.hash_code())
            return RES_TYPE::TEXTURE;
        if (typeid(T).hash_code() == _material.hash_code())
            return RES_TYPE::MATERIAL;
        if (typeid(T).hash_code() == _prefab.hash_code())
            return RES_TYPE::PREFAB;
        if (typeid(T).hash_code() == _sound.hash_code())
            return RES_TYPE::SOUND;
        if (typeid(T).hash_code() == _animation.hash_code())
            return RES_TYPE::ANIMATION3D;
        if (typeid(T).hash_code() == _animator_controller.hash_code())
            return RES_TYPE::ANIMATOR_CONTROLLER;

        return RES_TYPE::END;
    }


    template<typename T>
    inline Ptr<T> ResourceMgr::FindRes(const wstring& _key)
    {
        RES_TYPE _type = GetResType<T>();

        map<wstring, Ptr<Resource>>::iterator iter = resources[(UINT)_type].find(_key);
        if (iter == resources[(UINT)_type].end())
            return nullptr;

        return (T*)iter->second.Get();
    }


    template<typename T>
    inline void ResourceMgr::AddRes(const wstring& _key, Ptr<T> _res)
    {
        // 중복키로 리소스 추가하려는 경우
        //assert(!FindRes<T>(_key).Get());

        if (FindRes<T>(_key).Get())
        {
            return;
        }

        RES_TYPE _type = GetResType<T>();
        resources[(UINT)_type].insert(make_pair(_key, _res.Get()));
        _res->SetKey(_key);

        is_change = true;
    }


    template<typename T>
    inline Ptr<T> ResourceMgr::Load(const wstring& _key, const wstring& _relative_path)
    {
        Ptr<Resource> _res = FindRes<T>(_key).Get();

        // 이미 해당 키로 리소스가 있다면, 반환
        if (nullptr != _res)
        {
            return (T*)_res.Get();
        }

        _res = new T;
        _res->SetKey(_key);
        _res->SetRelativePath(_relative_path);

        wstring _file_path = PathMgr::GetInst()->GetContentPath();
        _file_path += _relative_path;

        if (FAILED(_res->Load(_file_path)))
        {
            assert(nullptr);
        }

        RES_TYPE _type = GetResType<T>();
        resources[(UINT)_type].insert(make_pair(_key, _res));


        is_change = true;

        return (T*)_res.Get();
    }

    template<typename T>
    void ResourceMgr::MultiThreadLoad(const wstring& _key, const wstring& _relative_path)
    {
        resource_thread->Enqueue([=]() { MultiLoad<T>(_key, _relative_path); });
    }

    template<typename T>
    inline Ptr<T> ResourceMgr::MultiFindRes(const wstring& _key)
    {
        RES_TYPE _type = GetResType<T>();

        std::unique_lock<std::mutex> lock(*resource_mutex[static_cast<size_t>(_type)].get());

        map<wstring, Ptr<Resource>>::iterator iter = resources[(UINT)_type].find(_key);
        if (iter == resources[(UINT)_type].end())
        {
            return nullptr;
        }

        return (T*)iter->second.Get();
    }


    template<typename T>
    inline Ptr<T> ResourceMgr::MultiLoad(const wstring& _key, const wstring& _relative_path)
    {
        Ptr<Resource> _res = MultiFindRes<T>(_key).Get();


        // 이미 해당 키로 리소스가 있다면, 반환
        if (nullptr != _res)
        {
            return (T*)_res.Get();
        }

        _res = new T;
        _res->SetKey(_key);
        _res->SetRelativePath(_relative_path);

        wstring _file_path = PathMgr::GetInst()->GetContentPath();
        _file_path += _relative_path;

        if (FAILED(_res->Load(_file_path)))
        {
            assert(nullptr);
        }



        RES_TYPE _type = GetResType<T>();
        std::unique_lock<std::mutex> lock(*resource_mutex[static_cast<size_t>(_type)].get());

        Ptr<Resource> _res2 = FindRes<T>(_key).Get();
        if (nullptr != _res2)
        {
            return (T*)_res2.Get();
        }

        resources[(UINT)_type].insert(make_pair(_key, _res));

        is_change = true;

        return (T*)_res.Get();
    }
}