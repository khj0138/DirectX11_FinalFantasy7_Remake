#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\MeshRender.h>
#include <Engine\ResourceMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"


namespace ff7r
{
	MeshRenderUI::MeshRenderUI()
		: ComponentUI("##MeshRender", COMPONENT_TYPE::MESHRENDER)
	{
		SetName("MeshRender");
	}

	MeshRenderUI::~MeshRenderUI()
	{
	}


	int MeshRenderUI::RenderUpdate()
	{
		if (FALSE == ComponentUI::RenderUpdate())
			return FALSE;

		char szBuff[150] = {};

		Ptr<Mesh> pMesh = GetTarget()->GetMeshRender()->GetMesh();
		Ptr<Material> pMtrl = GetTarget()->GetMeshRender()->GetMaterial();

		ImGui::Text("Mesh    ");
		ImGui::SameLine();
		GetResKey(pMesh.Get(), szBuff, 150);
		ImGui::InputText("##MeshName", szBuff, 150, ImGuiInputTextFlags_ReadOnly);

		// Mesh ��� üũ
		if (ImGui::BeginDragDropTarget())
		{
			// �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
			if (pPayLoad)
			{
				TreeNode* pNode = (TreeNode*)pPayLoad->Data;
				Resource* pRes = (Resource*)pNode->GetData();
				if (RES_TYPE::MESH == pRes->GetType())
				{
					GetTarget()->GetMeshRender()->SetMesh((Mesh*)pRes);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();


		if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
		{
			const map<wstring, Ptr<Resource>>& mapMesh = ResourceMgr::GetInst()->GetResources(RES_TYPE::MESH);

			ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
			pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
			for (const auto& pair : mapMesh)
			{
				pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
			}

			// �׸� ���ý� ȣ����� ��������Ʈ ���
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
		}

		ImGui::Text("Material");
		ImGui::SameLine();
		GetResKey(pMtrl.Get(), szBuff, 150);
		ImGui::InputText("##MtrlName", szBuff, 150, ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			// �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
			if (pPayLoad)
			{
				TreeNode* pNode = (TreeNode*)pPayLoad->Data;
				Resource* pRes = (Resource*)pNode->GetData();
				if (RES_TYPE::MATERIAL == pRes->GetType())
				{
					GetTarget()->GetMeshRender()->SetMaterial((Material*)pRes);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();

		if (ImGui::Button("##MtrlSelectBtn", ImVec2(18, 18)))
		{
			const map<wstring, Ptr<Resource>>& mapMtrl = ResourceMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

			ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
			pListUI->Reset("Material", ImVec2(300.f, 500.f));
			for (const auto& pair : mapMtrl)
			{
				pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
			}

			// �׸� ���ý� ȣ����� ��������Ʈ ���
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMaterial);
		}

		return TRUE;
	}

	void MeshRenderUI::SelectMesh(DWORD_PTR _Key)
	{
		string strKey = (char*)_Key;
		Ptr<Mesh> pMesh = ResourceMgr::GetInst()->FindRes<Mesh>(wstring(strKey.begin(), strKey.end()));
		GetTarget()->GetMeshRender()->SetMesh(pMesh);
	}

	void MeshRenderUI::SelectMaterial(DWORD_PTR _Key)
	{
		string strKey = (char*)_Key;
		Ptr<Material> pMtrl = ResourceMgr::GetInst()->FindRes<Material>(wstring(strKey.begin(), strKey.end()));
		GetTarget()->GetMeshRender()->SetMaterial(pMtrl);
	}
}