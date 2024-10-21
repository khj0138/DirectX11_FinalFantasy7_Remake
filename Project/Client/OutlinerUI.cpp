#include "pch.h"
#include "OutlinerUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\EventMgr.h>
#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>
#include <Engine\Transform.h>
#include <Engine\RenderMgr.h>
#include <Engine\Camera.h>

#include "TreeUI.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
bool useWindow = true;

namespace ff7r
{
	OutlinerUI::OutlinerUI()
		: UI("##Outliner")
		, m_Tree(nullptr)
	{
		SetName("Outliner");

		// OutlinerUI 안에 자식으로 Tree 를 추가한다.
		m_Tree = new TreeUI;
		m_Tree->SetName("OutlinerTree");
		m_Tree->SetActive(true);
		m_Tree->ShowRoot(false);

		m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&OutlinerUI::SetTargetToInspector);
		m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&OutlinerUI::DragDrop);
		m_Tree->SetDragDropID("GameObject");


		AddChildUI(m_Tree);
	}

	OutlinerUI::~OutlinerUI()
	{
	}

	void OutlinerUI::Tick()
	{
		if (EventMgr::GetInst()->IsLevelChanged())
		{
			ResetOutliner();

			// 트리 리셋 후, 다음 선택노드로 지정된 DATA 가 있으면, 찾아서 선택노드로 변경
			if (0 != m_dwSelectedData)
			{
				m_Tree->GetSelectedNode(m_dwSelectedData);
			}
		}
		RenderGuizmo();
	}

	int OutlinerUI::RenderUpdate()
	{
		bool _is_OutlinerUI_Hovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_ChildWindows);


		if (is_right_click)
		{
			bool _is_active = IsActive();

			ImGui::OpenPopup("##OutlinerPopup");
			if (ImGui::BeginPopup("##OutlinerPopup"))
			{
				if (is_item_right_click)
				{
					const char* _select_menu[4] = { "Rename", "Duplicate", "Delete", "CreatePrefab" };
					bool _is_selected[4] = { false };
					bool _is_item_hovered = false;

					for (int i = 0; i < 4; i++)
					{
						if (ImGui::Selectable(_select_menu[i], &_is_selected[i]))
						{
							is_item_right_click = false;
							is_right_click = false;

							if (i == 2)
							{
								Event evn = {};
								evn.type = EVENT_TYPE::DELETE_OBJECT;
								evn.w_param = DWORD_PTR(GetSelectedObject());
								EventMgr::GetInst()->AddEvent(evn);

								InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
								inspector->ClearTargetObject();
								// inspector->ClearTargetResource();
							}
							else if (i == 3)
							{
								Event evn = {};
								evn.type = EVENT_TYPE::CREATE_PREFAB;
								evn.w_param = DWORD_PTR(GetSelectedObject());
								evn.l_param = DWORD_PTR(&(GetSelectedObject()->GetName()));
								EventMgr::GetInst()->AddEvent(evn);
							}

						}
						else if (ImGui::IsItemHovered())
							_is_item_hovered = true;

					}
					if ((!_is_item_hovered))
					{
						if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left)
							|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
						{
							is_right_click = false;
							is_item_right_click = false;
						}
					}
				}
				else
				{
					if (ImGui::Selectable("Create Empty Object"))
					{
						CreateEmptyObject();
						is_right_click = false;
					}
					else if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
					{
						is_right_click = false;
					}
				}
				ImGui::EndPopup();
			}

		}
		else if (_is_OutlinerUI_Hovered)
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
			{
				RightClickMenu();
				if (is_item_right_click)
					is_item_right_click = false;
			}
		}
		return 0;
	}


	void OutlinerUI::ResetOutliner()
	{
		// Tree Clear
		m_Tree->Clear();
		m_Tree->AddItem("Root", 0);

		// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
		Level* pCurLevel = LevelMgr::GetInst()->GetCurLevel();

		for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
		{
			Layer* pLayer = pCurLevel->GetLayer(i);

			const vector<GameObject*>& vecParentObj = pLayer->GetParentObject();

			for (size_t i = 0; i < vecParentObj.size(); ++i)
			{
				AddGameObject(vecParentObj[i], nullptr);
			}
		}
	}

	void OutlinerUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
	{
		TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
		GameObject* pSelectObject = (GameObject*)pSelectedNode->GetData();

		// Inspector 에 선택된 GameObject 를 알려준다.	
		InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		pInspector->SetTargetObject(pSelectObject);
		if (is_right_click)
		{
			is_item_right_click = true;
		}

	}


	void OutlinerUI::AddGameObject(GameObject* _Obj, TreeNode* _ParentNode)
	{
		// 오브젝트를 트리에 넣고, 생성된 노드 주소를 받아둔다.
		TreeNode* pNode = m_Tree->AddItem(string(_Obj->GetName().begin(), _Obj->GetName().end())
			, (DWORD_PTR)_Obj
			, _ParentNode);

		// 오브젝트의 자식오브젝트 들을 오브젝트 노드를 부모로 해서 그 밑으로 다시 넣어준다.
		const vector<GameObject*>& vecChild = _Obj->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			AddGameObject(vecChild[i], pNode);
		}
	}


	GameObject* OutlinerUI::GetSelectedObject()
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

		if (nullptr == pSelectedNode)
			return nullptr;

		return (GameObject*)pSelectedNode->GetData();
	}

	bool OutlinerUI::EditTransform(float* matrix, bool editTransformDecomposition)
	{
		bool _manipulated = false;
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		{
			if (ImGui::IsKeyPressed(ImGuiKey_1))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (ImGui::IsKeyPressed(ImGuiKey_2))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			if (ImGui::IsKeyPressed(ImGuiKey_3))
				mCurrentGizmoOperation = ImGuizmo::SCALE;
		}

		ImGuiIO& io = ImGui::GetIO();
		float viewManipulateRight = io.DisplaySize.x;
		float viewManipulateTop = 0;
		static ImGuiWindowFlags gizmoWindowFlags = 0;

		if (useWindow)
		{
			ImGuiContext& g = *GImGui;
			ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();


			ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Appearing);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
			ImGui::SetNextWindowBgAlpha(0.0f);
			gizmoWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGuizmo::AllowAxisFlip(false);

			ImGui::Begin("##Default2", 0, gizmoWindowFlags);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
			viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
			viewManipulateTop = ImGui::GetWindowPos().y;
			ImGuiWindow* window = ImGui::GetCurrentWindow();
		}
		else
		{
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		}


		if (ImGuizmo::Manipulate(cam_view_mat, cam_proj_mat, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL))
		{
			_manipulated = true;
		}
		if (ImGuizmo::IsUsing())
			ff7r::ImGuiMgr::IsImGuizmoUsing = true;

		if (useWindow)
		{
			if (ImGui::IsWindowFocused())
				ImGuiMgr::GetInst()->IsMainFocusing = true;
			ImGui::End();
			ImGui::PopStyleColor(1);
		}
		return _manipulated;

	}

	void OutlinerUI::RenderGuizmo()
	{
		Matrix _cam_view_mat = RenderMgr::GetInst()->GetEditorCam()->GetViewMat();
		Matrix _cam_proj_mat = RenderMgr::GetInst()->GetEditorCam()->GetProjMat();
		memcpy(cam_view_mat, &_cam_view_mat, sizeof(cam_view_mat));
		memcpy(cam_proj_mat, &_cam_proj_mat, sizeof(cam_proj_mat));


		GameObject* _selected_object = GetSelectedObject();

		if (_selected_object != nullptr)//&& _selected_object->GetMeshRender() != nullptr)
		{
			int _id = 0;


			ImGuizmo::SetID(_id);
			//_id++;
			float _temp_mat[16];

			/* _relative_scale = _selected_object->GetTransform()->GetRelativeScale();
			Matrix _matScale = XMMatrixScaling(_relative_scale.x, _relative_scale.y, _relative_scale.z);

			vec3 _relative_rotation = _selected_object->GetTransform()->GetRelativeRot();
			Matrix _matRot = XMMatrixIdentity();
			_matRot = XMMatrixRotationX(_relative_rotation.x);
			_matRot *= XMMatrixRotationY(_relative_rotation.y);
			_matRot *= XMMatrixRotationZ(_relative_rotation.z);

			vec3 _relative_pos = _selected_object->GetTransform()->GetRelativePos();
			Matrvec3ix _matPos = XMMatrixTranslation(_relative_pos.x, _relative_pos.y, _relative_pos.z);

			Matrix _obj_world_mat = _matScale * _matRot * _matPos;*/
			Matrix _obj_world_mat = _selected_object->GetTransform()->GetWorldMat();

			memcpy(_temp_mat, &_obj_world_mat, sizeof(_temp_mat));
			if (EditTransform(_temp_mat, false))
			{
				GameObject* _parent = _selected_object->GetParent();
				if (_parent)
				{

					_obj_world_mat = XMMATRIX(_temp_mat);
					Matrix _parent_mat = XMMatrixInverse(nullptr, _parent->GetTransform()->GetWorldMat());
					//Matrix _parent_scale = XMMatrixInverse(nullptr, _parent->GetTransform()->GetWorldScaleMat());
					Matrix _parent_scale = _parent->GetTransform()->GetWorldScaleMat();
					_obj_world_mat = _obj_world_mat * _parent_mat * _parent_scale;
					memcpy(_temp_mat, &_obj_world_mat, sizeof(_temp_mat));
				}


				float _scale[3];
				float _rotation[3];
				float _translation[3];

				ImGuizmo::DecomposeMatrixToComponents(_temp_mat, _translation, _rotation, _scale);

				for (int i = 0; i < 3; i++)
				{
					_rotation[i] *= XM_PI / 180.0f;
				}
				_selected_object->GetTransform()->SetRelativePos(vec3(_translation[0], _translation[1], _translation[2]));
				_selected_object->GetTransform()->SetRelativeRot(vec3(_rotation[0], _rotation[1], _rotation[2]));
				_selected_object->GetTransform()->SetRelativeScale(vec3(_scale[0], _scale[1], _scale[2]));
			}
		}

	}


	void OutlinerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
	{
		TreeNode* pDragNode = (TreeNode*)_DragNode;
		TreeNode* pDropNode = (TreeNode*)_DropNode;

		GameObject* pDragObj = (GameObject*)pDragNode->GetData();
		GameObject* pDropObj = nullptr;
		if (nullptr != pDropNode)
		{
			pDropObj = (GameObject*)pDropNode->GetData();
		}

		// 자식으로 들어갈 오브젝트가 목적지 오브젝트의 조상(부모계층) 중 하나라면, 
		// AddChild 처리하지 않는다.
		if (nullptr != pDropObj)
		{
			if (pDropObj->IsAncestor(pDragObj))
				return;
		}

		// 이벤트 매니저를 통해서 처리한다.
		Event evn = {};
		evn.type = EVENT_TYPE::ADD_CHILD;
		evn.w_param = (DWORD_PTR)pDropObj;
		evn.l_param = (DWORD_PTR)pDragObj;
		EventMgr::GetInst()->AddEvent(evn);
	}

	void OutlinerUI::RightClickMenu()
	{
		is_right_click = true;
	}

	void OutlinerUI::CreateEmptyObject()
	{
		GameObject* pNewObject = new GameObject;
		pNewObject->AddComponent(new Transform);
		pNewObject->SetName(L"New Object");
		SpawnGameObject(pNewObject, vec3(0.f, 0.f, 0.f), L"Tile");

		SetSelectedNodeData(DWORD_PTR(pNewObject));
	}
}