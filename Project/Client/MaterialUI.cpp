#include "pch.h"
#include "MaterialUI.h"

#include "ParamUI.h"
#include "TreeUI.h"
#include "ListUI.h"

#include <Engine\ResourceMgr.h>
#include <Engine\Material.h>

namespace ff7r
{
    MaterialUI::MaterialUI()
        : ResourceUI(RES_TYPE::MATERIAL)
    {
        SetName("Material");
    }

    MaterialUI::~MaterialUI()
    {
    }

    int MaterialUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        // Material �̸�
        ImGui::Text("Material");
        ImGui::SameLine();

        Ptr<Material> pMtrl = (Material*)GetTargetRes().Get();
        string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
        ImGui::InputText("##MtrlUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

        // GraphicsShader �̸�
        ImGui::Text("Shader  ");
        ImGui::SameLine();

        Ptr<GraphicsShader> pShader = pMtrl->GetShader();
        if (nullptr != pShader)
        {
            string strKey = string(pShader->GetKey().begin(), pShader->GetKey().end());
            ImGui::InputText("##ShaderUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
        }
        else
        {
            char szEmtpy[10] = {};
            ImGui::InputText("##ShaderUIName", szEmtpy, 10, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
        }

        // GraphicsShader ��� üũ    
        if (ImGui::BeginDragDropTarget())
        {
            // �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
            if (pPayLoad)
            {
                TreeNode* pNode = (TreeNode*)pPayLoad->Data;
                Resource* pRes = (Resource*)pNode->GetData();
                if (RES_TYPE::GRAPHICS_SHADER == pRes->GetType())
                {
                    ((Material*)GetTargetRes().Get())->SetShader((GraphicsShader*)pRes);
                }
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();

        if (ImGui::Button("##ShaderSelectBtn", ImVec2(18, 18)))
        {
            const map<wstring, Ptr<Resource>>& mapShader = ResourceMgr::GetInst()->GetResources(RES_TYPE::GRAPHICS_SHADER);

            ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
            pListUI->Reset("GraphicsShader List", ImVec2(300.f, 500.f));
            for (const auto& pair : mapShader)
            {
                pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
            }

            // �׸� ���ý� ȣ����� ��������Ʈ ���
            pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MaterialUI::SelectShader);
        }


        ImGui::NewLine();
        ImGui::Text("Parameter");


        // Shader ���� �䱸�ϴ� ScalarParameter �� UI �� ����
        if (nullptr == pShader)
            return 0;

        const vector<ScalarParam>& vecScalarParam = pShader->GetScalarParam();

        for (size_t i = 0; i < vecScalarParam.size(); ++i)
        {
            switch (vecScalarParam[i].param)
            {
                // Shader Parameter �� Int Ÿ���� ���
            case INT_0:
            case INT_1:
            case INT_2:
            case INT_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                int data = 0;
                pMtrl->GetScalarParam(vecScalarParam[i].param, &data);
                if (ParamUI::Param_Int(vecScalarParam[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    pMtrl->SetScalarParam(vecScalarParam[i].param, &data);
                }
            }
            break;
            case FLOAT_0:
            case FLOAT_1:
            case FLOAT_2:
            case FLOAT_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                float data = 0;
                pMtrl->GetScalarParam(vecScalarParam[i].param, &data);
                if (ParamUI::Param_Float(vecScalarParam[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    pMtrl->SetScalarParam(vecScalarParam[i].param, &data);
                }
            }
            break;
            case VEC2_0:
            case VEC2_1:
            case VEC2_2:
            case VEC2_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                vec2 data;
                pMtrl->GetScalarParam(vecScalarParam[i].param, &data);
                if (ParamUI::Param_Vec2(vecScalarParam[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    pMtrl->SetScalarParam(vecScalarParam[i].param, &data);
                }
            }
            break;
            case VEC4_0:
            case VEC4_1:
            case VEC4_2:
            case VEC4_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                vec4 data;
                pMtrl->GetScalarParam(vecScalarParam[i].param, &data);
                if (ParamUI::Param_Vec4(vecScalarParam[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    pMtrl->SetScalarParam(vecScalarParam[i].param, &data);
                }
            }
            break;
            case MAT_0:
            case MAT_1:
            case MAT_2:
            case MAT_3:
                break;
            }
        }

        // Shader ���� �䱸�ϴ� Texture Parameter �� UI �� ����
        const vector<TexParam>& vecTexParam = pShader->GetTexParam();
        for (size_t i = 0; i < vecTexParam.size(); ++i)
        {
            ImGui::NewLine();
            Ptr<Texture> pCurTex = pMtrl->GetTexParam(vecTexParam[i].param);
            if (ParamUI::Param_Tex(vecTexParam[i].desc, pCurTex, this, (UI_DELEGATE_1)&MaterialUI::SelectTexture))
            {
                m_eSelected = vecTexParam[i].param;
            }
            pMtrl->SetTexParam(vecTexParam[i].param, pCurTex);
        }

        if (ImGui::Button("Save Material##MaterialSaveBtn", ImVec2(200, 18)))
        {
            pMtrl->Save(pMtrl->GetRelativePath());
        }

        return 0;
    }


    void MaterialUI::SelectTexture(DWORD_PTR _Key)
    {
        string strKey = (char*)_Key;
        Ptr<Texture> pTex = ResourceMgr::GetInst()->FindRes<Texture>(wstring(strKey.begin(), strKey.end()));

        Ptr<Material> pMtrl = (Material*)GetTargetRes().Get();
        pMtrl->SetTexParam(m_eSelected, pTex);
    }

    void MaterialUI::SelectShader(DWORD_PTR _data)
    {
        string strKey = (char*)_data;
        Ptr<GraphicsShader> pShader = ResourceMgr::GetInst()->FindRes<GraphicsShader>(wstring(strKey.begin(), strKey.end()));
        ((Material*)GetTargetRes().Get())->SetShader(pShader);
    }
}