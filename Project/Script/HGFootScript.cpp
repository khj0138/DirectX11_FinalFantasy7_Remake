#include "pch.h"

#include "Engine\Collider3D.h"
#include "UIMgr.h"
#include "HGFootScript.h"

namespace ff7r
{
	HGFootScript::HGFootScript()
	{
	}
	HGFootScript::~HGFootScript()
	{
	}

	void HGFootScript::Awake()
	{
		SetMaxHP(300.f);

		GetOwner()->AddComponent(new Collider3D());

		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(false);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.f, -0.5f));
		GetCollider3D()->SetOffsetScale(vec3(1.f, 1.f, 1.f));
		GetCollider3D()->SetMove(false);
		GetCollider3D()->SetMass(10.f);	

		const auto& children = GetOwner()->GetParent()->GetChild();
		for (const auto& child : children)
		{
			if (child != GetOwner())
			{
				break_object = child;
				child->GetRenderComponent()->SetRender(false);
				break;
			}
		}

	}

	void HGFootScript::Tick()
	{
	}
	void HGFootScript::Start()
	{
	}
	void HGFootScript::Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos)
	{
		CharacterDataScript::Hit(_type, _damage, _contact_pos);

		if (GetCurHP() <= 0.f)
		{
			GetCollider3D()->Disable();
			break_object->GetRenderComponent()->SetRender(true);
			GetOwner()->GetRenderComponent()->SetRender(false);
			return;
		}
	}
	void HGFootScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void HGFootScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void HGFootScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
}