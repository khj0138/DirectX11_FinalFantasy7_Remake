#include "pch.h"
#include "SkyBox.h"

#include "ResourceMgr.h"
#include "Mesh.h"
#include "Material.h"

#include "Transform.h"


namespace ff7r
{
	SkyBox::SkyBox()
		: RenderComponent(COMPONENT_TYPE::SKYBOX)
		, type(SKYBOX_TYPE::SPHERE)
	{
		SetSkyBoxType(type);
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"SkyBoxMtrl"));
		SetFrustumCheck(false);
	}

	SkyBox::~SkyBox()
	{
	}

	void SkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
	{
		type = _Type;

		if (type == SKYBOX_TYPE::SPHERE)
		{
			SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"SphereMesh"));
		}
		else
		{
			SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CubeMesh"));
		}

		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"SkyBoxMtrl"));
	}

	void SkyBox::SetSkyBoxTexture(Ptr<Texture> _tex)
	{
		if (_tex->IsCube())
		{
			SetSkyBoxType(SKYBOX_TYPE::CUBE);
			GetMaterial(0)->SetTexParam(TEX_CUBE_0, _tex);
		}
		else
		{
			SetSkyBoxType(SKYBOX_TYPE::SPHERE);
			GetMaterial(0)->SetTexParam(TEX_0, _tex);
		}
	}

	void SkyBox::SaveToLevelFile(FILE* _file)
	{
		fwrite(&type, sizeof(SKYBOX_TYPE), 1, _file);

		RenderComponent::SaveToLevelFile(_file);
	}

	void SkyBox::LoadFromLevelFile(FILE* _file)
	{
		fread(&type, sizeof(SKYBOX_TYPE), 1, _file);

		RenderComponent::LoadFromLevelFile(_file);
	}

	void SkyBox::FinalTick()
	{

	}

	void SkyBox::Render(UINT _i)
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial(0))
			return;

		// Transform 에 UpdateData 요청
		GetTransform()->UpdateData();

		// 재질 업데이트
		GetMaterial(0)->SetScalarParam(INT_0, &type);

		//if (nullptr != skybox_tex)
		//{
		//	if (skybox_tex->IsCube())
		//	{
		//		GetMaterial(0)->SetTexParam(TEX_CUBE_0, skybox_tex);
		//	}
		//	else
		//	{
		//		GetMaterial(0)->SetTexParam(TEX_0, skybox_tex);
		//	}
		//}

		GetMaterial(0)->UpdateData();

		// 렌더
		GetMesh()->Render(0);
	}

	void SkyBox::Render()
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial(0))
			return;

		// Transform 에 UpdateData 요청
		GetTransform()->UpdateData();

		// 재질 업데이트
		GetMaterial(0)->SetScalarParam(INT_0, &type);

		//if (nullptr != skybox_tex)
		//{
		//	if (skybox_tex->IsCube())
		//	{
		//		GetMaterial(0)->SetTexParam(TEX_CUBE_0, skybox_tex);
		//	}
		//	else
		//	{
		//		GetMaterial(0)->SetTexParam(TEX_0, skybox_tex);
		//	}
		//}

		GetMaterial(0)->UpdateData();

		// 렌더
		GetMesh()->Render(0);
	}
}