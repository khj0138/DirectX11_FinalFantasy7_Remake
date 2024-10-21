#include "pch.h"
#include "EditorObjMgr.h"

#include "GameObjectEx.h"
#include <Engine\components.h>

#include <Engine\ResourceMgr.h>
#include <Engine\RenderMgr.h>
#include <Engine\TimeMgr.h>
#include <Engine\KeyMgr.h>
#include <Engine\Device.h>

#include <Script\CameraMoveScript.h>

#include <Engine\ConstBuffer.h>

#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>

#include <Engine\ResourceMgr.h>
#include <Engine\CollisionMgr.h>
#include <Engine\PhysXMgr.h>

#include <Script\CloudMoveScript.h>

#include "LevelSaveLoad.h"


#include <Engine/SetColorShader.h>

// ========== Test Include Header =================
#include <Engine/GLTFLoader.h>
#include <Engine/Socket.h>
#include <Engine/ReflectionCapture.h>

#include <Engine/TrailRenderer.h>
#include <Engine/AnimatorController.h>
#include <Engine/RenderMgr.h>

#include <Script/PlayerCameraScript.h>
#include <Script/EffectScript.h>
#include <Script/BackWeaponScript.h>
#include <Script/HundredGunnerScript.h>
#include <Script/HundredGunnerRightArmScript.h>
#include <Script/SaturationFireScript.h>
namespace ff7r
{
	EditorObjMgr::EditorObjMgr()
		: debug_shape{}
	{

	}

	EditorObjMgr::~EditorObjMgr()
	{
		SafeDelVector(editor_obj);
		SafeDelArray(debug_shape);
	}

