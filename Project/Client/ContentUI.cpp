#include "pch.h"
#include "ContentUI.h"

#include <Engine\ResourceMgr.h>
#include <Engine\PathMgr.h>
#include <Engine\EventMgr.h>
#include <Engine\Device.h>

#include "TreeUI.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"


namespace ff7r
{
	ContentUI::ContentUI()
		: UI("##Content")
		, root_path{ L"Default\\" }
		, exclude{ L"Stage2" }
		, is_first(true)
	{
		SetName("Content");

		// ContentUI 안에 자식으로 Tree 를 추가한다.
		m_Tree = new TreeUI;
		m_Tree->SetName("ContentTree");
		m_Tree->SetActive(true);
		m_Tree->ShowRoot(false);

		m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&ContentUI::SetTargetToInspector);
		m_Tree->SetDragDropID("Resource");
		AddChildUI(m_Tree);
	}

	ContentUI::~ContentUI()
	{

	}

	void ContentUI::Init()
	{
		Reload();
	}

	void ContentUI::Tick()
	{
		UI::Tick();

		if (ResourceMgr::GetInst()->IsResourceChanged())
		{
			ResetContent();
		}
	}

	int ContentUI::RenderUpdate()
	{
		bool _is_contentUI_hovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_ChildWindows);

		if (is_right_click)
		{
			Prefab* _selected_prefab = (Prefab*)(GetSelectedResource((int)RES_TYPE::PREFAB));
			if (_selected_prefab != nullptr)
			{
				bool _is_item_hovered = false;

				ImGui::OpenPopup("##OutlinerPopup");
				if (ImGui::BeginPopup("##OutlinerPopup"))
				{
					if (ImGui::Selectable("MakeObject"))
					{
						_selected_prefab->Instantiate();
						is_right_click = false;
					}
					else if (ImGui::IsItemHovered())
						_is_item_hovered = true;

					if ((!_is_item_hovered))
					{
						if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left)
							|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
						{
							is_right_click = false;
						}
					}
					ImGui::EndPopup();
				}
			}

		}
		else if (_is_contentUI_hovered)
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
			{
				RightClickMenu();
			}
		}
		return 0;
	}

	void ContentUI::Reload()
	{
		//if(!is_first)
		//{
		//	ResetContent();
		//	return;
		//}
		//is_first = false;
		// 
		// 
		//// Content 폴더에 있는 파일 이름들을 확인
		//m_vecResPath.clear();
		//wstring strContentPath = wstring(PathMgr::GetInst()->GetContentPath()) + L"Stage2\\";
		//FindFileName(strContentPath);

		//for (size_t i = 0; i < m_vecResPath.size(); ++i)
		//{
		//	RES_TYPE type = GetResTypeByExt(m_vecResPath[i]);
		//		
		//	if (type == RES_TYPE::END)
		//		continue;

		//	switch (type)
		//	{
		//	case RES_TYPE::MESHDATA:
		//		ResourceMgr::GetInst()->MultiThreadLoad<MeshData>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::MATERIAL:
		//		ResourceMgr::GetInst()->MultiThreadLoad<Material>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::PREFAB:
		//		ResourceMgr::GetInst()->MultiThreadLoad<Prefab>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::MESH:
		//		ResourceMgr::GetInst()->MultiThreadLoad<Mesh>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::TEXTURE:
		//		ResourceMgr::GetInst()->MultiThreadLoad<Texture>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::SOUND:
		//		ResourceMgr::GetInst()->MultiThreadLoad<Sound>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::ANIMATION3D:
		//		ResourceMgr::GetInst()->MultiThreadLoad<Animation3D>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	case RES_TYPE::ANIMATOR_CONTROLLER:
		//		ResourceMgr::GetInst()->MultiThreadLoad<AnimatorController>(m_vecResPath[i], m_vecResPath[i]);
		//		break;
		//	default: break;
		//	}
		//}


		m_vecResPath.clear();
		wstring strContentPath = wstring(PathMgr::GetInst()->GetContentPath());
		FindFileName(strContentPath, exclude, L"Stage2");

		for (size_t i = 0; i < m_vecResPath.size(); ++i)
		{
			RES_TYPE type = GetResTypeByExt(m_vecResPath[i]);

			if (type == RES_TYPE::END)
				continue;

			switch (type)
			{
			case RES_TYPE::MESHDATA:
				ResourceMgr::GetInst()->MultiThreadLoad<MeshData>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::MATERIAL:
				ResourceMgr::GetInst()->MultiThreadLoad<Material>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::PREFAB:
				ResourceMgr::GetInst()->MultiThreadLoad<Prefab>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::MESH:
				ResourceMgr::GetInst()->MultiThreadLoad<Mesh>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::TEXTURE:
				ResourceMgr::GetInst()->MultiThreadLoad<Texture>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::SOUND:
				ResourceMgr::GetInst()->MultiThreadLoad<Sound>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::ANIMATION3D:
				ResourceMgr::GetInst()->MultiThreadLoad<Animation3D>(m_vecResPath[i], m_vecResPath[i]);
				break;
			case RES_TYPE::ANIMATOR_CONTROLLER:
				ResourceMgr::GetInst()->MultiThreadLoad<AnimatorController>(m_vecResPath[i], m_vecResPath[i]);
				break;
			default: break;
			}
		}

		ResourceMgr::GetInst()->ResourceThreadWait();
		Device::GetInst()->ExcuteCommadList();

		strContentPath = wstring(PathMgr::GetInst()->GetContentPath());
		// 리소스의 원본파일 체크
		for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
		{
			const map<wstring, Ptr<Resource>>& mapRes = ResourceMgr::GetInst()->GetResources((RES_TYPE)i);

			for (const auto& pair : mapRes)
			{
				if (pair.second->IsEngineRes())
					continue;

				wstring strFilePath = strContentPath + pair.first;
				if (!filesystem::exists(strFilePath))
				{
					Event evn = {};
					evn.type = EVENT_TYPE::DELETE_RESOURCE;
					evn.w_param = (DWORD_PTR)i;
					evn.l_param = (DWORD_PTR)pair.second.Get();
					EventMgr::GetInst()->AddEvent(evn);
				}
			}
		}

		// 트리 갱신
		ResetContent();
	}


	void ContentUI::ResetContent()
	{
		// Tree Clear
		m_Tree->Clear();
		m_Tree->AddItem("Root", 0);

		// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
		for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			const map<wstring, Ptr<Resource>>& mapRes = ResourceMgr::GetInst()->GetResources((RES_TYPE)i);

			// m_Tree 에 현재 리소스 목록을 AddItem
			TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
			pCategory->SetCategoryNode(true);

			for (const auto& pair : mapRes)
			{
				m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
			}
		}
	}

	void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
	{
		TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
		Resource* pSelectObject = (Resource*)pSelectedNode->GetData();

		if (nullptr == pSelectObject)
			return;

		// Inspector 에 선택된 Resource 를 알려준다.	
		InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		pInspector->SetTargetResource(pSelectObject);
		/*if (is_right_click)
		{
			is_item_right_click = true;
		}*/
	}

	void ContentUI::RightClickMenu()
	{
		is_right_click = true;
	}

	DWORD_PTR ContentUI::GetSelectedResource(int type)
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

		if (nullptr == pSelectedNode)
			return 0;
		string _str_type = string(RES_TYPE_STR[type]);
		if (pSelectedNode->GetCategory() == _str_type)
		{
			return pSelectedNode->GetData();
		}
	}


	//void ContentUI::FindFileName(const wstring& _FolderPath)
	//{
	//	WIN32_FIND_DATA FindData = {};
	//
	//	wstring FolderPath = _FolderPath + L"*.*";
	//
	//	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);
	//
	//	while (FindNextFile(hFindHandle, &FindData))
	//	{
	//		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	//		{
	//			if (!wcscmp(FindData.cFileName, L".."))
	//			{
	//				continue;
	//			}
	//
	//			FindFileName(_FolderPath + FindData.cFileName + L"\\");
	//			continue;
	//		}
	//
	//		wstring strContentPath = PathMgr::GetInst()->GetContentPath();
	//		wstring RelativePath = _FolderPath + FindData.cFileName;
	//		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());
	//
	//		m_vecResPath.push_back(RelativePath);
	//	}
	//
	//	FindClose(hFindHandle);
	//}

	void ContentUI::FindFileName(const wstring& _FolderPath, wstring exclude, wstring exclude2)
	{
		WIN32_FIND_DATA FindData = {};
		wstring FolderPath = _FolderPath + L"*.*";
		HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

		if (hFindHandle == INVALID_HANDLE_VALUE) {
			return; // 탐색 시작 실패 시 바로 반환
		}

		do {
			// 현재 폴더가 제외해야 할 폴더인지 확인
			if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !wcscmp(FindData.cFileName, exclude.c_str())
				|| (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !wcscmp(FindData.cFileName, exclude2.c_str())) {
				// 예: "A\D" 폴더를 제외하려면, 현재 _FolderPath가 "A\"이고
				// FindData.cFileName이 "D"인 경우를 건너뛰면 됩니다.
				// 경로 비교 로직에 따라 조건을 추가하거나 변경
				continue;
			}

			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (!wcscmp(FindData.cFileName, L".") || !wcscmp(FindData.cFileName, L"..")) {
					continue;
				}

				// 하위 폴더 탐색
				FindFileName(_FolderPath + FindData.cFileName + L"\\", exclude, exclude2);
				continue;
			}

			wstring strContentPath = PathMgr::GetInst()->GetContentPath();
			wstring RelativePath = _FolderPath + FindData.cFileName;
			RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

			m_vecResPath.push_back(RelativePath);

		} while (FindNextFile(hFindHandle, &FindData));

		FindClose(hFindHandle);
	}
	RES_TYPE ContentUI::GetResTypeByExt(const wstring& _relativepath)
	{
		wchar_t szExt[50] = {};
		_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
		wstring strExt = szExt;

		if (L".mdat" == strExt)
			return RES_TYPE::MESHDATA;
		else if (L".pref" == strExt)
			return RES_TYPE::PREFAB;
		else if (L".mesh" == strExt)
			return RES_TYPE::MESH;
		else if (L".mtrl" == strExt)
			return RES_TYPE::MATERIAL;
		else if (L".png" == strExt || L".jpg" == strExt
			|| L".jpeg" == strExt || L".bmp" == strExt
			|| L".tga" == strExt || L".dds" == strExt
			|| L".hdr" == strExt)
			return RES_TYPE::TEXTURE;
		else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
			return RES_TYPE::SOUND;
		else if (L".anim" == strExt)
			return RES_TYPE::ANIMATION3D;
		else if (L".animctrl" == strExt)
			return RES_TYPE::ANIMATOR_CONTROLLER;
		else if (L".pref" == strExt)
			return RES_TYPE::PREFAB;
		else
			return RES_TYPE::END;
	}

}