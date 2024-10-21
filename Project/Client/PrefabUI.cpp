#include "pch.h"

#include <Engine\Prefab.h>

#include "PrefabUI.h"
namespace ff7r
{
	PrefabUI::PrefabUI()
		:ResourceUI(RES_TYPE::PREFAB)
	{
		SetName("Prefab");
	}

	PrefabUI::~PrefabUI()
	{

	}

	int PrefabUI::RenderUpdate()
	{
		ResourceUI::RenderUpdate();
		
		if (ImGui::Button("Save Prefab", ImVec2(200, 18)))
		{
			Ptr<Prefab> _prefab = dynamic_cast<Prefab*>(GetTargetRes().Get());

			if (_prefab != nullptr)
			{
				_prefab->Save(L"");
			}
		}

		return 1;
	}
}