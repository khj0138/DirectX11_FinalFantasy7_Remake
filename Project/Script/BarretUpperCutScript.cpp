#include "pch.h"
#include "BarretUpperCutScript.h"
#include "EffectScript.h"

#include "ObjectPoolMgr.h"
#include "CharacterDataScript.h"
#include <Engine\SoundMgr.h>
#include <Engine\random_number.h>
namespace ff7r
{
	BarretUpperCutScript::BarretUpperCutScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::BARRETUPPERCUTSCRIPT)
		, charging(false)
	{
	}
	BarretUpperCutScript::~BarretUpperCutScript()
	{
	}
	void BarretUpperCutScript::Awake()
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
		GetCollider3D()->SetOffsetScale(vec3(8.0f,8.0f, 8.0f));
		GetCollider3D()->SetCollision(true);
		charging = false;
	}
	void BarretUpperCutScript::Start()
	{
		

	}
	void BarretUpperCutScript::Tick()
	{
		//_time += DT;
		//if (_time >= 1.0f)
		//{
		//	_time = 0.0f;
		//	//EffectScript* _effect = GetOwner()->GetScriptInChildren<EffectScript>();
		//	//_effect->SetEnabled(false);
		//	GetOwner()->SetActive(false);
		//}
	}
	void BarretUpperCutScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		//_time = 0.0f;
		////EffectScript* _effect = GetOwner()->GetScriptInChildren<EffectScript>();
		////_effect->SetEnabled(true);
		//GetOwner()->GetCollider3D()->Disable();

		if (_other->GetOwner()->GetLayerIndex() != 3)
			return;
		if (!charging)
		{
			int _dmg = (int)(100.f + 50.f * randf(1.f));
			SoundMgr::GetInst()->Play(L"sound\\bulst7_HIt_01.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, false);
			//GetTransform()->CalcWorldDir();
			vec3 _pos = GetTransform()->GetWorldPos();
			vec3 _other_pos = _other->GetTransform()->GetWorldPos();
			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::DEFAULT, _dmg,(_pos + _other_pos) / 2.0f);
			
			GetOwner()->GetCollider3D()->Disable();
			charging = true;
		}
		else
		{
			int _dmg = (int)(400.f + 200.f * randf(1.f));
			GetTransform()->CalcWorldDir();
			vec3 _pos = GetTransform()->GetWorldPos();
			vec3 _other_pos = _other->GetTransform()->GetWorldPos();
			_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::UP, _dmg, (_pos + _other_pos) / 2.0f);
			GetOwner()->SetActive(false);
			GetCollider3D()->Disable();
			SoundMgr::GetInst()->Play(L"sound\\bulst7_HIt_02.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, false);

			
			GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret UpperCut Effect");
			explosion_effect->GetTransform()->SetRelativePos((_pos + _other_pos) / 2.0f);
			SoundMgr::GetInst()->Play(L"sound\\bulst7_Shot.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1, false);
			charging = false;

		}


		
		
	}
	void BarretUpperCutScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretUpperCutScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}
	void BarretUpperCutScript::OnEnable()
	{
		charging = false;

		
		/*else
		{
			charging = true;
		}
		GetCollider3D()->SetCollision(true);*/
	}
	void BarretUpperCutScript::OnDisable()
	{
		GetCollider3D()->Disable();
	}
}