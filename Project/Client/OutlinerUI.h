#pragma once
#include "UI.h"

namespace ff7r
{
    class TreeUI;
    class TreeNode;
    class OutlinerUI : public UI
    {
    private:
        TreeUI* m_Tree;
        DWORD_PTR   m_dwSelectedData;

        bool is_right_click;
        bool is_item_right_click;

        float cam_view_mat[16];
        float cam_proj_mat[16];

    public:
        virtual void Tick() override;
        virtual int RenderUpdate() override;

    public:
        void ResetOutliner();
        void SetTargetToInspector(DWORD_PTR _SelectedNode);
        GameObject* GetSelectedObject();

        bool EditTransform(float* matrix, bool editTransformDecomposition);
        void RenderGuizmo();
        void SetRightClick(bool _click) { is_right_click = _click; }

        // 트리가 리셋된 경우, 입력으로 들어온 데이터와 동일한 노드를  선택상태로 둔다.
        void SetSelectedNodeData(DWORD_PTR _data)
        {
            m_dwSelectedData = _data;
        }
        void ClearSelectedNodeData()
        {
            m_dwSelectedData = 0;
        }

    private:
        void AddGameObject(GameObject* _Obj, TreeNode* _ParentNode);
        void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);
        void RightClickMenu();
        void CreateEmptyObject();

    public:
        OutlinerUI();
        ~OutlinerUI();
    };

}