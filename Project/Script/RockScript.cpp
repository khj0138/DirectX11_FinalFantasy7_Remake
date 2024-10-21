#include "pch.h"

#include <Engine\random_number.h>
#include "RockScript.h"
namespace ff7r
{
	RockScript::RockScript()
		: Script((UINT)SCRIPT_TYPE::ROCKSCRIPT)
		, first_tick(true)
	{
	}

	RockScript::~RockScript()
	{
	}

	void RockScript::Awake()
	{
		Collider3D* collider = GetCollider3D();
		collider->SetTrigger(false);
		collider->SetDynamic(true);
		collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		collider->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		collider->SetOffsetScale(vec3(0.5f, 0.5f, 0.5f));
		collider->SetMove(true);
		collider->SetMass(0.5f);
	}

	void RockScript::Start()
	{
		//float x = randf(100.f) - 50.f;
		//float y = randf(100.f) - 50.f;
		//float z = randf(100.f) - 50.f;
		//
		//GetCollider3D()->AddForce(vec3(x, y, z));
	}

	void RockScript::Tick()
	{
		if (first_tick)
		{
			float x = randf(500.f) - 250.f;
			float y = randf(250.f) - 125.f;
			float z = randf(500.f) - 250.f;

			GetCollider3D()->AddForce(vec3(x, y, z));

			GetTransform()->ShiftRelativePos(vec3(x, y, z) / 30.f);
			first_tick = false;
		}

		//float x = randf(300.f) - 150.f;
		//float y = randf(300.f) - 150.f;
		//float z = randf(300.f) - 150.f;

		//GetCollider3D()->AddForce(vec3(x, y, z));
		time += DT;

		if (time >= 4.5f)
		{
			DestroyObject(GetOwner());
			//GetOwner()->SetActive(false);
		}
	}
	void RockScript::OnEnable()
	{
		first_tick = true;
		time = 0.0f;

	}
}