#pragma once

#ifndef SCRIP_FUNC
#define SCRIP_FUNC

namespace ff7r
{
    typedef bool (*ImGuiFocusCallback)();
    typedef bool (*ImGuizmoUsingCallback)();
    typedef bool (*MainFocusCallback)();
    typedef void (*LoadLevelFunc)();

    extern ImGuiFocusCallback ImGui_Focus_Callback; // ���� ���� ����
    extern ImGuizmoUsingCallback ImGuizmo_Using_Callback; // ���� ���� ����
    extern MainFocusCallback Main_Focus_Callback; // ���� ���� ����
    extern LoadLevelFunc Load_Level_Func; // ���� ���� ����

    void SetImGuiFocusCallback(ImGuiFocusCallback callback); // �Լ� ����
    bool IsImGuiFocused(); // �Լ� ����
    void SetImGuizmoUsingCallback(ImGuizmoUsingCallback callback); // �Լ� ����
    bool IsImGuizmoUsed(); // �Լ� ����
    void SetMainFocusCallback(MainFocusCallback callback); // �Լ� ����
    bool IsMainFocused(); // �Լ� ����

    void SetLoadLevelFunc(LoadLevelFunc func);
    void CreateLoadLevelFunc();
}

#endif
