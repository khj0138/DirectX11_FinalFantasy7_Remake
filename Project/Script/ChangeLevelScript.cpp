#include "pch.h"

#include "func.h"
#include "ChangeLevelScript.h"
namespace ff7r
{
	ChangeLevelScript::ChangeLevelScript()
		: Script((UINT)SCRIPT_TYPE::CHANGELEVELSCRIPT)
	{
	}
	ChangeLevelScript::~ChangeLevelScript()
	{
	}

	void ChangeLevelScript::Awake()
	{
		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(false);
		GetCollider3D()->SetOffsetScale(vec3(30.f,20.f, 3.f));
	}
	void ChangeLevelScript::Tick()
	{
	}

	void ChangeLevelScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		CreateLoadLevelFunc();
	}
}