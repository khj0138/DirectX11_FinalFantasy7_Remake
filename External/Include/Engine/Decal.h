#pragma once
#include "RenderComponent.h"

namespace ff7r
{
    class Decal : public RenderComponent
    {
    public:
        Decal();
        ~Decal();

        CLONE(Decal);

        virtual void FinalTick() override;
        virtual void Render() override;
        virtual void Render(UINT _i) override {}

        void SetDeferredDecal(bool _bDeferred);
        void ActivateEmissive(bool _bActivate) { is_emissive = _bActivate; }

    private:
        Ptr<Texture>    decal_tex;

        bool            is_deferred;
        bool            is_emissive;
    };

}