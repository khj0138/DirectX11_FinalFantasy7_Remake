#pragma once
#include "Entity.h"

namespace ff7r
{
    class Resource : public Entity
    {
    public:
        Resource(RES_TYPE _type, bool _bEngine = false);
        Resource(const Resource& _Other);
        virtual ~Resource();
        
        CLONE_DISABLE(Resource);

        const wstring& GetKey() { return key; }
        const wstring& GetRelativePath() { return relative_path; }
        RES_TYPE GetType() { return type; }
        bool IsEngineRes() { return is_engine_res; }

        virtual int Save(const wstring&) = 0;

    protected:
        void SetKey(const wstring& _strKey) { key = _strKey; }
        void SetRelativePath(const wstring& _strPath) { relative_path = _strPath; }

    private:
        virtual int Load(const wstring& _strFilePath) = 0;

        virtual void UpdateData() {};

        void AddRef() { ++ref_cnt; }
        void Release();

        const RES_TYPE  type;
        int             ref_cnt;

        wstring         key;
        wstring         relative_path;

        bool            is_engine_res; 

        template<typename T> friend class Ptr;
        friend class ResourceMgr;
        friend class GLTFLoader;
    };

}