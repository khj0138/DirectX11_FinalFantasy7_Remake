#include "pch.h"
#include "UI.h"

namespace ff7r
{
	UI::UI(const string& _ID)
		: m_strID(_ID)
		, m_ParentUI(nullptr)
		, m_Modal(false)
		, m_Active(false)
	{

	}

	UI::~UI()
	{
		SafeDelVector(m_vecChildUI);
	}

	void UI::FinalTick()
	{
		if (!m_Active)
			return;

		string strFullName = m_strName + m_strID;

		// 부모 UI
		if (nullptr == m_ParentUI)
		{
			// 모달리스
			if (!m_Modal)
			{
				ImGui::Begin(strFullName.c_str(), &m_Active);

				RenderUpdate();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					// 자식UI 가 비활성화 상태면 건너뛴다.
					if (!m_vecChildUI[i]->IsActive())
						continue;

					m_vecChildUI[i]->FinalTick();

					// 자식 UI 간의 구분선
					if (i != m_vecChildUI.size() - 1)
						ImGui::Separator();
				}

				ImGui::End();
			}

			// 모달
			else
			{
				ImGui::SetNextWindowPos(m_vPopupPos);
				ImGui::SetNextWindowSize(m_vSize);

				ImGui::OpenPopup(strFullName.c_str());
				if (ImGui::BeginPopupModal(strFullName.c_str(), &m_Active))
				{
					RenderUpdate();

					for (size_t i = 0; i < m_vecChildUI.size(); ++i)
					{
						// 자식UI 가 비활성화 상태면 건너뛴다.
						if (!m_vecChildUI[i]->IsActive())
							continue;

						m_vecChildUI[i]->FinalTick();

						// 자식 UI 간의 구분선
						if (i != m_vecChildUI.size() - 1)
							ImGui::Separator();
					}

					ImGui::EndPopup();
				}
			}
		}

		// 자식 UI
		else
		{
			ImGui::BeginChild(strFullName.c_str(), m_vSize);

			RenderUpdate();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->FinalTick();

				if (i != m_vecChildUI.size() - 1)
					ImGui::Separator();
			}

			ImGui::EndChild();
		}
	}
}