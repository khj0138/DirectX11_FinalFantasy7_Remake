#include "pch.h"
#include "Camera.h"

#include "Device.h"
#include "RenderMgr.h"
#include "Transform.h"
#include "Light3D.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Material.h"
#include "GraphicsShader.h"

#include "RenderMgr.h"
#include "MRT.h"

#include "ResourceMgr.h"
#include "Mesh.h"
#include "Material.h"
#include "InstancingBuffer.h"
#include "BlurShader.h"
#include "Animator3D.h"

namespace ff7r
{
	Camera::Camera()
		: Component(COMPONENT_TYPE::CAMERA)
		, frustum(this)
		, aspect_ratio(1.f)
		, scale(1.f)
		, __far(1000.f)
		, fov(XM_PI / 3.f)
		, ortho_width(0.f)
		, ortho_height(0.f)
		, proj_type(PROJ_TYPE::ORTHOGRAPHIC)
		, layer_mask(0)
		, camera_idx(-1)
		, __near(0.1f)
		, intensity(0.f)
		, post_process_option{ false, false, false, false }
	{
		SetName(L"Camera");

		vec2 vRenderResol = Device::GetInst()->GetRenderResolution();
		aspect_ratio = vRenderResol.x / vRenderResol.y;

		ortho_width = vRenderResol.x;
		ortho_height = vRenderResol.y;
	}

	Camera::Camera(const Camera& _Other)
		: Component(_Other)
		, frustum(this)
		, aspect_ratio(_Other.aspect_ratio)
		, scale(_Other.scale)
		, fov(_Other.fov)
		, ortho_width(_Other.ortho_width)
		, ortho_height(_Other.ortho_height)
		, proj_type(_Other.proj_type)
		, layer_mask(_Other.layer_mask)
		, camera_idx(-1)
		//, post_process_option{ _Other.post_process_option }
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Awake()
	{
		RenderMgr::GetInst()->RegisterCamera(this, camera_idx);
	}

	void Camera::Begin()
	{
		if (-1 != camera_idx)
		{
			//RenderMgr::GetInst()->RegisterCamera(this, camera_idx);
		}
	}

	void Camera::FinalTick()
	{
		CalcViewMat();

		CalcProjMat();

		frustum.FinalTick();
	}

	void Camera::CalcViewMat()
	{
		// ==============
		// View 행렬 계산
		// ==============
		mat_view = XMMatrixIdentity();

		// 카메라 좌표를 원점으로 이동
		vec3 vCamPos = GetTransform()->GetWorldPos();
		Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

		// 카메라가 바라보는 방향을 Z 축과 평행하게 만드는 회전 행렬을 적용
		Matrix matViewRot = XMMatrixIdentity();

		vec3 vR = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 vU = GetTransform()->GetWorldDir(DIR_TYPE::UP);
		vec3 vF = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);

		matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
		matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
		matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

		mat_view = matViewTrans * matViewRot;


