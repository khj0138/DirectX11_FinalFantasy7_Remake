#include "pch.h"
#include "BarretSmackDownScript.h"
#include "EffectScript.h"

#include "ObjectPoolMgr.h"
#include "CharacterDataScript.h"
#include <Engine\random_number.h>
namespace ff7r
{
	BarretSmackDownScript::BarretSmackDownScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::BARRETSMACKDOWNSCRIPT)
	{
	}
	BarretSmackDownScript::~BarretSmackDownScript()
	{
	}
	void BarretSmackDownScript::Awake()
	{

		/*Ptr<MeshData> _meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Flash_01_M.gltf");
		if (_meshdata != nullptr)
		{
			_effect = _meshdata->Instantiate();
			_effect->SetName(L"Normal Attack Effect");
			_effect->GetTransform()->SetRelativeScale(3.f, 3.f, 3.f);
			_effect->GetTransform()->SetRelativePos(2.5, 1.5f, 0);
			_effect->GetTransform()->SetRelativeRot(0, 0, Degree2Rad(270.f));
			_effect->SetLayerIndex(8);
			_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
			_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(1.0f, 0.5f, 0.0f, 1.0f));
			_effect->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0
				, ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Fire_UV_03_T.png"));

			_effect->AddComponent(new EffectScript);
			_effect->GetScript<EffectScript>()->SetIntensity(4.0f);
			_effect->GetScript<EffectScript>()->SetMaxUVLength(1.0f);
			_effect->GetScript<EffectScript>()->SetDiscardRangeY(vec2(0.0f,0.0f));
			_effect->GetScript<EffectScript>()->SetRangeX(vec2(0.0f,1.0f));
			_effect->GetScript<EffectScript>()->SetRangeY(vec2(0.0f,1.0f));
			_effect->GetScript<EffectScript>()->IsChangeRange(false, false);

			_effect->GetScript<EffectScript>()->SetEnabled(false);
			_effect->SetActive(false);

			GetOwner()->AddChild(_effect);
		}*/
		GetCollider3D()->SetTrigger(true);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(10.0f, 10.0f, 10.0f));
		GetCollider3D()->SetCollision(true);
	}
	void BarretSmackDownScript::Start()
	{
		GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret SmackDown Effect");
		explosion_effect->GetTransform()->SetRelativePos(GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
		
	}
	void BarretSmackDownScript::Tick()
	{
	
	}
	void BarretSmackDownScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		//_time = 0.0f;
		////EffectScript* _effect = GetOwner()->GetScriptInChildren<EffectScript>();
		////_effect->SetEnabled(true);
		//GetOwner()->GetCollider3D()->Disable();


		
		//GetCollider3D()->Disable();
		if (_other->GetOwner()->GetLayerIndex() != 3)
			return;
		int _dmg = (int)(700.f + 300.f * randf(1.f));

		GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Normal Attack Effect");
		_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::UP, _dmg , GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());



		GetOwner()->SetActive(false);
		GetCollider3D()->SetGravity(false);
	}
	void BarretSmackDownScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretSmackDownScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretSmackDownScript::OnEnable()
	{
		GetCollider3D()->SetCollision(true);
	}
	void BarretSmackDownScript::OnDisable()
	{
		GetCollider3D()->Disable();
	}
}