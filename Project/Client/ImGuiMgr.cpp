#include "pch.h"
#include "ImGuiMgr.h"

#include <Engine\Device.h>
#include <Engine\LevelMgr.h>
#include <Engine\KeyMgr.h>
#include <Engine\PathMgr.h>
#include <Engine\GameObject.h>

#include <Script\func.h>

#include "UI.h"
#include "ParamUI.h"

#include "InspectorUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "ListUI.h"
#include "MenuUI.h"

namespace ff7r
{
 
    bool ImGuiMgr::IsImGuizmoUsing = false;
    bool ImGuiMgr::IsMainFocusing = false;

    ImGuiMgr::ImGuiMgr()
        : m_hMainHwnd(nullptr)
        , m_hObserver(nullptr)
        , is_update(true)
    {
        
    }

    ImGuiMgr::~ImGuiMgr()
    {
        // ImGui Release
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        // UI 삭제
        SafeDelMap(m_mapUI);
    }


    void ImGuiMgr::Init(HWND _hWnd)
    {
        m_hMainHwnd = _hWnd;

        // ImGui 초기화
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        //io.ConfigViewportsNoDefaultParent = true;
        //io.ConfigDockingAlwaysTabBar = true;
        //io.ConfigDockingTransparentPayload = true;
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(m_hMainHwnd);
        ImGui_ImplDX11_Init(DEVICE, CONTEXT);

        // Tool 용 UI 생성
        CreateUI();

        // Content 폴더 감시
        wstring strContentPath = PathMgr::GetInst()->GetContentPath();
        m_hObserver = FindFirstChangeNotification(strContentPath.c_str(), true
            , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
            | FILE_ACTION_REMOVED | FILE_ACTION_ADDED);

        SetMainFocusCallback([]()->bool { return ImGuiMgr::IsMainFocused(); });
        SetImGuizmoUsingCallback([]()->bool {return ImGuiMgr::IsImGuizmoUsed(); });
    }

    void ImGuiMgr::progress()
    {
        if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::I))
        {
            is_update = !is_update;
        }
        if (is_update)
        {
            Begin();

            Tick();
            FinalTick();

            Render();

            // Content 폴더 변경 감시
            ObserveContent();
        }
    }

    void ImGuiMgr::ChangeLevel(wstring stage)
    {
        if (stage == L"Stage1")
        {
            ContentUI* UI = (ContentUI*)FindUI("##Content");
            UI->SetExcludeFile(L"Stage2");
            UI->Reload();
        }
        else
        {
            ContentUI* UI = (ContentUI*)FindUI("##Content");
            UI->SetExcludeFile(L"Stage1");
            UI->Reload();
        }
    }



    void ImGuiMgr::Begin()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ParamUI::g_NextId = 0;

        IsImGuizmoUsing = false;
        IsMainFocusing = false;
    }

    void ImGuiMgr::Tick()
    {
        for (const auto& pair : m_mapUI)
        {
            pair.second->Tick();
        }
    }

    void ImGuiMgr::FinalTick()
    {
        // Demo UI
        ImGui::ShowDemoWindow();

        // InspectorUI
        for (const auto& pair : m_mapUI)
        {
            if (pair.second->IsActive())
            {
                pair.second->FinalTick();
            }
        }
       

        if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        {
            IsMainFocusing = true;
        }
        if (KEY_TAP(KEY::ENTER))
            ImGui::SetWindowFocus(nullptr);
    }

    void ImGuiMgr::Render()
    {
        // ImGui Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }




    void ImGuiMgr::CreateUI()
    {
        UI* pUI = nullptr;

        // InspectorUI
        pUI = new InspectorUI;
        pUI->SetActive(true);
        m_mapUI.insert(make_pair(pUI->GetID(), pUI));

        // ContentUI
        pUI = new ContentUI;
        pUI->SetActive(true);
        m_mapUI.insert(make_pair(pUI->GetID(), pUI));

        // Outliner
        pUI = new OutlinerUI;
        pUI->SetActive(true);
        m_mapUI.insert(make_pair(pUI->GetID(), pUI));

        // Menu
        pUI = new MenuUI;
        pUI->SetActive(true);
        m_mapUI.insert(make_pair(pUI->GetID(), pUI));


        // ListUI
        pUI = new ListUI;
        pUI->SetModal(true);
        pUI->SetActive(false);
        m_mapUI.insert(make_pair(pUI->GetID(), pUI));


        for (const auto& pair : m_mapUI)
        {
            pair.second->Init();
        }
    }

    void ImGuiMgr::ObserveContent()
    {
        DWORD dwWaitStatus = WaitForSingleObject(m_hObserver, 0);

        if (dwWaitStatus == WAIT_OBJECT_0)
        {
            // content 폴더에 변경점이 생겼다.
            ContentUI* UI = (ContentUI*)FindUI("##Content");
            UI->Reload();

            FindNextChangeNotification(m_hObserver);
        }
    }


    UI* ImGuiMgr::FindUI(const string& _UIName)
    {
        map<string, UI*>::iterator iter = m_mapUI.find(_UIName);

        if (iter == m_mapUI.end())
            return nullptr;

        return iter->second;
    }
}