	void EditorObjMgr::Init()
	{

		//ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl")->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(
		//	L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_01O_T.png"));
		// 디버그 쉐이프 생성
		debug_shape[(UINT)SHAPE_TYPE::RECT] = new GameObjectEx;
		debug_shape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new Transform);
		debug_shape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new MeshRender);
		debug_shape[(UINT)SHAPE_TYPE::RECT]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh_Debug"));
		debug_shape[(UINT)SHAPE_TYPE::RECT]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl"));

		debug_shape[(UINT)SHAPE_TYPE::CIRCLE] = new GameObjectEx;
		debug_shape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new Transform);
		debug_shape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new MeshRender);
		debug_shape[(UINT)SHAPE_TYPE::CIRCLE]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CircleMesh_Debug"));
		debug_shape[(UINT)SHAPE_TYPE::CIRCLE]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl"));

		debug_shape[(UINT)SHAPE_TYPE::CUBE] = new GameObjectEx;
		debug_shape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new Transform);
		debug_shape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new MeshRender);
		debug_shape[(UINT)SHAPE_TYPE::CUBE]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CubeMesh_Debug"));
		debug_shape[(UINT)SHAPE_TYPE::CUBE]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl"));

		debug_shape[(UINT)SHAPE_TYPE::SPHERE] = new GameObjectEx;
		debug_shape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new Transform);
		debug_shape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new MeshRender);
		debug_shape[(UINT)SHAPE_TYPE::SPHERE]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"DebugSphereMesh"));
		debug_shape[(UINT)SHAPE_TYPE::SPHERE]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl"));

		debug_shape[(UINT)SHAPE_TYPE::CAPSULE] = new GameObjectEx;
		debug_shape[(UINT)SHAPE_TYPE::CAPSULE]->AddComponent(new Transform);
		debug_shape[(UINT)SHAPE_TYPE::CAPSULE]->AddComponent(new MeshRender);
		debug_shape[(UINT)SHAPE_TYPE::CAPSULE]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CapsuleMesh"));
		debug_shape[(UINT)SHAPE_TYPE::CAPSULE]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl"));

		// EditorObject 생성
		GameObjectEx* pEditorCamObj = new GameObjectEx;
		pEditorCamObj->AddComponent(new Transform);
		pEditorCamObj->AddComponent(new Camera);
		pEditorCamObj->AddComponent(new CameraMoveScript);

		pEditorCamObj->GetCamera()->SetFar(1000.f);
		pEditorCamObj->GetCamera()->SetLayerMaskAll(true);
		pEditorCamObj->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pEditorCamObj->GetCamera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pEditorCamObj->GetCamera()->SetLayerMask(31, false);
		pEditorCamObj->GetCamera()->SetPostProcess(Camera::CameraPostProcess::BLOOM, true);
		pEditorCamObj->GetCamera()->SetPostProcess(Camera::CameraPostProcess::TONEMAPPING, true);
		pEditorCamObj->GetCamera()->SetPostProcess(Camera::CameraPostProcess::FXAA, true);
		pEditorCamObj->SetDestroy(false);
		editor_obj.push_back(pEditorCamObj);
		RenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->GetCamera());
	}



	void EditorObjMgr::Progress()
	{
		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::_1))
		{
			RenderMgr::GetInst()->RegisterEditorCamera(editor_obj[0]->GetCamera());
		}



		// 전역 상수 데이터 바인딩
		//g_global_data.main_camera_pos = RenderMgr::GetInst()->GetCam(1)->GetTransform()->GetRelativePos();
		//ConstBuffer* pGlobalBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
		//pGlobalBuffer->SetData(&g_global_data, sizeof(GlobalConst));
		//pGlobalBuffer->UpdateData();
		////pGlobalBuffer->UpdateData_CS();
		RenderMgr::GetInst()->GetEditorCam()->RegisterMatrix();

		// DebugShape 정보 가져오기
		vector<DebugShapeInfo>& vecInfo = RenderMgr::GetInst()->GetDebugShapeInfo();
		debug_shape_info.insert(debug_shape_info.end(), vecInfo.begin(), vecInfo.end());
		vecInfo.clear();


		Tick();

		Render();
	}

	void EditorObjMgr::Tick()
	{

		for (size_t i = 0; i < editor_obj.size(); ++i)
		{
			editor_obj[i]->Tick();
		}

		for (size_t i = 0; i < editor_obj.size(); ++i)
		{
			editor_obj[i]->FinalTick();
		}
	}

	void EditorObjMgr::Render()
	{
		for (size_t i = 0; i < editor_obj.size(); ++i)
		{
			editor_obj[i]->Render();
		}


		return;
		// DebugShape Render
		GameObjectEx* pShapeObj = nullptr;

		vector<DebugShapeInfo>::iterator iter = debug_shape_info.begin();
		for (; iter != debug_shape_info.end();)
		{
			switch (iter->shape)
			{
			case SHAPE_TYPE::RECT:
				pShapeObj = debug_shape[(UINT)SHAPE_TYPE::RECT];
				break;
			case SHAPE_TYPE::CIRCLE:
				pShapeObj = debug_shape[(UINT)SHAPE_TYPE::CIRCLE];
				break;
			case SHAPE_TYPE::CUBE:
				pShapeObj = debug_shape[(UINT)SHAPE_TYPE::CUBE];
				break;
			case SHAPE_TYPE::SPHERE:
				pShapeObj = debug_shape[(UINT)SHAPE_TYPE::SPHERE];
				break;
			case SHAPE_TYPE::CAPSULE:
				pShapeObj = debug_shape[(UINT)SHAPE_TYPE::CAPSULE];
				break;
			}

			if (iter->mat_world != XMMatrixIdentity())
			{
				pShapeObj->GetTransform()->SetWorldMat(iter->mat_world);
			}
			else
			{
				pShapeObj->GetTransform()->SetRelativePos(iter->world_pos);
				pShapeObj->GetTransform()->SetRelativeScale(iter->world_scale);
				pShapeObj->GetTransform()->SetRelativeRot(iter->world_rotation);
				pShapeObj->FinalTick();
			}

			pShapeObj->GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, &iter->color);

			if (iter->depth_test)
				pShapeObj->GetMeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
			else
				pShapeObj->GetMeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);


			//CONTEXT->VSSetShader(nullptr, nullptr, 0);
			//CONTEXT->HSSetShader(nullptr, nullptr, 0);	
			//CONTEXT->DSSetShader(nullptr, nullptr, 0);
			//CONTEXT->GSSetShader(nullptr, nullptr, 0);
			//CONTEXT->PSSetShader(nullptr, nullptr, 0);

			//for (UINT i = 0; i < TEX_END; ++i)
			//{

			//		Texture::Clear(i);
			//}
			pShapeObj->Render();

			iter->cur_time += DT;
			if (iter->max_time < iter->cur_time)
			{
				iter = debug_shape_info.erase(iter);
			} 
			else
			{
				++iter;
			}
		}

	}
}