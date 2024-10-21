#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\Resource.h>

namespace ff7r
{
    class ResourceUI : public UI
    {
    private:
        Ptr<Resource>       m_TargetRes;
        const RES_TYPE  m_ResType;


    public:
        virtual int RenderUpdate() override;

    public:
        void SetTargetRes(Ptr<Resource> _Res) { m_TargetRes = _Res; }
        Ptr<Resource> GetTargetRes() { return m_TargetRes; }


    public:
        ResourceUI(RES_TYPE _type);
        ~ResourceUI();
    };
}
