#include "pch.h"

#include <Engine\ReflectionCapture.h>
#include <Engine\RenderMgr.h>

#include "CaptureUI.h"
namespace ff7r
{
	CaptureUI::CaptureUI()
		: ComponentUI("##Capture", COMPONENT_TYPE::REFLECTIONCAPTURE)
	{
		SetName("Capture");
	}

	CaptureUI::~CaptureUI()
	{
	}

	int CaptureUI::RenderUpdate()
	{
		if (FALSE == ComponentUI::RenderUpdate())
		{
			return FALSE;
		}

		CaptureInfo& _info = GetTarget()->GetReflectionCapture()->GetCaptureInfo();

		int _level = static_cast<int>(_info.level);

		ImGui::Text("Level");
		ImGui::SameLine();
		ImGui::InputInt("##Capture Level", &_level);

		ImGui::Text("Radius");
		ImGui::SameLine();
		ImGui::DragFloat("##Capture Radius", &_info.radius);

		if (_level < 0)
		{
			_level = 0;
		}

		_info.level = _level;

		if (ImGui::Button("Capture", ImVec2(200, 18)))
		{
			RenderMgr::GetInst()->RegisterUpdateCapture(GetTarget()->GetReflectionCapture());
		}


		return TRUE;
	}
}