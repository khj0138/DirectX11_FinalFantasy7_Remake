#include "pch.h"
#include "Decal.h"

#include "ResourceMgr.h"
#include "Mesh.h"
#include "Material.h"

#include "Transform.h"

namespace ff7r
{
	Decal::Decal()
		: RenderComponent(COMPONENT_TYPE::DECAL)
		, is_deferred(false)
		, is_emissive(false)
	{
		SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CubeMesh"));
		SetDeferredDecal(is_deferred);
	}

	Decal::~Decal()
	{

	}


	void Decal::FinalTick()
	{
		DrawDebugCube(GetTransform()->GetWorldMat(), vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
	}

	void Decal::Render()
	{
		if (nullptr == GetMaterial() || nullptr == GetMesh())
			return;

		GetTransform()->UpdateData();

		int IsEmissive = is_emissive;
		GetMaterial()->SetScalarParam(INT_0, &IsEmissive);
		GetMaterial()->UpdateData();

		GetMesh()->Render();
	}



	void Decal::SetDeferredDecal(bool _bDeferred)
	{
		is_deferred = _bDeferred;

		if (is_deferred)
		{
			//SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DeferredDecalMtrl"));
		}
		else
		{
			SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DecalMtrl"));
		}

		GetMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"PositionTarget"));

	}
}