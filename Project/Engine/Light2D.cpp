#include "pch.h"
#include "Light2D.h"

#include "RenderMgr.h"
#include "Transform.h"

#include "KeyMgr.h"
#include "TimeMgr.h"

namespace ff7r
{
	Light2D::Light2D()
		: Component(COMPONENT_TYPE::LIGHT2D)
	{
	}

	Light2D::~Light2D()
	{
	}

	void Light2D::FinalTick()
	{
		if (KEY_PRESSED(KEY::C))
		{
			vec3 vRot = GetTransform()->GetRelativeRot();
			vRot.z += DT * (XM_PI / 2.f);
			GetTransform()->SetRelativeRot(vRot);
		}

		light_info.world_pos = GetTransform()->GetWorldPos();
		light_info.world_dir = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		RenderMgr::GetInst()->RegisterLight2D(this, light_info);
	}

	void Light2D::SaveToLevelFile(FILE* _File)
	{
		fwrite(&light_info, sizeof(LightInfo), 1, _File);
	}

	void Light2D::LoadFromLevelFile(FILE* _File)
	{
		fread(&light_info, sizeof(LightInfo), 1, _File);
	}
}