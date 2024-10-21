#include "pch.h"
#include "MenuUI.h"

#include "LevelSaveLoad.h"
#include <Engine\EventMgr.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>
#include <Engine\Script.h>
#include <Engine/LevelMgr.h>

#include <Script\ScriptMgr.h>


#include "ImGuiMgr.h"
#include "OutlinerUI.h"
#include "InspectorUI.h"
#include "LevelSaveLoad.h"
#include "ListUI.h"

namespace ff7r
{

    MenuUI::MenuUI()
        : UI("##Menu")
    {
        SetName("Menu");
    }

    MenuUI::~MenuUI()
    {
    }

    void MenuUI::FinalTick()
    {
        if (!IsActive())
            return;

        RenderUpdate();
    }

    int MenuUI::RenderUpdate()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save Level"))
                {
                    time_t raw_time;
                    time(&raw_time);
                    struct tm cur_time; // tm 구조체를 스택에 할당
                    localtime_s(&cur_time, &raw_time);

                    wstring time_wstr =
                        to_wstring((cur_time.tm_mon + 1)) + L"." + to_wstring((cur_time.tm_mday)) + L" "
                        + to_wstring((cur_time.tm_hour + 1)) + L"." + to_wstring((cur_time.tm_min));
                    // Level 저장
                    LevelSaveLoad::SaveLevel(L"Level\\" + LevelMgr::GetInst()->GetCurLevel()->GetName() + time_wstr + L".lv"
                        , LevelMgr::GetInst()->GetCurLevel(), true);
                }
                else if (ImGui::MenuItem("Save Level with different name"))
                {
                    // Level 저장
                    LevelSaveLoad::SaveLevel(L"Level\\TestLevel.lv", LevelMgr::GetInst()->GetCurLevel(), false);
                }

