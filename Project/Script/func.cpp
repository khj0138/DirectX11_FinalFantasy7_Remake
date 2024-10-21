#include "pch.h"
#include "func.h"

namespace ff7r
{
    ImGuiFocusCallback ImGui_Focus_Callback = nullptr; // 전역 변수 정의
    ImGuizmoUsingCallback ImGuizmo_Using_Callback = nullptr;
    MainFocusCallback Main_Focus_Callback = nullptr;
    LoadLevelFunc Load_Level_Func = nullptr;
    void SetImGuiFocusCallback(ImGuiFocusCallback callback) // 함수 정의
    {
        ImGui_Focus_Callback = callback;
    }

    bool IsImGuiFocused() // 함수 정의
    {
        if (ImGui_Focus_Callback)
        {
            return ImGui_Focus_Callback();
        }
        return false;
    }

    void SetImGuizmoUsingCallback(ImGuizmoUsingCallback callback) // 함수 정의
    {
        ImGuizmo_Using_Callback = callback;
    }
    bool IsImGuizmoUsed() // 함수 정의
    {
        if (ImGuizmo_Using_Callback)
        {
            return ImGuizmo_Using_Callback();
        }
        return false;
    }// 함수 선언

    void SetMainFocusCallback(MainFocusCallback callback) // 함수 정의
    {
        Main_Focus_Callback = callback;
    }
    bool IsMainFocused() // 함수 정의
    {
        if (Main_Focus_Callback)
        {
            return Main_Focus_Callback();
        }
        return false;
    }
    void CreateLoadLevelFunc()
    {
        Load_Level_Func();
    }
    void SetLoadLevelFunc(LoadLevelFunc func)
    {
        Load_Level_Func = func;
    }
    // 함수 선언
}