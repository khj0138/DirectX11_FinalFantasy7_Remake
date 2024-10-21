#pragma once

#ifndef SCRIP_FUNC
#define SCRIP_FUNC

namespace ff7r
{
    typedef bool (*ImGuiFocusCallback)();
    typedef bool (*ImGuizmoUsingCallback)();
    typedef bool (*MainFocusCallback)();
    typedef void (*LoadLevelFunc)();

    extern ImGuiFocusCallback ImGui_Focus_Callback; // 전역 변수 선언
    extern ImGuizmoUsingCallback ImGuizmo_Using_Callback; // 전역 변수 선언
    extern MainFocusCallback Main_Focus_Callback; // 전역 변수 선언
    extern LoadLevelFunc Load_Level_Func; // 전역 변수 선언

    void SetImGuiFocusCallback(ImGuiFocusCallback callback); // 함수 선언
    bool IsImGuiFocused(); // 함수 선언
    void SetImGuizmoUsingCallback(ImGuizmoUsingCallback callback); // 함수 선언
    bool IsImGuizmoUsed(); // 함수 선언
    void SetMainFocusCallback(MainFocusCallback callback); // 함수 선언
    bool IsMainFocused(); // 함수 선언

    void SetLoadLevelFunc(LoadLevelFunc func);
    void CreateLoadLevelFunc();
}

#endif
