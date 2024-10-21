#include "pch.h"
#include "Collider3DUI.h"

#include <Engine\GameObject.h>
#include <Engine\Collider3D.h>

namespace ff7r
{
    Collider3DUI::Collider3DUI()
        : ComponentUI("##Collider3D", COMPONENT_TYPE::COLLIDER3D)
    {
        SetName("Collider3D");
    }

    Collider3DUI::~Collider3DUI()
    {
    }

    int Collider3DUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;

        vec3 _offset_pos = GetTarget()->GetCollider3D()->GetOffsetPos();
        vec3 _offset_scale = GetTarget()->GetCollider3D()->GetOffsetScale();
        float _mass = GetTarget()->GetCollider3D()->GetMass();


        bool    _able_collision = GetTarget()->GetCollider3D()->GetCollision();
        bool    _is_absolute = GetTarget()->GetCollider3D()->IsAbsolute();
        bool    _is_trigger = GetTarget()->GetCollider3D()->IsTrigger();
        bool    _is_dynamic = GetTarget()->GetCollider3D()->IsDynamic();
        bool    _is_collision = GetTarget()->GetCollider3D()->IsCollision();
        bool    _is_move = GetTarget()->GetCollider3D()->GetMove();
        int type = (UINT)GetTarget()->GetCollider3D()->GetType();


        ImGui::Text("OffsetPos ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Collider Offset Position", _offset_pos);

        ImGui::Text("OffsetScale ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Collider Offset Scale", _offset_scale);

        ImGui::Text("Mass ");
        ImGui::SameLine();
        ImGui::InputFloat("##Collider Mass", &_mass);

        ImGui::Text("Able Collision ");
        ImGui::SameLine();
        ImGui::Checkbox("##Collider Able Collision", &_able_collision);
        
        ImGui::Text("Absolute Option ");
        ImGui::SameLine();
        ImGui::Checkbox("##Collider Absolute Option", &_is_absolute);
        
        ImGui::Text("Trigger Option ");
        ImGui::SameLine();
        ImGui::Checkbox("##Collider Trigger Option", &_is_trigger);
        
        ImGui::Text("Dynamic Option");
        ImGui::SameLine();
        ImGui::Checkbox("##Collider Dynamic Option", &_is_dynamic);

        ImGui::Text("Move Option ");
        ImGui::SameLine();
        ImGui::Checkbox("##Collider Move Option", &_is_move);

        ImGui::Text("Current Collision : ");
        ImGui::SameLine();
        if(_is_collision)
            ImGui::Text("True");
        else
            ImGui::Text("False");

        ImGui::Text("3D Collider Type ");
        ImGui::SameLine();
        ImGui::InputInt("##Collider Type", &type);
        switch (type)
        {
        case 0:
        {
            ImGui::Text("Cube(0)");
            break;
        }
        case 1:
        {
            ImGui::Text("Sphere(1) ");
            break;
        }
        case 2:
        {
            ImGui::Text("Capsule(2)");
            break;
        }
        case 3:
        {
            ImGui::Text("ConvexMesh(3)");
            break;
        }
        default:
            break;
        }

        GetTarget()->GetCollider3D()->SetOffsetPos(_offset_pos);
        GetTarget()->GetCollider3D()->SetOffsetScale(_offset_scale);
        GetTarget()->GetCollider3D()->SetMass(_mass);
        if (_able_collision)
        {
            GetTarget()->GetCollider3D()->SetCollision(true);
        }
        else 
        {
            GetTarget()->GetCollider3D()->Disable();
        }
        GetTarget()->GetCollider3D()->SetAbsolute(_is_absolute);
        GetTarget()->GetCollider3D()->SetTrigger(_is_trigger);
        GetTarget()->GetCollider3D()->SetDynamic(_is_dynamic);
        GetTarget()->GetCollider3D()->SetMove(_is_move);
        
        if(type >= 0 && type <= 3)
            GetTarget()->GetCollider3D()->SetCollider3DType((COLLIDER3D_TYPE)(UINT)type);

       


        return TRUE;
    }
}