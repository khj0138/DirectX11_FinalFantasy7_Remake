#pragma once
#include "Resource.h"

namespace ff7r
{
    class Shader : public Resource
    {
    protected:
        ComPtr<ID3DBlob>    error_blob;

    public:
        virtual int Save(const wstring& _strRelativePath) { return S_OK; }
    private:
        virtual int Load(const wstring& _strFilePath) { return S_OK; }

    public:
        Shader(RES_TYPE _eType);
        ~Shader();
    };

}