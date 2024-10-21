#include "pch.h"

#include <Engine\RenderMgr.h>

#include "LensEffectScript.h"
namespace ff7r
{
	LensEffectScript::LensEffectScript()
		: Script((UINT)SCRIPT_TYPE::LENSEFFECTSCRIPT)
	{
	}

	LensEffectScript::~LensEffectScript()
	{
	}

	void LensEffectScript::Awake()
	{
		GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"LensMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, RenderMgr::GetInst()->GetBlurRenderTarget());
		GetMeshRender()->GetMaterial()->SetTexParam(TEX_1,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Lens_01COL_T.png"));
	}

	void LensEffectScript::Tick()
	{
		static float time = 0.0f;
		time += DT;

		vec3 scale = vec3(1.0f, 1.0f, 1.f);

		GetTransform()->SetRelativeScale(scale * time);

		if (time >= 2.f)
		{
			DestroyObject(GetOwner());
			time = 0.0f;
		}
	}
}