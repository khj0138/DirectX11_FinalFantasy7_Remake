#include "pch.h"

#include <Engine\MeshData.h>
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
#include <Engine\GameObject.h>

#include "ObjectPoolMgr.h"
#include "UIMgr.h"
#include "EffectScript.h"
#include "CWeaponHitEffectScript.h"
#include "CWeaponFlashScript.h"
#include "CWeaponSparkScript.h"
#include "CSlashHitEffectScript.h"
#include "CharacterDataScript.h"
#include "LensEffectScript.h"
#include "PlayerDataScript.h"

#include "CloudWeaponScript.h"
namespace ff7r
{
	CloudWeaponScript::CloudWeaponScript()
		: AttackDataScript((UINT)SCRIPT_TYPE::CLOUDWEAPONSCRIPT)
		, on_collider(false)
		, change(false)
		, flash_effect(nullptr)
		, cloud(nullptr)
		, time(0.f)
	{
	}
	CloudWeaponScript::~CloudWeaponScript()
	{
	}

	void CloudWeaponScript::Awake()
	{
		GameObject* spark = new GameObject;
		spark->SetName(L"Cloud Weaopn Spark");
		spark->AddComponent(new Transform);
		spark->AddComponent(new MeshRender);
		spark->AddComponent(new EffectScript);
		spark->AddComponent(new CWeaponSparkScript);
		spark->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(spark, L"Cloud Weapon Spark");

		GetCollider3D()->SetTrigger(false);
		GetCollider3D()->SetDynamic(true);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		GetCollider3D()->SetOffsetScale(vec3(1.8f, 0.7f, 0.5f));
		GetCollider3D()->SetOffsetPos(vec3(1.1f, 0.0f, 0.0f));
		GetCollider3D()->SetMove(false);
		GetCollider3D()->SetMass(0.00001f);

		CreateEffect();

		light = new GameObject;
		light->SetName(L"Effect");
		light->AddComponent(new Transform);
		light->AddComponent(new MeshRender);
		light->AddComponent(new EffectScript);
		light->GetTransform()->SetRelativeScale(vec3(5.f, 5.f, 1.0f));
		light->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		light->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardEffectMtrl"));
		light->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Glow_02_T.png"));
		
		light->GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, vec4(235.f / 255.f, 47.f / 255.f, 27.f / 255.f, 1.0f));
		light->GetScript<EffectScript>()->SetIntensity(7.0f);
		
		
		SpawnGameObject(light, vec3(0.f, 0.f, 0.f), L"Effect");
		light->SetActive(false);
		light->SetDestroy(false);
	}

	void CloudWeaponScript::Start()
	{
		GetCollider3D()->Disable();
	}

	void CloudWeaponScript::Tick()
	{
		if (time >= 0.0f)
		{
			time -= DT;
			float _intensity = lerp(0.1f, 6.0f, time / 0.05f);
			light->GetScript<EffectScript>()->SetIntensity(_intensity);
			if (time <= 0.0f)
			{
				light->SetActive(false);
			}
		}
		if (change)
		{
			on_collider = !on_collider;

			if (on_collider)
			{
				GetCollider3D()->SetCollision(on_collider);
			}
			else
			{
				GetCollider3D()->Disable();
			}
			colliders.clear();
			change = false;
		}

		prev_pos = GetTransform()->GetWorldPos();
	}

	void CloudWeaponScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		for (Collider3D* colli : colliders)
		{
			if (colli == _other)
			{
				return;
			}
		}

		colliders.push_back(_other);
		GameObject* _other_obj = _other->GetOwner();

		vec3 _cur_pos = GetTransform()->GetWorldPos();

		vec3 _close_pos = _contact_pos[0];
		float _len = vec3(_cur_pos - _close_pos).Length();

		for (size_t i = 1; i < _contact_pos.size(); i++)
		{
			float _len_2 = vec3(_cur_pos - _contact_pos[i]).Length();

			if (_len_2 < _len)
			{
				_len = _len_2;
				_close_pos = _contact_pos[i];
			}
		}

		GameObject* _hit_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"CWeapon Hit Effect");
		_hit_effect->GetScript<CWeaponHitEffectScript>()->SetDir((prev_pos - _cur_pos));
		_hit_effect->GetParticleSystem()->SetGroundYPos(GetOwner()->GetParent()->GetParent()->GetTransform()->GetRelativePos().y);
		_hit_effect->GetTransform()->SetRelativePos(_close_pos);


		if (type == WEAPON_TYPE::BRAVER)
		{
			Transform* cloud_trans = GetOwner()->GetParent()->GetParent()->GetTransform();
			GameObject* t = ObjectPoolMgr::GetInst()->GetGameObject(L"Break Effect");
			t->GetTransform()->SetRelativePos(cloud_trans->GetRelativePos() + cloud_trans->GetWorldDir(DIR_TYPE::RIGHT) * 5.5f);
			TimeMgr::GetInst()->SetTimeSpeed(0.01f, 0.1f);
		}
		else
		{
			for (int i = 0; i < 7; i++)
			{
				GameObject* spark_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Cloud Weapon Spark");
				spark_effect->GetScript<CWeaponSparkScript>()->SetSpawnPosition(_close_pos, prev_pos - _cur_pos);
			}
		}


		light->GetTransform()->SetRelativePos(_close_pos);
		light->SetActive(true);
		time = 0.05f;

		int _sound_idx = randi(2);
		SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\CWeaponHit" + to_wstring(_sound_idx) + L".wav", SOUND_OBJECT_TYPE::_3D);

		cloud->IncreaseAtbGauge(0.1f);
		cloud->IncreaseLimitGauge(0.05f);

		_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(GetAttackType(), GetAttackDamage(), _close_pos);
	}

	void CloudWeaponScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
	}

	void CloudWeaponScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		int a = 0;
	}

	void CloudWeaponScript::ChangeWeaponType(WEAPON_TYPE _type)
	{
		type = _type;

		switch (type)
		{
		case WEAPON_TYPE::ATK0:
			SetAttackData(ATK_TYPE::PUSH_WEAK, 10);
			SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\CWeapon1.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);

			break;

		case WEAPON_TYPE::ATK1:
			SetAttackData(ATK_TYPE::PUSH_NONE, 20);
			SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\CWeapon2.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);
			break;

		case WEAPON_TYPE::ATK2:
			SetAttackData(ATK_TYPE::DOWN, 30);
			SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\CWeapon3.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);
			break;

		case WEAPON_TYPE::ATK3:
			SetAttackData(ATK_TYPE::UP, 40);
			SoundMgr::GetInst()->Play(L"sound\\Cloud\\Weapon\\CWeapon4.wav", SOUND_OBJECT_TYPE::_3D, GetOwner(), 0.6f);
			break;
			 
		default: break;
		}
		colliders.clear();
	}

	void CloudWeaponScript::PlayCounterStanceEffect(bool _is_play)
	{
		if (_is_play)
		{
			flash_effect->SetActive(true);
			for (size_t i = 0; i < fire_effect.size(); i++)
			{
				fire_effect[i]->SetActive(true);
				fire_effect[i]->GetAnimator2D()->Play(L"Fire0", true, randi(25));
			}
		}
		else
		{
			flash_effect->SetActive(false);
			for (size_t i = 0; i < fire_effect.size(); i++)
			{
				fire_effect[i]->SetActive(false);
				// fire_effect[i]->GetAnimator2D()->SetAc
			}
		}
	}

	void CloudWeaponScript::SetCollider(bool _on)
	{
		if (on_collider != _on)
		{
			change = true;
		}
	}

	void CloudWeaponScript::CreateEffect()
	{
		flash_effect = new GameObject;
		flash_effect->SetName(L"Flash Effect");

		flash_effect->AddComponent(new Transform);
		flash_effect->AddComponent(new MeshRender);
		flash_effect->AddComponent(new EffectScript);
		flash_effect->AddComponent(new CWeaponFlashScript);

		flash_effect->GetTransform()->SetAbsoluteRot(true);

		flash_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		flash_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardEffectMtrl"));
		flash_effect->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Flash_03_T.png"));

		flash_effect->GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, vec4(1.0f, 0.5f, 0.2f, 1.0f));
		flash_effect->GetScript<EffectScript>()->SetIntensity(5.5f);

		flash_effect->SetLayerIndexByName(L"Effect");
		flash_effect->SetActive(false);
		GetOwner()->AddChild(flash_effect);


		for (int i = 0; i < 6; i++)
		{
			GameObject* _fire = new GameObject;
			_fire->SetName(L"Fire");
			_fire->AddComponent(new Transform);
			_fire->AddComponent(new MeshRender);
			_fire->AddComponent(new Animator2D);
			_fire->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
			_fire->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));

			Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Smk_07A_T.png");
			Animator2D* _anim = _fire->GetAnimator2D();

			if (i == 3)
			{
				_fire->GetTransform()->SetRelativeScale(vec3(2.5f, 1.f, 1.0f));
				_fire->GetTransform()->SetRelativePos(vec3((float)1, 0.f, 0.0f));
				_fire->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(5.0f, 0.6f, 0.4f, 1.0f));
				_anim->CreateAnimation(L"Fire0", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 120.f);
			}
			else if (i == 2)
			{
				_fire->GetTransform()->SetRelativeScale(vec3(2.0f, 1.f, 1.0f));
				_fire->GetTransform()->SetRelativePos(vec3((float)1.2, 0.f, 0.0f));
				_fire->GetTransform()->SetRelativeRot(vec3(0.0f, XM_PI, 0.0f));
				_fire->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(10.0f, 0.6f, 0.2f, 1.0f));
				_anim->CreateAnimation(L"Fire0", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 140.f);
			}
			else
			{
				_fire->GetTransform()->SetRelativeScale(vec3(1.f + i * 0.1f, 1.f, 1.0f));
				_fire->GetTransform()->SetRelativePos(vec3((float)i / 3.f, 0.f, 0.0f));
				_fire->GetTransform()->SetRelativeRot(vec3((float)i / 3.f, (float)(i - 2) / 3.f, 0.0f));
				_fire->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f + (float)i * 2.f, 0.6f + (float)i * 0.1f, 0.2f, 1.0f));
				_anim->CreateAnimation(L"Fire0", _tex, vec2(0.0f, 0.0f), vec2(8.0f, 8.0f), 100.f + i * 3);
			}

			_fire->SetLayerIndexByName(L"Effect");
			_fire->SetActive(false);
			fire_effect.push_back(_fire);
			GetOwner()->AddChild(_fire);
		}
	}

	void CloudWeaponScript::SpawnSlashHitEffect(vec3 _pos)
	{
		Ptr<MeshData> _mdat = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Character\\Player\\PC0000_Cloud\\Model\\FX_PC0000_Slash_01_M.gltf");
		if (_mdat != nullptr)
		{
			int _spawn_cnt = randi(5) + 5;

			for (int i = 0; i < _spawn_cnt; i++)
			{
				GameObject* _effect = _mdat->Instantiate();
				_effect->SetName(L"Slash Hit Effect");
				_effect->AddComponent(new CSlashHitEffectScript);
				_effect->AddComponent(new EffectScript);

				SpawnGameObject(_effect, _pos, L"Effect");
			}
		}
	}
}