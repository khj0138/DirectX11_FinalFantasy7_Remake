#pragma once
#include "ComponentUI.h"
namespace ff7r
{
    class Animator3D;
    class ListUI;
    struct GraphEditorDelegate : public GraphEditor::Delegate
    {
        bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override
        {
            return true;
        }

        void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
        {
            mNodes[nodeIndex].mSelected = selected;
        }
        void HoverNode(GraphEditor::NodeIndex nodeIndex) override
        {
            hovered_node_idx = nodeIndex;
        }

        void MoveSelectedNodes(const ImVec2 delta) override
        {
            for (auto& node : mNodes)
            {
                if (!node.mSelected)
                {
                    continue;
                }
                node.x += delta.x;
                node.y += delta.y;
            }
        }

        virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override
        {
        }

        void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
        {
            mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });
        }

        void DelLink(GraphEditor::LinkIndex linkIndex) override
        {
            mLinks.erase(mLinks.begin() + linkIndex);
        }

        void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) override
        {
            drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
            drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), "Draw");
        }

        const size_t GetTemplateCount() override
        {
            return sizeof(mTemplates) / sizeof(GraphEditor::Template);
        }

        const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
        {
            return mTemplates[index];
        }

        const size_t GetNodeCount() override
        {
            return mNodes.size();
        }

        const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
        {
            const auto& myNode = mNodes[index];
            return GraphEditor::Node
            {
                myNode.name,
                myNode.templateIndex,
                ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 200, myNode.y + 200)),
                myNode.mSelected
            };
        }

        const size_t GetLinkCount() override
        {
            return mLinks.size();
        }

        const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
        {
            return mLinks[index];
        }


        vector<GraphEditor::Template> mTemplates;
        /*static const inline GraphEditor::Template mTemplates[] = {
            {
                IM_COL32(160, 160, 180, 255),
                IM_COL32(100, 100, 140, 255),
                IM_COL32(110, 110, 150, 255),
                1,
                Array{"MyInput"},
                Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
                1,
                Array{"MyOutput0", "MyOuput1"},
                Array{ IM_COL32(200,200,200,255)}
            }
        };*/

        struct Node
        {
            const char* name;
            GraphEditor::TemplateIndex templateIndex;
            float x, y;
            bool mSelected;
            bool mHovered;
        };

        std::vector<Node> mNodes;

        std::vector<GraphEditor::Link> mLinks;

        int hovered_node_idx = -1;
    };

    static const char* SequencerItemTypeNames[] = { "Animation" };
    struct RampEdit : public ImCurveEdit::Delegate
    {
        RampEdit()
        {
            mPts[0][0] = ImVec2(-10.f, 0);
            mPts[0][1] = ImVec2(20.f, 0.6f);
            mPts[0][2] = ImVec2(25.f, 0.2f);
            mPts[0][3] = ImVec2(70.f, 0.4f);
            mPts[0][4] = ImVec2(120.f, 1.f);
            mPointCount[0] = 5;

            mPts[1][0] = ImVec2(-50.f, 0.2f);
            mPts[1][1] = ImVec2(33.f, 0.7f);
            mPts[1][2] = ImVec2(80.f, 0.2f);
            mPts[1][3] = ImVec2(82.f, 0.8f);
            mPointCount[1] = 4;


            mPts[2][0] = ImVec2(40.f, 0);
            mPts[2][1] = ImVec2(60.f, 0.1f);
            mPts[2][2] = ImVec2(90.f, 0.82f);
            mPts[2][3] = ImVec2(150.f, 0.24f);
            mPts[2][4] = ImVec2(200.f, 0.34f);
            mPts[2][5] = ImVec2(250.f, 0.12f);
            mPointCount[2] = 6;
            mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
            mMax = ImVec2(1.f, 1.f);
            mMin = ImVec2(0.f, 0.f);
        }
        size_t GetCurveCount()
        {
            return 3;
        }

        bool IsVisible(size_t curveIndex)
        {
            return mbVisible[curveIndex];
        }
        size_t GetPointCount(size_t curveIndex)
        {
            return mPointCount[curveIndex];
        }

        uint32_t GetCurveColor(size_t curveIndex)
        {
            uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
            return cols[curveIndex];
        }
        ImVec2* GetPoints(size_t curveIndex)
        {
            return mPts[curveIndex];
        }
        virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveNone; }
        virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
        {
            mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
            SortValues(curveIndex);
            for (size_t i = 0; i < GetPointCount(curveIndex); i++)
            {
                if (mPts[curveIndex][i].x == value.x)
                    return (int)i;
            }
            return pointIndex;
        }
        virtual void AddPoint(size_t curveIndex, ImVec2 value)
        {
            if (mPointCount[curveIndex] >= 8)
                return;
            mPts[curveIndex][mPointCount[curveIndex]++] = value;
            SortValues(curveIndex);
        }
        virtual ImVec2& GetMax() { return mMax; }
        virtual ImVec2& GetMin() { return mMin; }
        virtual unsigned int GetBackgroundColor() { return 0; }
        ImVec2 mPts[3][8];
        size_t mPointCount[3];
        bool mbVisible[3];
        ImVec2 mMin;
        ImVec2 mMax;
    private:
        void SortValues(size_t curveIndex)
        {
            auto b = std::begin(mPts[curveIndex]);
            auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
            std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });

        }
    };
    struct MySequence : public ImSequencer::SequenceInterface
    {
        // interface with sequencer

        virtual int GetFrameMin() const {
            return mFrameMin;
        }
        virtual int GetFrameMax() const {
            return mFrameMax;
        }
        virtual int GetItemCount() const { return (int)myItems.size(); }

        virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
        virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
        virtual const char* GetItemLabel(int index) const
        {
            static char tmps[512];
            snprintf(tmps, 512, "%s", SequencerItemTypeNames[myItems[index].mType]);
            return tmps;
        }

        virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
        {
            MySequenceItem& item = myItems[index];
            if (color)
                *color = 0xFFAA8080; // same color for everyone, return color based on type
            if (start)
                *start = &item.mFrameStart;
            if (end)
                *end = &item.mFrameEnd;
            if (type)
                *type = item.mType;
        }
        virtual void Add(int type) { myItems.push_back(MySequenceItem{ type, 0, 10, false }); };
        virtual void Del(int index) { myItems.erase(myItems.begin() + index); }
        virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

        virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

        // my datas
        MySequence() : mFrameMin(0), mFrameMax(0) {}
        int mFrameMin, mFrameMax;
        struct MySequenceItem
        {
            int mType;
            int mFrameStart, mFrameEnd;
            bool mExpanded;
        };
        std::vector<MySequenceItem> myItems;
        RampEdit rampEdit;

        virtual void DoubleClick(int index) {
            if (myItems[index].mExpanded)
            {
                myItems[index].mExpanded = false;
                return;
            }
            for (auto& item : myItems)
                item.mExpanded = false;
            myItems[index].mExpanded = !myItems[index].mExpanded;
        }

        virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
        {
            //static const char* labels[] = { "Translation", "Rotation" , "Scale" };

            rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
            rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
            draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
            ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y);
            ImVec2 ptb(legendRect.Max.x, legendRect.Min.y);
            if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
                rampEdit.mbVisible[0] = !rampEdit.mbVisible[0];
            draw_list->AddText(pta, rampEdit.mbVisible[0] ? 0xFFFFFFFF : 0x80FFFFFF, "frames");
            /* for (int i = 0; i < 3; i++)
             {
                 ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
                 ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
                 draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
                 if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
                     rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
             }*/
            draw_list->PopClipRect();

            ImGui::SetCursorScreenPos(rc.Min);
            ImCurveEdit::Edit(rampEdit, rc.Max - rc.Min, 137 + index, &clippingRect);

        }

        virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
        {
            rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
            rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
            draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < rampEdit.mPointCount[i]; j++)
                {
                    float p = rampEdit.mPts[i][j].x;
                    if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
                        continue;
                    float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
                    float x = ImLerp(rc.Min.x, rc.Max.x, r);
                    draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
                }
            }
            draw_list->PopClipRect();
        }
    };

    class Animator3DUI : public ComponentUI
    {

        struct ControllerInfo
        {
            GraphEditor::Options options;
            GraphEditorDelegate delegate;
            GraphEditor::ViewState viewState;
            GraphEditor::FitOnScreen fit;
        };
    public:
        virtual int RenderUpdate() override;
        Animator3D* selected_animator;
        wstring     selected_state_name;
        int         selected_transition_idx;
        int         selected_event_idx;
        bool        selected_state_hovered;
        wstring     selected_parameter_name;
        wstring     changed_parameter_name;

        bool is_activate_controller;
        bool is_state_right_click;
        bool is_param_right_click;
        bool is_item_right_click;
        bool is_activate_anim_manager;
        bool is_add_parameter;
        bool is_rename_parameter;
        bool is_transition_check;

        ControllerInfo controller_info;

        //char** animations_list;
        vector<wstring> animations_list;
        vector<wstring> parameters_list;
        vector<wstring> events_list;
        int             selected_animation_idx;
        int             info_animation_idx;
        vector<float> frames_list;
        float** frames_graph_data;
        char* text_buf[2];
        int int_buf[3];
        vector<char> text_vec;
        //float** frames_time;

    public:
        Animator3DUI();
        ~Animator3DUI();
        void ShowAnimationsList();
    };
}
