#pragma once

#include "RenderComponent.h"

namespace ff7r
{
    enum class SKYBOX_TYPE
    {
        SPHERE,
        CUBE,
    };

    class SkyBox : public RenderComponent
    {
    public:
        SkyBox();
        ~SkyBox();

        CLONE(SkyBox);

        virtual void FinalTick() override;
        virtual void Render() override;
        virtual void Render(UINT _i) override;

        void SetSkyBoxType(SKYBOX_TYPE _type);
        void SetSkyBoxTexture(Ptr<Texture> _tex);

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

    private:
        SKYBOX_TYPE     type;
        Ptr<Texture>    skybox_tex;
    };

}