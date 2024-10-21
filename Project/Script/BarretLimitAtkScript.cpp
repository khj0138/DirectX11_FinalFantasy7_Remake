#include "pch.h"
#include "BarretLimitAtkScript.h"
#include "EffectScript.h"

#include "ObjectPoolMgr.h"
#include "CharacterDataScript.h"
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
namespace ff7r
{
	BarretLimitAtkScript::BarretLimitAtkScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::BARRETLIMITATKSCRIPT)
		, max_scale(0.5f)
	{
	}
	BarretLimitAtkScript::~BarretLimitAtkScript()
	{
	}
	void BarretLimitAtkScript::Awake()
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
		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetMove(false);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		GetCollider3D()->SetOffsetPos(vec3(0.0f, 0.0f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(3.0f, 3.0f, 3.0f));

		GetCollider3D()->Disable();


		//GetCollider3D()->SetCollision(true);
	}
	void BarretLimitAtkScript::Start()
	{
		//GetCollider3D()->SetGravity(false);

		GetCollider3D()->SetCollision(false);


	}
	void BarretLimitAtkScript::Tick()
	{
		vec3 _scale = GetTransform()->GetRelativeScale();
		if (_scale.x < max_scale)
		{
			if (_scale.x <= 0.2f)
			{
				SoundMgr::GetInst()->Play(L"sound\\Limit1_charge.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, false);
			}
			_scale *= 1.01f;
			if (_scale.x > max_scale)
			{
				_scale = vec3(max_scale, max_scale, max_scale);
			}
			GetTransform()->SetRelativeScale(_scale);
		}
		if (boom_cnt == 1)
		{
			time += DT;
			if (time >= 0.3f && !GetCollider3D()->GetCollision())
			{

				GetCollider3D()->SetCollision(true);
				velocity = vec3(0.0f, -1.0f, 0.0f);

			}
		}
		else if (velocity.Length() > 0.0f && !GetCollider3D()->GetCollision())
		{
			GetCollider3D()->SetCollision(true);


		}
		GetTransform()->ShiftRelativePos(velocity * DT);
		//_time += DT;
		//if (_time >= 1.0f)
		//{
		//	_time = 0.0f;
		//	//EffectScript* _effect = GetOwner()->GetScriptInChildren<EffectScript>();
		//	//_effect->SetEnabled(false);
		//	GetOwner()->SetActive(false);
		//}
	}
	void BarretLimitAtkScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		//_time = 0.0f;
		////EffectScript* _effect = GetOwner()->GetScriptInChildren<EffectScript>();
		////_effect->SetEnabled(true);
		//GetOwner()->GetCollider3D()->Disable();

		float _dmg = 3000 + (int)(randf(1.f) * 2000.f);
		if (_other->GetOwner()->GetLayerIndex() == 3)
		{
			if (boom_cnt == 0)
			{
				GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Limit Attack Effect");
				explosion_effect->GetTransform()->SetRelativePos(GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
				velocity = vec3(0.0f, 1.0f, 0.0f);
				GetCollider3D()->Disable();
				time = 0.0f;

				_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::DOWN, _dmg, GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
				SoundMgr::GetInst()->Play(L"sound\\Limit1_bullet.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, false);
			}
			else if (boom_cnt == 1)
			{
				GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Limit Attack Effect");
				explosion_effect->GetTransform()->SetRelativePos(GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
				//GetCollider3D()->Disable();
				GetOwner()->SetActive(false);
				//GetCollider3D()->SetGravity(false);
				_dmg += (int)(randf(1.f) * 2000.f);
				_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::DOWN, _dmg, GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
				GetOwner()->GetScript<EffectScript>()->SetEnabled(false);
				SoundMgr::GetInst()->Play(L"sound\\Limit1_hit.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, false);

			}
			boom_cnt++;
		}
		/*else if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			velocity = vec3(0.0f, 3.0f, 0.0f);
		}*/
	}
	void BarretLimitAtkScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretLimitAtkScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		/*if (_other->GetOwner()->GetLayerIndex() == 1)
		{
			velocity = vec3(0.0f, 3.0f, 0.0f);
		}*/
	}
	void BarretLimitAtkScript::OnEnable()
	{
		//GetCollider3D()->SetCollision(true);

		boom_cnt = 0;
		GetTransform()->SetRelativeScale(vec3(0.1f, 0.1f, 0.1f));
	}
	void BarretLimitAtkScript::OnDisable()
	{
		GetCollider3D()->Disable();
		velocity = vec3(0.0f, 0.0f, 0.0f);
	}
}