#include "pch.h"

#include <Engine\SoundMgr.h>

#include "PlayerDataScript.h"
#include "PrimaryFireScript.h"
namespace ff7r
{
	PrimaryFireScript::PrimaryFireScript()
		: AttackDataScript(SCRIPT_TYPE::PRIMARYFIRESCRIPT)
		, life_time(0.0f)
	{
	}

	PrimaryFireScript::~PrimaryFireScript()
	{
	}

	void PrimaryFireScript::Awake()
	{
		SetAttackData(ATK_TYPE::PUSH_STRONG, 37);
		GameObject* _owner = GetOwner();
		_owner->SetName(L"Primary Fire");

		_owner->AddComponent(new MeshRender);
		_owner->AddComponent(new Animator2D);
		_owner->AddComponent(new Collider3D);

		Collider3D* collider = GetCollider3D();
		collider->SetTrigger(false);
		collider->SetDynamic(true);
		collider->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		collider->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		collider->SetOffsetScale(vec3(6.f, 6.0f, 6.f));
		collider->SetMove(false);
		collider->SetMass(0.00001f);

		_owner->GetTransform()->SetRelativeScale(vec3(10.f, 10.f, 1.0f));
		GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));

		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_3,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_01N_T.png"));

		_owner->GetMeshRender()->GetMaterial()->SetTexParam(TEX_2,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_01A_T.png"));

		_owner->GetMeshRender()->GetMaterial()->SetTexParam(TEX_1,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_01E_T.png"));

		Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Explosion_01O_T.png");
		Animator2D* _anim = _owner->GetAnimator2D();

		_anim->CreateAnimation(L"Primary_Fire_Anim", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 60.f);
		_anim->Play(L"Primary_Fire_Anim", false);

		GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, vec4(5.0f, 1.0f, 0.5f, 1.0f));
	}

	void PrimaryFireScript::Start()
	{
		SoundMgr::GetInst()->Play(L"sound\\HundredGunner\\Battle_Resident [51].wav", SOUND_OBJECT_TYPE::_2D);
	}

	void PrimaryFireScript::Tick()
	{
		life_time += DT;

		if (life_time >= 2.0f && !GetOwner()->IsDead())
		{
			DestroyObject(GetOwner());
		}
	}

	void PrimaryFireScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		if (_other->GetOwner()->GetLayerIndex() == 0)
		{
			_other->GetOwner()->GetScript<PlayerDataScript>()->Hit(GetAttackType(), GetAttackDamage(), _contact_pos[0]);
		}
	}
}