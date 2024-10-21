#include "pch.h"

#include <Engine\random_number.h>

#include "CharacterDataScript.h"

#include "HundredBulletScript.h"
namespace ff7r
{
	HundredBulletScript::HundredBulletScript()
		: AttackDataScript(SCRIPT_TYPE::HUNDREDBULLETSCRIPT)
		, speed(15.0f)
		, active_time(0.0f)
	{
	}
	HundredBulletScript::~HundredBulletScript()
	{
	}

	void HundredBulletScript::Awake()
	{
		SetAttackData(ATK_TYPE::DEFAULT, 3);
		
		GetTransform()->SetRelativeScale(3.0f, 3.0f, 3.0f);

		Collider3D* collider = GetCollider3D();
		collider->SetTrigger(false);
		collider->SetDynamic(true);
		collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		collider->SetOffsetPos(vec3(-.04f, 0.3f, 0.0f));
		collider->SetOffsetScale(vec3(0.15f, 0.15f, 0.15f));
		collider->SetMove(false);
		collider->SetMass(0.00001f);
	}

	void HundredBulletScript::Start()
	{
	}

	void HundredBulletScript::Tick()
	{
		active_time += DT;

		if (active_time >= 3.0f)
		{
			GetOwner()->SetActive(false);
			GetCollider3D()->Disable();
		}

		GetTransform()->ShiftRelativePos(GetTransform()->GetWorldDir(DIR_TYPE::UP) * speed * DT);

	}

	void HundredBulletScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			GetOwner()->SetActive(false);
			GetCollider3D()->Disable();
		}
		else
		{
			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(GetAttackType(), GetAttackDamage(), _contact_pos[0]);
		}
	}

	void HundredBulletScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void HundredBulletScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void HundredBulletScript::OnEnable()
	{
		active_time = 0.0f;
		GetCollider3D()->SetCollision(true);
	}

	void HundredBulletScript::SetTargetPos(vec3 pos)
	{
		vec3 _dir = pos - GetTransform()->GetRelativePos();
		_dir.Normalize();

		_dir.x += randf(0.2f) - 0.1f;
		_dir.y += randf(0.2f) - 0.1f;
		_dir.z += randf(0.2f) - 0.1f;
		_dir.Normalize();

		dir = _dir;

		vec3 front = { 0.0f, 1.0f, 0.0f };
		vec3 rotationAxis = XMVector3Cross(front, dir);

		float dotProduct = XMVectorGetX(XMVector3Dot(front, dir));
		float angle = acos(dotProduct);

		XMVECTOR rotationQuaternion = XMQuaternionRotationAxis(rotationAxis, angle);
		XMFLOAT4 quaternion;
		XMStoreFloat4(&quaternion, rotationQuaternion);

		Matrix mat = {};
		mat = mat.CreateFromQuaternion(quaternion);

		vec3 rot;
		rot.x = atan2f(mat._23, mat._33);
		rot.y = atan2f(-mat._13, sqrtf(mat._23 * mat._23 + mat._33 * mat._33));
		rot.z = atan2f(mat._12, mat._11);
		
		GetTransform()->SetRelativeRot(rot);
	}
}