#include "pch.h"
#include "SoldierWeaponState.h"

#include "BWeaponHitEffectScript.h"
#include "BWeaponLimitEffectScript.h"
#include "ObjectPoolMgr.h"
#include "EffectScript.h"
#include "BWeaponShotEffectScript.h"
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
namespace ff7r
{

	void SoldierNormalAtkState::Enter()
	{
		shoot_idx = 0;
	}
	void SoldierNormalAtkState::Excute()
	{
		GameObject* _target = weapon->GetTarget();
		if (_target != nullptr)
		{
			shoot_idx++;
			GameObject* _muzzle = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Muzzle Effect");
			_muzzle->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos());


			GameObject* _normal_atk = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret_Normal_Atk");

			vec3 _weapon_pos = weapon->GetTransform()->GetWorldPos();
			vec3 _target_pos = _target->GetTransform()->GetWorldPos();
			vec3 _target_scale = _target->GetCollider3D()->GetOffsetScale();
			_target_pos.y += _target_scale.y * 0.5f;

			vec3 _target_vec = _target_pos - _weapon_pos;
			vec3 _random_vec = vec3(randf(1.0f), randf(1.0f), randf(1.0f)) * 0.3f;
			_normal_atk->GetTransform()->SetRelativePos(_weapon_pos + _target_vec.Normalize() * (_target_vec.Length() - _target_scale.x * 0.5f) + _random_vec);
			if (shoot_idx == 20)
			{
				//_normal_atk->GetScript<BarretNormalAtkScript>()->SetBoom(true);
				SoundMgr::GetInst()->Play(L"sound\\AtkShoot01_finish_shot.wav", SOUND_OBJECT_TYPE::_3D, weapon->GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1);

			}
		}
	}
	void SoldierNormalAtkState::Exit()
	{

	}

	void SoldierNormalAtkState::CreateObjects()
	{
		//// ResourceMgr::GetInst()->Load<Sound>(L"test_sound", L"sound\\ak47-168856.mp3");
		//GameObject* _normal_attack = new GameObject();
		//_normal_attack->AddComponent(new Transform);
		//_normal_attack->AddComponent(new Collider3D);
		//_normal_attack->AddComponent(new SoldierNormalAtkScript);
		//Collider3D* _collider = _normal_attack->GetCollider3D();
		//_collider->SetDynamic(false);
		//_collider->SetTrigger(true);
		//_collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		//_normal_attack->SetLayerIndexByName(L"PlayerProjectile");
		//ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack, L"Soldier_Normal_Atk");



		//GameObject* _normal_attack_effect = new GameObject();


		//_normal_attack_effect->SetName(L"Soldier Normal Attack Effect");
		//_normal_attack_effect->AddComponent(new Transform);
		//_normal_attack_effect->AddComponent(new MeshRender);
		//_normal_attack_effect->AddComponent(new Animator2D);
		//_normal_attack_effect->AddComponent(new SoldierNormalAtkEffectScript);
		//_normal_attack_effect->SetLayerIndexByName(L"Effect");

		//_normal_attack_effect->GetTransform()->SetRelativeScale(vec3(2.f, 2.f, 1.0f));
		//_normal_attack_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		//_normal_attack_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		//_normal_attack_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		//_normal_attack_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		//ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack_effect, L"Soldier Normal Attack Effect");


		//GameObject* _normal_attack_effect2 = new GameObject();


		//_normal_attack_effect2->SetName(L"Soldier Limit Attack Effect");
		//_normal_attack_effect2->AddComponent(new Transform);
		//_normal_attack_effect2->AddComponent(new MeshRender);
		//_normal_attack_effect2->AddComponent(new Animator2D);
		//_normal_attack_effect2->AddComponent(new SoldierNormalAtkEffectScript);
		//_normal_attack_effect2->SetLayerIndexByName(L"Effect");

		//_normal_attack_effect2->GetTransform()->SetRelativeScale(vec3(5.f, 5.f, 1.0f));
		//_normal_attack_effect2->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		//_normal_attack_effect2->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		//_normal_attack_effect2->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		//_normal_attack_effect2->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		//ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack_effect2, L"Soldier Limit Attack Effect");

		//GameObject* _hit_effect = new GameObject;
		//_hit_effect->AddComponent(new Transform);
		//_hit_effect->AddComponent(new ParticleSystem);
		//_hit_effect->GetParticleSystem()->SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Soldier\\Mesh\\FX_PC0001_TwistLine_02_M.mesh"));

		//_hit_effect->AddComponent(new BWeaponHitEffectScript);
		//_hit_effect->SetLayerIndexByName(L"Effect");

		/////*GameObject* hundred_lazer = new GameObject();
		////hundred_lazer->AddComponent(new Transform);
		////hundred_lazer->AddComponent(new Collider3D);
		////hundred_lazer->AddComponent(new ParticleSystem);
		////hundred_lazer->AddComponent(new HomingLaserScript);
		////hundred_lazer->SetName(L"Hundred Homing Laser");
		////hundred_lazer->SetLayerIndexByName(L"EnemyProjectile");*/
		//ObjectPoolMgr::GetInst()->RegisterObject(_hit_effect, L"Soldier Normal Attack Hit Effect");
		////

		//GameObject* _muzzle_effect = new GameObject();


		//_muzzle_effect->SetName(L"Soldier Muzzle Effect");
		//_muzzle_effect->AddComponent(new Transform);
		//_muzzle_effect->AddComponent(new MeshRender);
		//_muzzle_effect->AddComponent(new Animator2D);
		//_muzzle_effect->AddComponent(new SoldierNormalAtkEffectScript);
		//_muzzle_effect->SetLayerIndexByName(L"Effect");

		//_muzzle_effect->GetTransform()->SetRelativeScale(vec3(0.5f, 0.5f, 0.5f));
		//_muzzle_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		//_muzzle_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		//_muzzle_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		//_muzzle_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(0.1f, 0.15f, 0.15f, 0.1f));
		//ObjectPoolMgr::GetInst()->RegisterObject(_muzzle_effect, L"Soldier Muzzle Effect");
	}


	void SoldierSleepState::Enter()
	{
	}

	void SoldierSleepState::Excute()
	{

	}

	void SoldierSleepState::Exit()
	{
	}

	

}