		// View 역행렬 구하기
		mat_view_inv = XMMatrixInverse(nullptr, mat_view);
	}

	void Camera::CalcProjMat()
	{
		// =============
		// 투영 행렬 계산
		// =============
		mat_proj = XMMatrixIdentity();

		if (PROJ_TYPE::ORTHOGRAPHIC == proj_type)
		{
			// 직교 투영
			vec2 vResolution = Device::GetInst()->GetRenderResolution();
			mat_proj = XMMatrixOrthographicLH(ortho_width * (1.f / scale), ortho_height * (1.f / scale), __near, 10000.f);
		}
		else
		{
			// 원근 투영
			mat_proj = XMMatrixPerspectiveFovLH(fov, aspect_ratio, __near, __far);
		}

		// 투영행렬 역행렬 구하기
		mat_proj_inv = XMMatrixInverse(nullptr, mat_proj);
	}

	void Camera::SetLayerMask(int _iLayer, bool _Visible)
	{
		if (_Visible)
		{
			layer_mask |= 1 << _iLayer;
		}
		else
		{
			layer_mask &= ~(1 << _iLayer);
		}
	}

	void Camera::SetLayerMaskAll(bool _Visible)
	{
		if (_Visible)
			layer_mask = 0xffffffff;
		else
			layer_mask = 0;
	}

	void Camera::SetCameraIndex(int _idx)
	{
		camera_idx = _idx;
		RenderMgr::GetInst()->RegisterCamera(this, camera_idx);
	}

	void Camera::RegisterMatrix()
	{
		g_transform_data.mat_view = mat_view;
		g_transform_data.mat_view_inv = mat_view_inv;

		g_transform_data.mat_proj = mat_proj;
		g_transform_data.mat_proj_inv = mat_proj_inv;
	}

	void Camera::SortObjects()
	{
		//// 이전 프레임 분류정보 제거
		Clear();

		Level* pCurLevel = LevelMgr::GetInst()->GetCurLevel();

		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (layer_mask & (1 << i))
			{
				Layer* _layer = pCurLevel->GetLayer(i);
				const vector<GameObject*>& objects = _layer->GetParentObject();

				for (size_t j = 0; j < objects.size(); ++j)
				{
					SortObject(objects[j]);
				}
			}
		}
		
	}

	void Camera::SortObject(GameObject* _object)
	{
		if (!_object->GetActive())
			return;

		RenderComponent* _render_comp = _object->GetRenderComponent();
		if (_render_comp && _render_comp->GetMaterial() != nullptr 
			&& _render_comp->GetMaterial()->GetShader() != nullptr && _render_comp->IsRender())
		{
			UINT _mtrl_cnt = _render_comp->GetMtrlCount();

			for (UINT i = 0; i < _mtrl_cnt; ++i)
			{
				if (nullptr == _render_comp->GetMaterial(i)
					|| nullptr == _render_comp->GetMaterial(i)->GetShader())
				{
					continue;
				}

				Ptr<Mesh> _mesh = _render_comp->GetMesh();
				//if (_mesh != nullptr)
				//{
				//	vec3 pos = _mesh->GetBoundingOffset();
				//	_object->GetTransform()->CalcWorldDir();
				//
				//	vec3 new_pos;
				//	new_pos += pos.z * _object->GetTransform()->GetWorldDir(DIR_TYPE::FRONT);
				//	new_pos += pos.y * _object->GetTransform()->GetWorldDir(DIR_TYPE::UP);
				//	new_pos += pos.x * _object->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
				//	DrawDebugSphere(_object->GetTransform()->GetWorldPos() + new_pos, _mesh->GetBoundingRadius(), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f));
				//}

				if (_render_comp->IsFrustumCheck())
				{
					vec3 pos = _mesh->GetBoundingOffset();

					vec3 new_pos;
					new_pos += pos.z * _object->GetTransform()->GetWorldDir(DIR_TYPE::FRONT);
					new_pos += pos.y * _object->GetTransform()->GetWorldDir(DIR_TYPE::UP);
					new_pos += pos.x * _object->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

					vec3 vWorldPos = _object->GetTransform()->GetWorldPos();
					if (false == frustum.FrustumCheckBound(vWorldPos + new_pos, _render_comp->GetMesh()->GetBoundingRadius()))
					{
						continue;
					}
				}
				SHADER_DOMAIN eDomain = _render_comp->GetMaterial(i)->GetShader()->GetDomain();
				Ptr<GraphicsShader> pShader = _render_comp->GetMaterial(i)->GetShader();

				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
				case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
				case SHADER_DOMAIN::DOMAIN_MASK:
				{
					// Shader 의 POV 에 따라서 인스턴싱 그룹을 분류한다.
					map<ULONG64, vector<InstObj>>* pMap = NULL;
					Ptr<Material> pMtrl = _render_comp->GetMaterial(i);

					if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
					{
						pMap = &inst_deferred;
					}
					else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE || pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASK)
					{
						pMap = &inst_forward;
					}
					else
					{
						assert(nullptr);
						continue;
					}

					InstID uID = {};
					uID.llID = _render_comp->GetInstID(i);

					// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
					if (0 == uID.llID)
						continue;

					map<ULONG64, vector<InstObj>>::iterator iter = pMap->find(uID.llID);
					if (iter == pMap->end())
					{
						pMap->insert(make_pair(uID.llID, vector<InstObj>{InstObj{ _object, i }}));
					}
					else
					{
						iter->second.push_back(InstObj{ _object, i });
					}
				}
				break;
				case SHADER_DOMAIN::DOMAIN_DECAL: decal_obj.push_back(_object); break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT: transparent_obj.push_back(_object); break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS: post_obj.push_back(_object); break;
				case SHADER_DOMAIN::DOMAIN_UI: ui_obj.push_back(_object); break;
				}
			}
		}
		if (_render_comp && !_render_comp->IsRender())
		{
			return;
		}
		vector<GameObject*> _childs = _object->GetChild();
		for (size_t i = 0; i < _childs.size(); i++)
		{
			SortObject(_childs[i]);
		}
	}

	void Camera::SortShadowObject()
	{
		// 이전 프레임 분류정보 제거
		ClearShadow();

		// 현재 레벨 가져와서 분류
		Level* pCurLevel = LevelMgr::GetInst()->GetCurLevel();

		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			// 레이어 마스크 확인
			if (layer_mask & (1 << i))
			{
				Layer* pLayer = pCurLevel->GetLayer(i);
				const vector<GameObject*>& vecObject = pLayer->GetObjects();

				for (size_t j = 0; j < vecObject.size(); ++j)
				{
					RenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

					// 렌더링 기능이 없는 오브젝트는 제외
					if (nullptr == pRenderCom
						|| nullptr == pRenderCom->GetMaterial()
						|| nullptr == pRenderCom->GetMaterial()->GetShader())
					{
						continue;
					}

					shadow_pbj.push_back(vecObject[j]);
				}
			}
		}
	}

	void Camera::Render()
	{
		RenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

		RegisterMatrix();
		//cameras[1]->RegisterMatrix();
		UINT _layer_mask = GetLayerMask();

		RenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSet(true);
		RenderDeferred();

		static Ptr<Mesh> _mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh");

		RenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet(true);
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"DefaultLightMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"ColorMetalicTarget"));
		_mtrl->SetTexParam(TEX_1, ResourceMgr::GetInst()->FindRes<Texture>(L"NormalRoughnessTarget"));
		_mtrl->SetTexParam(TEX_2, ResourceMgr::GetInst()->FindRes<Texture>(L"PositionTarget"));
		_mtrl->SetTexParam(TEX_3, ResourceMgr::GetInst()->FindRes<Texture>(L"EmissiveOcclusionTarget"));

		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);


		RenderMgr::GetInst()->GetMRT(MRT_TYPE::AMBIENT_LIGHT)->OMSet(true);

		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"AmbientIBLMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"ColorMetalicTarget"));
		_mtrl->SetTexParam(TEX_1, ResourceMgr::GetInst()->FindRes<Texture>(L"NormalRoughnessTarget"));
		_mtrl->SetTexParam(TEX_2, ResourceMgr::GetInst()->FindRes<Texture>(L"PositionTarget"));
		_mtrl->SetTexParam(TEX_3, ResourceMgr::GetInst()->FindRes<Texture>(L"EmissiveOcclusionTarget"));
		_mtrl->SetTexParam(TEX_5, ResourceMgr::GetInst()->FindRes<Texture>(L"BRDFLookUp"));
		_mtrl->SetTexParam(TEX_6, ResourceMgr::GetInst()->FindRes<Texture>(L"DefaultIrradianceMap"));
		_mtrl->SetTexParam(TEX_CUBE_0, ResourceMgr::GetInst()->FindRes<Texture>(L"DefaultPrefilterMap"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		RenderMgr::GetInst()->GetMRT(MRT_TYPE::HDR_TARGET)->OMSet(true);
		static Ptr<Material> pMtrl = ResourceMgr::GetInst()->FindRes<Material>(L"MergeLightMtrl");

		static bool bSet = false;
		if (!bSet)
		{
			bSet = true;
			pMtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"AmbientLightTarget"));
			pMtrl->SetTexParam(TEX_1, ResourceMgr::GetInst()->FindRes<Texture>(L"LightTarget"));
		}

		pMtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		RenderForward();
		RenderDecal();
		//RenderOpaque();
		RenderTransparent();
		// RenderPostprocess();

		UINT stride = 0;
		UINT offset = 0;

		ID3D11Buffer* nullBuffer = nullptr;
		CONTEXT->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);

		if (post_process_option[(UINT)CameraPostProcess::BLOOM])
		{
			Bloom();
		}
		if (post_process_option[(UINT)CameraPostProcess::TONEMAPPING])
		{
			ToneMapping();

		}
		RenderPostprocess();
		if (post_process_option[(UINT)CameraPostProcess::FXAA])
		{
			FXAA();
		}
		if (post_process_option[(UINT)CameraPostProcess::GRAYSCALE])
		{
			GrayScale();
		}
		if (post_process_option[(UINT)CameraPostProcess::FLASH])
		{
			Flash();
		}
		// g_global_data.main_camera_pos = cameras[0]->GetTransform()->GetRelativePos();
		//GetCam(1)->RegisterMatrix();
		//_layer_mask = GetCam(1)->GetLayerMask();

		//for (GameObject* object : sort_objects[(UINT)SHADER_DOMAIN::DOMAIN_UI])
		//{
		//	if (_layer_mask & (1 << object->GetLayerIndex()))
		//	{
		//		object->Render();
		//	}
		//}
		RenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
		RenderUI();

		// if (post_process_option[(UINT)CameraPostProcess::FXAA])
		// {
		// 	FXAA();
		// }
	}

	void Camera::RenderShadowMap()
	{
		g_transform_data.mat_view = mat_view;
		g_transform_data.mat_proj = mat_proj;

		for (size_t i = 0; i < shadow_pbj.size(); ++i)
		{
			//shadow_pbj[i]->RenderShadowMap();
		}
	}

	void Camera::Clear() 
	{
		deferred_obj.clear();
		deferred_dacal_obj.clear();

		for (auto& pair : inst_deferred)
			pair.second.clear();
		for (auto& pair : inst_forward)
			pair.second.clear();

		opaque_obj.clear();
		mask_obj.clear();
		decal_obj.clear();
		transparent_obj.clear();
		post_obj.clear();
		ui_obj.clear();
	}

	void Camera::ClearShadow()
	{
		shadow_pbj.clear();
	}

	void Camera::RenderDeferred()
	{
		RegisterMatrix();

		for (auto& pair : inst_single)
		{
			pair.second.clear();
		}

		// Deferred object render
		InstancingData tInstData = {};

		for (auto& pair : inst_deferred)
		{
			// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
			if (pair.second.empty())
				continue;

			// instancing 개수 조건 미만이거나
			// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
			// Shader 가 Instancing 을 지원하지 않는경우
			if (pair.second.size() <= 1
				|| pair.second[0].object->GetAnimator2D()
				|| pair.second[0].object->GetRenderComponent()->GetMaterial(pair.second[0].mtrl_idx)->GetShader()->GetVSInst() == nullptr)
			{
				// 해당 물체들은 단일 랜더링으로 전환
				for (UINT i = 0; i < pair.second.size(); ++i)
				{
					map<INT_PTR, vector<InstObj>>::iterator iter
						= inst_single.find((INT_PTR)pair.second[i].object);

					if (iter != inst_single.end())
						iter->second.push_back(pair.second[i]);
					else
					{
						inst_single.insert(make_pair((INT_PTR)pair.second[i].object, vector<InstObj>{pair.second[i]}));
					}
				}
				continue;
			}

			GameObject* _object = pair.second[0].object;
			Ptr<Mesh> pMesh = _object->GetRenderComponent()->GetMesh();
			Ptr<Material> pMtrl = _object->GetRenderComponent()->GetMaterial(pair.second[0].mtrl_idx);

			// Instancing 버퍼 클리어
			InstancingBuffer::GetInst()->Clear();

			int iRowIdx = 0;
			bool bHasAnim3D = false;
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				tInstData.mat_world = pair.second[i].object->GetTransform()->GetWorldMat();
				tInstData.mat_wv = tInstData.mat_world * mat_view;
				tInstData.mat_wvp = tInstData.mat_wv * mat_proj;

				if (pair.second[i].object->GetAnimator3D())
				{
					pair.second[i].object->GetAnimator3D()->UpdateData();
					tInstData.row_idx = iRowIdx++;
					InstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].object->GetAnimator3D()->GetFinalJointMat());
					bHasAnim3D = true;
				}
				else
					tInstData.row_idx = -1;

				InstancingBuffer::GetInst()->AddInstancingData(tInstData);
			}

			// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
			InstancingBuffer::GetInst()->SetData();

			if (bHasAnim3D)
			{
				pMtrl->SetAnimation3D(true); // Animation Mesh 알리기
				pMtrl->SetJointCount(pMesh->GetJointCount());
			}

			pMtrl->UpdateDataInst();
			pMesh->RenderInstancing(pair.second[0].mtrl_idx);

			// 정리
			if (bHasAnim3D)
			{
				pMtrl->SetAnimation3D(false); // Animation Mesh 알리기
				pMtrl->SetJointCount(0);
			}
		}

		// 개별 랜더링
		for (auto& pair : inst_single)
		{
			if (pair.second.empty())
				continue;

			pair.second[0].object->GetTransform()->UpdateData();

			for (auto& instObj : pair.second)
			{
				instObj.object->GetRenderComponent()->Render(instObj.mtrl_idx);
			}

			if (pair.second[0].object->GetAnimator3D())
			{
				pair.second[0].object->GetAnimator3D()->ClearData();
			}
		}
	}

	void Camera::RenderForward()
	{
		RegisterMatrix();

		for (auto& pair : inst_single)
		{
			pair.second.clear();
		}

		// Deferred object render
		InstancingData tInstData = {};

		for (auto& pair : inst_forward)
		{
			// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
			if (pair.second.empty())
				continue;

			// instancing 개수 조건 미만이거나
			// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
			// Shader 가 Instancing 을 지원하지 않는경우
			if (pair.second.size() <= 1
				|| pair.second[0].object->GetAnimator2D()
				|| pair.second[0].object->GetRenderComponent()->GetMaterial(pair.second[0].mtrl_idx)->GetShader()->GetVSInst() == nullptr)
			{
				// 해당 물체들은 단일 랜더링으로 전환
				for (UINT i = 0; i < pair.second.size(); ++i)
				{
					map<INT_PTR, vector<InstObj>>::iterator iter
						= inst_single.find((INT_PTR)pair.second[i].object);

					if (iter != inst_single.end())
						iter->second.push_back(pair.second[i]);
					else
					{
						inst_single.insert(make_pair((INT_PTR)pair.second[i].object, vector<InstObj>{pair.second[i]}));
					}
				}
				continue;
			}

			GameObject* _object = pair.second[0].object;
			Ptr<Mesh> pMesh = _object->GetRenderComponent()->GetMesh();
			Ptr<Material> pMtrl = _object->GetRenderComponent()->GetMaterial(pair.second[0].mtrl_idx);

			// Instancing 버퍼 클리어
			InstancingBuffer::GetInst()->Clear();

			int iRowIdx = 0;
			bool bHasAnim3D = false;
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				tInstData.mat_world = pair.second[i].object->GetTransform()->GetWorldMat();
				tInstData.mat_wv = tInstData.mat_world * mat_view;
				tInstData.mat_wvp = tInstData.mat_wv * mat_proj;

				if (pair.second[i].object->GetAnimator3D())
				{
					pair.second[i].object->GetAnimator3D()->UpdateData();
					tInstData.row_idx = iRowIdx++;
					InstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].object->GetAnimator3D()->GetFinalJointMat());
					bHasAnim3D = true;
				}
				else
					tInstData.row_idx = -1;

				InstancingBuffer::GetInst()->AddInstancingData(tInstData);
			}

			// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
			InstancingBuffer::GetInst()->SetData();

			if (bHasAnim3D)
			{
				pMtrl->SetAnimation3D(true); // Animation Mesh 알리기
				pMtrl->SetJointCount(pMesh->GetJointCount());
			}

			pMtrl->UpdateDataInst();
			pMesh->RenderInstancing(pair.second[0].mtrl_idx);

			// 정리
			if (bHasAnim3D)
			{
				pMtrl->SetAnimation3D(false); // Animation Mesh 알리기
				pMtrl->SetJointCount(0);
			}
		}

		// 개별 랜더링
		for (auto& pair : inst_single)
		{
			if (pair.second.empty())
				continue;

			pair.second[0].object->GetTransform()->UpdateData();

			for (auto& instObj : pair.second)
			{
				instObj.object->GetRenderComponent()->Render(instObj.mtrl_idx);
			}

			if (pair.second[0].object->GetAnimator3D())
			{
				pair.second[0].object->GetAnimator3D()->ClearData();
			}
		}
	}

	void Camera::RenderOpaque()
	{
		RegisterMatrix();
		for (size_t i = 0; i < opaque_obj.size(); ++i)
		{
			opaque_obj[i]->Render();
		}
	}

	void Camera::RenderMask()
	{
		RegisterMatrix();
		for (size_t i = 0; i < mask_obj.size(); ++i)
		{
			mask_obj[i]->Render();
		}
	}

	void Camera::RenderDecal()
	{
		RegisterMatrix();
		for (size_t i = 0; i < decal_obj.size(); ++i)
		{
			decal_obj[i]->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		RegisterMatrix();
		for (size_t i = 0; i < transparent_obj.size(); ++i)
		{
			transparent_obj[i]->Render();
		}
	}

	void Camera::RenderPostprocess()
	{
		RegisterMatrix();
		RenderMgr::GetInst()->CopyRenderTarget();
		if (post_obj.size() > 0)
		{
			RenderMgr::GetInst()->BlurRenderTarget();
		}

		for (size_t i = 0; i < post_obj.size(); ++i)
		{
			post_obj[i]->Render();
		}
	}

	void Camera::RenderUI()
	{
		RegisterMatrix();
		for (size_t i = 0; i < ui_obj.size(); ++i)
		{
			ui_obj[i]->Render();
		}
	}

	void Camera::Bloom()
	{
		Ptr<Mesh> _mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh");

		Ptr<Texture> _bloom_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomExtract");
		CONTEXT->OMSetRenderTargets(1, _bloom_tex->GetRTV().GetAddressOf(), nullptr);
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"BloomExtractMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"HDR_Target"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f / 2.f;
		viewport.Height = 900.f / 2.f;

		CONTEXT->RSSetViewports(1, &viewport);

		_bloom_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided2");
		CONTEXT->OMSetRenderTargets(1, _bloom_tex->GetRTV().GetAddressOf(), nullptr);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomExtract"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		viewport;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f / 4.f;
		viewport.Height = 900.f / 4.f;
		CONTEXT->RSSetViewports(1, &viewport);

		_bloom_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4");
		CONTEXT->OMSetRenderTargets(1, _bloom_tex->GetRTV().GetAddressOf(), nullptr);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided2"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		viewport;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f / 8.f;
		viewport.Height = 900.f / 8.f;

		CONTEXT->RSSetViewports(1, &viewport);

		//_bloom_tex->Clear();
		_bloom_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided8");
		CONTEXT->OMSetRenderTargets(1, _bloom_tex->GetRTV().GetAddressOf(), nullptr);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
		Ptr<BlurShader> blur = (BlurShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"BlurCS").Get();


		blur->SetTexSize(vec2(1600.f / 8.f, 900.f / 8.f));
		blur->SetTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided8"), ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided8_2"));
		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f / 4.f;
		viewport.Height = 900.f / 4.f;

		CONTEXT->RSSetViewports(1, &viewport);

		Texture::Clear(0);
		_bloom_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4");
		CONTEXT->OMSetRenderTargets(1, _bloom_tex->GetRTV().GetAddressOf(), nullptr);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided8"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);

		blur->SetTexSize(vec2(1600.f / 4.f, 900.f / 4.f));
		blur->SetTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4"), ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4_2"));
		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f / 2.f;
		viewport.Height = 900.f / 2.f;

		CONTEXT->RSSetViewports(1, &viewport);

		_bloom_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided2");
		CONTEXT->OMSetRenderTargets(1, _bloom_tex->GetRTV().GetAddressOf(), nullptr);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FullScreenRenderMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided4"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);

		CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
		blur->SetTexSize(vec2(1600.f / 2.f, 900.f / 2.f));
		blur->SetTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided2"), ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided2_2"));
		blur->SetCount(0);
		blur->Execute();
		blur->SetCount(1);
		blur->Execute();

		blur->SetCount(0);
		blur->Execute();

		blur->SetCount(1);
		blur->Execute();

		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		viewport.Width = 1600.f;
		viewport.Height = 900.f;

		CONTEXT->RSSetViewports(1, &viewport);
	}

	void Camera::ToneMapping()
	{
		RenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet(true);
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"ToneMappingMtrl");
		_mtrl->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"HDR_Target"));
		_mtrl->SetTexParam(TEX_1, ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided2"));
		_mtrl->UpdateData();
		CONTEXT->Draw(4, 0);
	}

	void Camera::FXAA()
	{
		RenderMgr::GetInst()->CopyRenderTarget();
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FXAAMtrl");
		_mtrl->UpdateData();
		_mtrl->SetTexParam(TEX_0, RenderMgr::GetInst()->GetCopyRenderTarget());
		CONTEXT->Draw(4, 0);
	}

	void Camera::GrayScale()
	{
		RenderMgr::GetInst()->CopyRenderTarget();
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"GrayScaleMtrl");
		_mtrl->UpdateData();
		_mtrl->SetTexParam(TEX_0, RenderMgr::GetInst()->GetCopyRenderTarget());
		CONTEXT->Draw(4, 0);
	}

	void Camera::Flash()
	{
		RenderMgr::GetInst()->CopyRenderTarget();
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"FlashMtrl");
		_mtrl->SetScalarParam(FLOAT_0, &intensity);
		_mtrl->UpdateData();
		_mtrl->SetTexParam(TEX_0, RenderMgr::GetInst()->GetCopyRenderTarget());
		CONTEXT->Draw(4, 0);
	}

	void Camera::SaveToLevelFile(FILE* _File)
	{
		fwrite(&aspect_ratio, sizeof(float), 1, _File);
		fwrite(&scale, sizeof(float), 1, _File);
		fwrite(&proj_type, sizeof(UINT), 1, _File);
		fwrite(&layer_mask, sizeof(UINT), 1, _File);
		fwrite(&camera_idx, sizeof(int), 1, _File);
	}

	void Camera::LoadFromLevelFile(FILE* _File)
	{
		fread(&aspect_ratio, sizeof(float), 1, _File);
		fread(&scale, sizeof(float), 1, _File);
		fread(&proj_type, sizeof(UINT), 1, _File);
		fread(&layer_mask, sizeof(UINT), 1, _File);
		fread(&camera_idx, sizeof(int), 1, _File);
	}
}