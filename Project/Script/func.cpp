#include "pch.h"
#include "func.h"

namespace ff7r
{
    ImGuiFocusCallback ImGui_Focus_Callback = nullptr; // ���� ���� ����
    ImGuizmoUsingCallback ImGuizmo_Using_Callback = nullptr;
    MainFocusCallback Main_Focus_Callback = nullptr;
    LoadLevelFunc Load_Level_Func = nullptr;
    void SetImGuiFocusCallback(ImGuiFocusCallback callback) // �Լ� ����
    {
        ImGui_Focus_Callback = callback;
    }

    bool IsImGuiFocused() // �Լ� ����
    {
        if (ImGui_Focus_Callback)
        {
            return ImGui_Focus_Callback();
        }
        return false;
    }

    void SetImGuizmoUsingCallback(ImGuizmoUsingCallback callback) // �Լ� ����
    {
        ImGuizmo_Using_Callback = callback;
    }
    bool IsImGuizmoUsed() // �Լ� ����
    {
        if (ImGuizmo_Using_Callback)
        {
            return ImGuizmo_Using_Callback();
        }
        return false;
    }// �Լ� ����

    void SetMainFocusCallback(MainFocusCallback callback) // �Լ� ����
    {
        Main_Focus_Callback = callback;
    }
    bool IsMainFocused() // �Լ� ����
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
    // �Լ� ����
}