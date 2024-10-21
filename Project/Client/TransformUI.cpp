#include "pch.h"
#include "TransformUI.h"

#include <Engine\GameObject.h>
#include <Engine\Transform.h>


namespace ff7r
{
	TransformUI::TransformUI()
		: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
	{
		SetName("Transform");
	}

	TransformUI::~TransformUI()
	{
	}

	int TransformUI::RenderUpdate()
	{
		if (FALSE == ComponentUI::RenderUpdate())
			return FALSE;

		vec3 vPos = GetTarget()->GetTransform()->GetRelativePos();
		vec3 vScale = GetTarget()->GetTransform()->GetRelativeScale();
		vec3 vRotation = GetTarget()->GetTransform()->GetRelativeRot();
		vRotation = (vRotation / XM_PI) * 180.f;

		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Relative Position", vPos);

		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Relative Scale", vScale);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Relative Rotation", vRotation);

		GetTarget()->GetTransform()->SetRelativePos(vPos);
		GetTarget()->GetTransform()->SetRelativeScale(vScale);

		vRotation = (vRotation / 180.f) * XM_PI;
		GetTarget()->GetTransform()->SetRelativeRot(vRotation);


		
		int _idx = GetTarget()->GetLayerIndex();
		ImGui::Text("Layer Index");
		ImGui::InputInt("#Layer Index", &_idx);

		if (GetTarget()->GetParent())
		{
			GetTarget()->SetLayerIndex(_idx);
		}
		return TRUE;
	}
}