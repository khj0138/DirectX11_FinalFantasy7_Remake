#include "pch.h"

#include "BeamScript.h"
namespace ff7r
{
	BeamScript::BeamScript()
		: Script((UINT)SCRIPT_TYPE::BEAMSCRIPT)
		, time(0.f)
	{
	}

	BeamScript::~BeamScript()
	{
	}

	void BeamScript::Awake()
	{
	}

	void BeamScript::Tick()
	{
		time += DT;
		if (time >= 0.5f)
		{
			//GetTransform()->SetRelativeScaleX(6.0f);

			//if (time >= 1.3f)
			//{
			//	GetTransform()->ShiftRelativeRot(rot * DT * 2.0f);
			//}
			//else
			{
				GetTransform()->ShiftRelativeRot(-rot * DT * 2.0f);
			}
			
			//if (fabs(Rad2Degree(GetTransform()->GetRelativeRot().y)) >= 45.f)
			//{
			//	GetOwner()->SetActive(false);
			//}
			if (time >= 1.5f)
			{
				GetOwner()->SetActive(false);
			}
		}
		else
		{
			//float scale_x = lerp(0.f, 6.0f, time / 1.0f);
			//GetTransform()->SetRelativeScaleX(scale_x);
		}
	}

	void BeamScript::OnEnable()
	{
		GetTransform()->SetRelativeRot(0.f, 0.f, 0.f);
		time = 0.0f;
	}

	void BeamScript::SetRot(vec3 _rot)
	{
		rot = _rot;
		GetTransform()->ShiftRelativeRot(_rot);
	}
}