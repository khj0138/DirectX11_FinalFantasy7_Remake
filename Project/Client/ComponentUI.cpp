#include "pch.h"
#include "ComponentUI.h"

#include <Engine\GameObject.h>

namespace ff7r
{
	ComponentUI::ComponentUI(const string& _Name, COMPONENT_TYPE _Type)
		: UI(_Name)
		, m_Type(_Type)
	{
	}

	ComponentUI::~ComponentUI()
	{

	}

	int ComponentUI::RenderUpdate()
	{
		if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_Type))
			return FALSE;

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::Button(GetName().c_str());
		ImGui::PopStyleColor(3);
		ImGui::PopID();

		return TRUE;
	}


	bool ComponentUI::SetTarget(GameObject* _Target)
	{
		m_Target = _Target;

		if (nullptr == m_Target)
		{
			SetActive(false);
			return false;
		}

		if (nullptr == m_Target->GetComponent(m_Type))
		{
			SetActive(false);
			return false;
		}
		else
		{
			SetActive(true);
			return true;
		}
	}

	void ComponentUI::GetResKey(Ptr<Resource> _Res, char* _Buff, size_t _BufferSize)
	{
		memset(_Buff, 0, sizeof(char) * _BufferSize);

		if (nullptr == _Res)
			return;

		wstring wstrKey = _Res->GetKey();
		string	strKey = string(wstrKey.begin(), wstrKey.end());
		memcpy(_Buff, strKey.data(), sizeof(char) * strKey.length());
	}
}