                if (ImGui::MenuItem("Load Level"))
                {
                    vector<wstring> _level_list;
                    wstring _level_folder_path = PathMgr::GetInst()->GetContentPath();
                    _level_folder_path += L"Level\\";

                    LoadFileList(_level_folder_path, _level_list);

                    ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                    pListUI->Reset("LoadLevel", ImVec2(300.0f, 500.0f));
                    for (auto level_name : _level_list)
                    {
                        string _str_level_name;
                        _str_level_name.assign(level_name.begin(), level_name.end());
                        pListUI->AddItem(_str_level_name);
                    }
                    pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MenuUI::LoadLevel);

                }

                ImGui::EndMenu();
            }
            //if (ImGui::BeginMenu("GameObject"))
            //{
            //    // 현재 레벨에 게임오브젝트 생성
            //    if (ImGui::MenuItem("Create Empty Object"))
            //    {
            //        CreateEmptyObject();
            //    }
            //    ImGui::Separator();


            //    if (ImGui::BeginMenu("Add Component"))
            //    {
            //        for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
            //        {
            //            if (ImGui::MenuItem(ToString((COMPONENT_TYPE)i)))
            //            {
            //                AddComponent(COMPONENT_TYPE(i));
            //            }
            //        }
            //        ImGui::EndMenu();
            //    }

            //    ImGui::Separator();

            //    if (ImGui::BeginMenu("Add Script"))
            //    {
            //        vector<wstring> vecScripts;
            //        ScriptMgr::GetScriptInfo(vecScripts);

            //        for (size_t i = 0; i < vecScripts.size(); ++i)
            //        {
            //            string strScriptName = string(vecScripts[i].begin(), vecScripts[i].end());
            //            if (ImGui::MenuItem(strScriptName.c_str()))
            //            {
            //                AddScript(vecScripts[i]);
            //            }
            //        }

            //        ImGui::EndMenu();
            //    }

            //    ImGui::EndMenu();
            //}

            if (ImGui::BeginMenu("Level"))
            {
                Level* CurLevel = LevelMgr::GetInst()->GetCurLevel();
                bool PlayEnable = true;
                bool PauseEnable = true;
                bool StopEnable = true;

                if (CurLevel->GetState() == LEVEL_STATE::PLAY)
                {
                    PlayEnable = false;
                    PauseEnable = true;
                    StopEnable = true;
                }
                else if (CurLevel->GetState() == LEVEL_STATE::PAUSE)
                {
                    PlayEnable = true;
                    PauseEnable = false;
                    StopEnable = true;
                }
                else if (CurLevel->GetState() == LEVEL_STATE::STOP)
                {
                    PlayEnable = true;
                    PauseEnable = false;
                    StopEnable = false;
                }



                if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
                {
                    LevelSaveLoad::SaveLevel(L"Level\\Temp.lv", CurLevel, true);
                    CurLevel->ChangeState(LEVEL_STATE::PLAY);
                }
                else if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
                {
                    CurLevel->ChangeState(LEVEL_STATE::PAUSE);
                }
                else if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
                {
                    CurLevel->ChangeState(LEVEL_STATE::STOP);
                    Level* pNewLevel = LevelSaveLoad::LoadLevel(L"Level\\Temp.lv");

                    Event evn = {};
                    evn.type = EVENT_TYPE::LEVEL_CHANGE;
                    evn.w_param = DWORD_PTR(pNewLevel);
                    EventMgr::GetInst()->AddEvent(evn);

                    // InspectorUI
                    InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
                    Inspector->SetTargetObject(nullptr);
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Resource"))
            {
                if (ImGui::MenuItem("Create Emty Material"))
                {
                    CreateEmptyMaterial();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        return 0;
    }

    void MenuUI::CreateEmptyObject()
    {
        GameObject* pNewObject = new GameObject;
        pNewObject->AddComponent(new Transform);
        pNewObject->SetName(L"New Object");
        SpawnGameObject(pNewObject, vec3(0.f, 0.f, 0.f), L"Default");

        // Outliner 를 가져온다.
        OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

        // 새로추가된 오브젝트를 데이터로 하는 노드가 추가되면, 선택상태로 두게 한다.
        outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));
    }

    void MenuUI::CreateEmptyMaterial()
    {
        Ptr<Material> pNewMtrl = new Material;
        ResourceMgr::GetInst()->AddRes<Material>(L"material\\EmptyMtrl.mtrl", pNewMtrl);
        pNewMtrl->Save(pNewMtrl->GetKey());
    }

    void MenuUI::AddComponent(COMPONENT_TYPE _type)
    {
        // Outliner 와 Inspector 를 가져온다.
        OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
        InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

        // 선택된 오브젝트를 가져와서 ComponentType 에 맞는 컴포넌트를 생성해서 추가한다.
        GameObject* pSelectedObject = outliner->GetSelectedObject();

        if (nullptr == pSelectedObject)
            return;

        switch (_type)
        {
        case COMPONENT_TYPE::TRANSFORM:
            pSelectedObject->AddComponent(new Transform);
            break;
        case COMPONENT_TYPE::COLLIDER2D:
            pSelectedObject->AddComponent(new Collider2D);
            break;
        case COMPONENT_TYPE::COLLIDER3D:
            //pSelectedObject->AddComponent(new CCollider3D);
            break;
        case COMPONENT_TYPE::ANIMATOR2D:
            pSelectedObject->AddComponent(new Animator2D);
            break;
        case COMPONENT_TYPE::ANIMATOR3D:
            //pSelectedObject->AddComponent(new CAnimator3D);
            break;
        case COMPONENT_TYPE::LIGHT2D:
            pSelectedObject->AddComponent(new Light2D);
            break;
        case COMPONENT_TYPE::LIGHT3D:
            //pSelectedObject->AddComponent(new CLight3D);
            break;
        case COMPONENT_TYPE::CAMERA:
            pSelectedObject->AddComponent(new Camera);
            break;
        case COMPONENT_TYPE::MESHRENDER:
            pSelectedObject->AddComponent(new MeshRender);
            break;
        case COMPONENT_TYPE::PARTICLESYSTEM:
            pSelectedObject->AddComponent(new ParticleSystem);
            break;
        case COMPONENT_TYPE::TILEMAP:
            pSelectedObject->AddComponent(new TileMap);
            break;
        case COMPONENT_TYPE::LANDSCAPE:
            //pSelectedObject->AddComponent(new CLandScape);
            break;
        case COMPONENT_TYPE::DECAL:
            //pSelectedObject->AddComponent(new CDecal);
            break;
        }

        // Inspector 에 새롭게 추가된 컴포넌트를 알리기 위해서 타겟을 다시 알려준다.
        inspector->SetTargetObject(pSelectedObject);

    }

    void MenuUI::AddScript(const wstring& _strScriptName)
    {
        // Outliner 와 Inspector 를 가져온다.
        OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
        InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

        // 선택된 오브젝트를 가져와서 ComponentType 에 맞는 컴포넌트를 생성해서 추가한다.
        GameObject* pSelectedObject = outliner->GetSelectedObject();

        if (nullptr == pSelectedObject)
            return;

        Script* pScript = ScriptMgr::GetScript(_strScriptName);

        pSelectedObject->AddComponent(pScript);

        inspector->SetTargetObject(pSelectedObject);
    }
    void MenuUI::LoadFileList(const wstring& _folder_Path, vector<wstring>& _level_list)
    {
        WIN32_FIND_DATA FindData = {};

        wstring FolderPath = _folder_Path + L"*.*";

        HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

        while (FindNextFile(hFindHandle, &FindData))
        {
            if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!wcscmp(FindData.cFileName, L".."))
                {
                    continue;
                }

                LoadFileList(_folder_Path + FindData.cFileName + L"\\", _level_list);
                continue;
            }

            //wstring strContentPath = PathMgr::GetInst()->GetContentPath();
            //wstring RelativePath = _folder_Path + FindData.cFileName;
            //RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

            _level_list.push_back(FindData.cFileName);
        }
    }
    void MenuUI::LoadLevel(const DWORD_PTR _level_name)
    {
        wstring _level_path = L"Level\\";
        char* _char_level_name = (char*)_level_name;
        wstring _wstr_level_name = wstring(_char_level_name, _char_level_name + strlen(_char_level_name));
        _level_path += _wstr_level_name;

        //LevelSaveLoad::LoadLevel(_level_path);

        Level* _loaded_level = LevelSaveLoad::LoadLevel(_level_path);

        Event evn = {};
        evn.type = EVENT_TYPE::LEVEL_CHANGE;
        evn.w_param = (DWORD_PTR)_loaded_level;

        EventMgr::GetInst()->AddEvent(evn);


        InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
        inspector->ClearTargetObject();
        inspector->ClearTargetResource();
        OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
        outliner->ClearSelectedNodeData();
    }
}