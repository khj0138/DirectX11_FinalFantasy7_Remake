#pragma once
#include "Component.h"

namespace ff7r
{
    class Light2D : public Component
    {
    private:
        LightInfo      light_info;

    public:
        void SetLightColor(vec3 _color) { light_info.color = _color; }
        void SetLightType(LIGHT_TYPE _type);
        void SetRadius(float _radius);
        void SetAngle(float _angle) { light_info.angle = _angle; }
        void SetIntensity(float _intentisy) { light_info.intensity = _intentisy; }

        vec3 GetLightColor() { return light_info.color; }
        LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)light_info.light_type; }
        float GetRadius() { return light_info.attenuation_radius; }
        float GetAngle() { return light_info.angle; }

    public:
        virtual void FinalTick() override;

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;

        CLONE(Light2D);
    public:
        Light2D();
        ~Light2D();
    };

}