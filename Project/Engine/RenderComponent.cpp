#include "pch.h"
#include "RenderComponent.h"

#include "Transform.h"

#include "ResourceMgr.h"
#include "Material.h"

namespace ff7r
{
	RenderComponent::RenderComponent(COMPONENT_TYPE _type)
		: Component(_type)
		, frustum_check(true)
		, bounding(500.f)
		, dynamice_shadow(false)
		, is_render(true)
	{
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::RenderShadowmap()
	{
		Ptr<Material> pShadowMapMtrl = ResourceMgr::GetInst()->FindRes<Material>(L"ShadowMapMtrl");

		GetTransform()->UpdateData();

		pShadowMapMtrl->UpdateData();

		GetMesh()->Render();
	}

	void RenderComponent::SetMesh(Ptr<Mesh> _mesh)
	{
		mesh = _mesh;

		if (!materials.empty())
		{
			materials.clear();
			vector<MtrlSet> _mtrls;
			materials.swap(_mtrls);
		}

		if (nullptr != mesh)
			materials.resize(mesh->GetSubsetCount());
	}

	void RenderComponent::SetMaterial(Ptr<Material> _mtrl, UINT _idx)
	{
		materials[_idx].shared_mtrl = _mtrl;
		materials[_idx].current_mtrl = _mtrl;
	}

	Ptr<Material> RenderComponent::GetMaterial(UINT _idx)
	{
		//if()
		if (nullptr == materials[_idx].current_mtrl)
		{
			materials[_idx].current_mtrl = materials[_idx].shared_mtrl;
		}

		return materials[_idx].current_mtrl;
	}

	Ptr<Material> RenderComponent::GetSharedMaterial(UINT _idx)
	{
		materials[_idx].current_mtrl = materials[_idx].shared_mtrl;

		if (materials[_idx].dynamic_mtrl.Get())
		{
			materials[_idx].dynamic_mtrl = nullptr;
		}

		return materials[_idx].shared_mtrl;
	}

	Ptr<Material> RenderComponent::GetDynamicMaterial(UINT _idx)
	{
		// 원본 재질이 없다 -> Nullptr 반환
		if (nullptr == materials[_idx].shared_mtrl)
		{
			materials[_idx].current_mtrl = nullptr;
			return materials[_idx].current_mtrl;
		}

		if (nullptr == materials[_idx].dynamic_mtrl)
		{
			materials[_idx].dynamic_mtrl = materials[_idx].shared_mtrl->Clone();
			materials[_idx].dynamic_mtrl->SetName(materials[_idx].shared_mtrl->GetName() + L"_Clone");
			materials[_idx].current_mtrl = materials[_idx].dynamic_mtrl;
		}

		return materials[_idx].current_mtrl;
	}

	ULONG64 RenderComponent::GetInstID(UINT _mtrl_idx)
	{
		if (mesh == NULL || materials[_mtrl_idx].current_mtrl == NULL)
		{
			return 0;
		}

		InstID id{ (UINT)mesh->GetID(), (WORD)materials[_mtrl_idx].current_mtrl->GetID(), (WORD)_mtrl_idx };
		return id.llID;
	}

	void RenderComponent::SaveToLevelFile(FILE* _file)
	{
		//COMPONENT_TYPE _type = GetType();
		//fwrite(&_type, sizeof(UINT), 1, _file);

		SaveResRef(mesh.Get(), _file);

		UINT _mtrl_cnt = GetMtrlCount();
		fwrite(&_mtrl_cnt, sizeof(UINT), 1, _file);

		for (UINT i = 0; i < _mtrl_cnt; ++i)
		{
			SaveResRef(materials[i].shared_mtrl.Get(), _file);
		}

		fwrite(&dynamice_shadow, 1, 1, _file);
		fwrite(&frustum_check, 1, 1, _file);
		fwrite(&bounding, 1, 1, _file);
	}

	void RenderComponent::LoadFromLevelFile(FILE* _file)
	{
   		LoadResRef(mesh, _file);

		UINT _mtrl_cnt = GetMtrlCount();
		fread(&_mtrl_cnt, sizeof(UINT), 1, _file);

		materials.resize(_mtrl_cnt);

		for (UINT i = 0; i < _mtrl_cnt; ++i)
		{
			Ptr<Material> _mtrl;
			LoadResRef(_mtrl, _file);
			SetMaterial(_mtrl, i);
		}

		fread(&dynamice_shadow, 1, 1, _file);
		fread(&frustum_check, 1, 1, _file);
		fread(&bounding, 1, 1, _file);
	}
}