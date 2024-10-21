#pragma once
#include <Engine\Singleton.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"

#include "ImGui\GraphEditor.h"
#include "ImGui\ImCurveEdit.h"
#include "ImGui\ImGradient.h"
#include "ImGui\ImGuizmo.h"
#include "ImGui\ImSequencer.h"
#include "ImGui\ImZoomSlider.h"

#include "ImGui\implot.h"
#include "ImGui\implot_internal.h"



namespace ff7r
{
    // ========
    // Delegate
    // ========
    class UI;

    typedef void (UI::* UI_DELEGATE)(void);
    typedef void (UI::* UI_DELEGATE_1)(DWORD_PTR);
    typedef void (UI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

    class ImGuiMgr :
        public Singleton<ImGuiMgr>
    {
        SINGLE(ImGuiMgr);
    private:
        HWND                m_hMainHwnd;
        map<string, UI*>    m_mapUI;

        HANDLE              m_hObserver;


    public:
        void Init(HWND _hWnd);
        void progress();
        void ChangeLevel(wstring stage);

        static bool                IsImGuizmoUsing;
        static bool                IsMainFocusing;

    public:
        UI* FindUI(const string& _UIName);
        HWND GetMainHwnd() { return m_hMainHwnd; }
        static bool IsImGuizmoUsed() { return IsImGuizmoUsing; }
        static bool IsMainFocused() { return IsMainFocusing; }

    private:
        void CreateUI();
        void ObserveContent();

        void Begin();
        void Tick();
        void FinalTick();
        void Render();

        bool is_update;
    };

}