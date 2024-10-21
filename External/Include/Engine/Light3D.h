#pragma once
#include "Component.h"

#include "Mesh.h"
#include "Material.h"

namespace ff7r
{
    class Light3D : public Component
    {
    private:
        LightInfo      light_info;
        Ptr<Mesh>      mesh;
        Ptr<Material>  material;
        vec3            light_dir;
        int             light_idx;
        bool            is_show_range;


    public:
        virtual void FinalTick() override;

        void Render(); // 라이팅 렌더
        void RenderShadowMap(); // 광원 시점에서 물체들의 깊이(ShadowMap)를 그림

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;

    public:
        void SetLightColor(vec3 _color) { light_info.color = _color; }
        void SetLightType(LIGHT_TYPE _type);
        void SetRadius(float _radius);
        void SetAngle(float _angle) { light_info.angle = _angle; }
        void SetIntensity(float _intentisy) { light_info.intensity = _intentisy; }
        //void SetLightDir(vec3 _dir) { light_info.world_dir = _dir; }

        vec3 GetLightColor() { return light_info.color; }
        //vec3 GetLightDir() { return light_info.world_dir; }
        LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)light_info.light_type; }
        float GetRadius() { return light_info.attenuation_radius; }
        float GetAngle() { return light_info.angle; }
        float GetIntensity() { return light_info.intensity; }

        void ShowRange(bool _bSet) { is_show_range = _bSet; }

        CLONE(Light3D);
    public:
        Light3D();
        Light3D(const Light3D& _Origin);
        ~Light3D();
    };

}