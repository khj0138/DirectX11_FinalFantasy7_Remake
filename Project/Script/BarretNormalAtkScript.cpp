#include "pch.h"
#include "BarretNormalAtkScript.h"
#include "BWeaponHitEffectScript.h"
#include "CharacterDataScript.h"
#include "EffectScript.h"

#include "ObjectPoolMgr.h"
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
namespace ff7r
{
	BarretNormalAtkScript::BarretNormalAtkScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::BARRETNORMALATKSCRIPT)
		, boom(false)
	{
	}
	BarretNormalAtkScript::~BarretNormalAtkScript()
	{
	}
	void BarretNormalAtkScript::Awake()
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
		GetCollider3D()->SetOffsetScale(vec3(3.0f, 3.0f, 3.0f));
		GetCollider3D()->SetCollision(true);
	}
	void BarretNormalAtkScript::Tick()
	{
	}
	void BarretNormalAtkScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		
		if (_other->GetOwner()->GetLayerIndex() != 3)
			return;
		vec3 _atk_pos = GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos();
		vec3 _other_pos = _other->GetTransform()->GetWorldPos() + _other->GetOffsetPos();
		
		int _dmg = randi(8, 12);

		if (boom)
		{
			//SetAttackData(ATK_TYPE::UP, _dmg * 8);
			GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Normal Attack Effect");
			explosion_effect->GetTransform()->SetRelativePos(_atk_pos);
			boom = false;

			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::UP, _dmg * 7, _atk_pos);
			
			SoundMgr::GetInst()->Play(L"sound\\AtkShoot01_finish_hit.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, true);

		}
		else
		{
			
			GameObject * _hit_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Normal Attack Hit Effect");
			_hit_effect->GetTransform()->SetRelativePos(_atk_pos);
			_hit_effect->GetScript<BWeaponHitEffectScript>()->SetDir((_atk_pos - _other_pos));
			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::DEFAULT, _dmg, _atk_pos);
			SoundMgr::GetInst()->Play(L"sound\\AtkShoot01_bullet.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, true);

		}



		GetOwner()->SetActive(false);
		GetCollider3D()->SetGravity(false);
	}
	void BarretNormalAtkScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretNormalAtkScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretNormalAtkScript::OnEnable()
	{
		GetCollider3D()->SetCollision(true);
	}
	void BarretNormalAtkScript::OnDisable()
	{
		GetCollider3D()->Disable();
	}
}