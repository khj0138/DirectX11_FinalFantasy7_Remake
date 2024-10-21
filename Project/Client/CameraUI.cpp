#include "pch.h"
#include "CameraUI.h"

#include <Engine\GameObject.h>
#include <Engine\Camera.h>

namespace ff7r
{
    CameraUI::CameraUI()
        : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
    {
        SetName("Camera");
    }

    CameraUI::~CameraUI()
    {
    }

    int CameraUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;


		float _fov = GetTarget()->GetCamera()->GetFOV();
        _fov = Rad2Degree(_fov);

		ImGui::Text("FOV");
		ImGui::SameLine();
		ImGui::DragFloat("##FOV", &_fov);

        _fov = Degree2Rad(_fov);
		GetTarget()->GetCamera()->SetFOV(_fov);

        return TRUE;
    }
}