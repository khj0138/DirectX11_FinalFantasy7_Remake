#include "pch.h"
#include <Engine\Light3D.h>
#include "Light3DUI.h"



namespace ff7r
{
    Light3DUI::Light3DUI()
        : ComponentUI("##Light3D", COMPONENT_TYPE::LIGHT3D)
    {
        SetName("Light3D");
    }

    Light3DUI::~Light3DUI()
    {
    }


    int Light3DUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;

        vec3 _color = GetTarget()->GetLight3D()->GetLightColor();
        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::DragFloat3("##Light Color", _color);


        int _type = (int)GetTarget()->GetLight3D()->GetLightType();
        ImGui::Text("Type");
        ImGui::SameLine();
        ImGui::InputInt("##Light Type", &_type);

        float _radius = GetTarget()->GetLight3D()->GetRadius();
        ImGui::Text("Radius");
        ImGui::SameLine();
        ImGui::DragFloat("##Light Radius", &_radius);

        float _intensity = GetTarget()->GetLight3D()->GetIntensity();
        ImGui::Text("Intensity");
        ImGui::SameLine();
        ImGui::DragFloat("##Light Intensity", &_intensity);

        float _angle = Rad2Degree(GetTarget()->GetLight3D()->GetAngle());
        ImGui::Text("Angle");
        ImGui::SameLine();
        ImGui::DragFloat("##Light Angle", &_angle);

        GetTarget()->GetLight3D()->SetLightColor(_color);
        GetTarget()->GetLight3D()->SetRadius(_radius);
        GetTarget()->GetLight3D()->SetIntensity(_intensity);
        GetTarget()->GetLight3D()->SetAngle(Degree2Rad(_angle));
        GetTarget()->GetLight3D()->SetLightType(LIGHT_TYPE(_type));

        return TRUE;
    }
}