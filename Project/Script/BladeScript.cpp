#include "pch.h"

#include <Engine\random_number.h>

#include "CharacterDataScript.h"
#include "EffectScript.h"

#include "BladeScript.h"
namespace ff7r
{
	BladeScript::BladeScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::BLADESCRIPT)
		, time(0.f)
	{
	}

	BladeScript::~BladeScript()
	{
	}

	void BladeScript::Awake()
	{
		GetTransform()->SetRelativeScale(vec3(0.2f, 5.f, 0.2f));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Shock_01_T.png"));

		GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, vec4(0.2f, 0.7f, 0.2f, 1.0f));
		GetOwner()->GetScript<EffectScript>()->SetIntensity(4.0f);
		GetOwner()->GetScript<EffectScript>()->SetDiscardRangeY(vec2(0.5f, 1.0f));

		Collider3D* collider = GetCollider3D();
		collider->SetTrigger(false);
		collider->SetDynamic(true);
		collider->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		collider->SetOffsetScale(vec3(0.5f, 5.f, 0.5f));
		collider->SetOffsetPos(vec3(0.f, 0.0f, 0.f));
		collider->SetMove(false);

		collider->SetMass(0.00001f);

		SetAttackData(ATK_TYPE::UP, 15);
	}

	void BladeScript::Start()
	{

		GetCollider3D()->SetGravity(false);
	}

	void BladeScript::Tick()
	{
		GetCollider3D()->SetMove(false);
		time += DT;

		if (time >= 0.5f)
		{
			GetOwner()->SetActive(false);
		}

		vec3 _cur_scale = vec3::Lerp(scale, vec3(0.0f, 0.0f, 0.0f), time / 0.5f);
		GetTransform()->SetRelativeScale(_cur_scale);
	}

	void BladeScript::OnEnable()
	{
		GetCollider3D()->SetCollision(true);
		GetTransform()->SetRelativeRotY(randf(Degree2Rad(360.f)));
		time = 0.0f;
	}

	void BladeScript::OnDisable()
	{
		//GetCollider3D()->Disable();
	}

	void BladeScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 3)
		{
			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(GetAttackType(), GetAttackDamage());
		}
	}

	void BladeScript::SetBladeIdx(int _idx)
	{
		scale = vec3(0.45f, 5.f * _idx * 0.7f, 0.45f);
	}
}