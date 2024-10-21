#include "pch.h"
#include "BarretWeaponState.h"

#include "BarretNormalAtkScript.h"
#include "BarretNormalAtkEffectScript.h"
#include "BarretUpperCutScript.h"
#include "BarretUpperCutEffectScript.h"
#include "BarretSmackDownScript.h"
#include "BarretSmackDownEffectScript.h"
#include "BarretLimitAtkScript.h"
#include "BarretLimitAtkEffectScript.h"
#include "BWeaponHitEffectScript.h"
#include "BWeaponLimitEffectScript.h"
#include "ObjectPoolMgr.h"
#include "EffectScript.h"
#include "BWeaponShotEffectScript.h"
#include "BarretMaxFuryEffectScript.h"
#include "BarretMaximumFuryScript.h"
#include <Engine\random_number.h>
#include <Engine\SoundMgr.h>
namespace ff7r
{

	void BarretNormalAtkState::Enter()
	{
		shoot_idx = 0;
	}
	void BarretNormalAtkState::Excute()
	{
		//SoundMgr::GetInst()->Play(L"test_sound", SOUND_OBJECT_TYPE::_3D, weapon->GetOwner(), 0.5f, vec2(1.0f, 30.0f));

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
				_normal_atk->GetScript<BarretNormalAtkScript>()->SetBoom(true);
				SoundMgr::GetInst()->Play(L"sound\\AtkShoot01_finish_shot.wav", SOUND_OBJECT_TYPE::_3D, weapon->GetOwner(), 0.3f, vec2(1.0f, 20.0f), 1);

			}
		}
	}
	void BarretNormalAtkState::Exit()
	{

	}

	void BarretNormalAtkState::CreateObjects()
	{
		// ResourceMgr::GetInst()->Load<Sound>(L"test_sound", L"sound\\ak47-168856.mp3");
		GameObject* _normal_attack = new GameObject();
		_normal_attack->AddComponent(new Transform);
		_normal_attack->AddComponent(new Collider3D);
		_normal_attack->AddComponent(new BarretNormalAtkScript);
		Collider3D* _collider = _normal_attack->GetCollider3D();
		_collider->SetDynamic(false);
		_collider->SetTrigger(true);
		_collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		_normal_attack->SetLayerIndexByName(L"PlayerProjectile");
		ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack, L"Barret_Normal_Atk");



		GameObject* _normal_attack_effect = new GameObject();


		_normal_attack_effect->SetName(L"Barret Normal Attack Effect");
		_normal_attack_effect->AddComponent(new Transform);
		_normal_attack_effect->AddComponent(new MeshRender);
		_normal_attack_effect->AddComponent(new Animator2D);
		_normal_attack_effect->AddComponent(new BarretNormalAtkEffectScript);
		_normal_attack_effect->SetLayerIndexByName(L"Effect");

		_normal_attack_effect->GetTransform()->SetRelativeScale(vec3(2.f, 2.f, 1.0f));
		_normal_attack_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_normal_attack_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_normal_attack_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_normal_attack_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack_effect, L"Barret Normal Attack Effect");


		GameObject* _normal_attack_effect2 = new GameObject();


		_normal_attack_effect2->SetName(L"Barret Limit Attack Effect");
		_normal_attack_effect2->AddComponent(new Transform);
		_normal_attack_effect2->AddComponent(new MeshRender);
		_normal_attack_effect2->AddComponent(new Animator2D);
		_normal_attack_effect2->AddComponent(new BarretNormalAtkEffectScript);
		_normal_attack_effect2->SetLayerIndexByName(L"Effect");

		_normal_attack_effect2->GetTransform()->SetRelativeScale(vec3(5.f, 5.f, 1.0f));
		_normal_attack_effect2->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_normal_attack_effect2->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_normal_attack_effect2->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_normal_attack_effect2->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack_effect2, L"Barret Limit Attack Effect");

		GameObject* _hit_effect = new GameObject;
		_hit_effect->AddComponent(new Transform);
		_hit_effect->AddComponent(new ParticleSystem);
		_hit_effect->GetParticleSystem()->SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Barret\\Mesh\\FX_PC0001_TwistLine_02_M.mesh"));

		_hit_effect->AddComponent(new BWeaponHitEffectScript);
		_hit_effect->SetLayerIndexByName(L"Effect");

		///*GameObject* hundred_lazer = new GameObject();
		//hundred_lazer->AddComponent(new Transform);
		//hundred_lazer->AddComponent(new Collider3D);
		//hundred_lazer->AddComponent(new ParticleSystem);
		//hundred_lazer->AddComponent(new HomingLaserScript);
		//hundred_lazer->SetName(L"Hundred Homing Laser");
		//hundred_lazer->SetLayerIndexByName(L"EnemyProjectile");*/
		ObjectPoolMgr::GetInst()->RegisterObject(_hit_effect, L"Barret Normal Attack Hit Effect");
		//

		GameObject* _muzzle_effect = new GameObject();


		_muzzle_effect->SetName(L"Barret Muzzle Effect");
		_muzzle_effect->AddComponent(new Transform);
		_muzzle_effect->AddComponent(new MeshRender);
		_muzzle_effect->AddComponent(new Animator2D);
		_muzzle_effect->AddComponent(new BarretNormalAtkEffectScript);
		_muzzle_effect->SetLayerIndexByName(L"Effect");

		_muzzle_effect->GetTransform()->SetRelativeScale(vec3(0.5f, 0.5f, 0.5f));
		_muzzle_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_muzzle_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_muzzle_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_muzzle_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(0.1f, 0.15f, 0.15f, 0.1f));
		ObjectPoolMgr::GetInst()->RegisterObject(_muzzle_effect, L"Barret Muzzle Effect");
	}


	void BarretSleepState::Enter()
	{
	}

	void BarretSleepState::Excute()
	{

	}

	void BarretSleepState::Exit()
	{
	}

	void BarretUpperCutState::Enter()
	{
		charging = false;
	}

	void BarretUpperCutState::Excute()
	{
		GameObject* _target = weapon->GetTarget();

		if (_target != nullptr)
		{
			if (!charging)
			{
				charging = true;
				_upper_cut = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret_UpperCut");
				_upper_cut->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos() );
			}
			else
			{
				charging = false;
				_upper_cut->GetCollider3D()->SetCollision(true);
			}


		}
	}

	void BarretUpperCutState::Exit()
	{
	}

	void BarretUpperCutState::CreateObjects()
	{

		GameObject* _uppercut_attack = new GameObject();
		_uppercut_attack->AddComponent(new Transform);
		_uppercut_attack->AddComponent(new Collider3D);
		_uppercut_attack->AddComponent(new BarretUpperCutScript);
		Collider3D* _collider = _uppercut_attack->GetCollider3D();
		_collider->SetDynamic(false);
		_collider->SetTrigger(true);
		_collider->SetMove(false);
		_collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		_uppercut_attack->SetLayerIndexByName(L"PlayerProjectile");
		ObjectPoolMgr::GetInst()->RegisterObject(_uppercut_attack, L"Barret_UpperCut");



		GameObject* _uppercut_attack_effect = new GameObject();


		_uppercut_attack_effect->SetName(L"Barret UpperCut Effect");
		_uppercut_attack_effect->AddComponent(new Transform);
		_uppercut_attack_effect->AddComponent(new MeshRender);
		_uppercut_attack_effect->AddComponent(new Animator2D);
		_uppercut_attack_effect->AddComponent(new BarretUpperCutEffectScript);
		_uppercut_attack_effect->SetLayerIndexByName(L"Effect");

		_uppercut_attack_effect->GetTransform()->SetRelativeScale(vec3(3.f, 3.f, 1.0f));
		_uppercut_attack_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_uppercut_attack_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_uppercut_attack_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_uppercut_attack_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		ObjectPoolMgr::GetInst()->RegisterObject(_uppercut_attack_effect, L"Barret UpperCut Effect");
	}

	void BarretSmackDownState::Enter()
	{
	}

	void BarretSmackDownState::Excute()
	{
		GameObject* _target = weapon->GetTarget();
		if (_target != nullptr)
		{
			GameObject* _smack_down = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret_SmackDown");


			_smack_down->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos());

		}
	}

	void BarretSmackDownState::Exit()
	{
	}

	void BarretSmackDownState::CreateObjects()
	{
		GameObject* _smack_down = new GameObject();
		_smack_down->AddComponent(new Transform);
		_smack_down->AddComponent(new Collider3D);
		_smack_down->AddComponent(new BarretSmackDownScript);
		Collider3D* _collider = _smack_down->GetCollider3D();
		_collider->SetDynamic(false);
		_collider->SetTrigger(true);
		_collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		_smack_down->SetLayerIndexByName(L"PlayerProjectile");
		ObjectPoolMgr::GetInst()->RegisterObject(_smack_down, L"Barret_SmackDown");



		GameObject* _smack_down_effect = new GameObject();


		_smack_down_effect->SetName(L"Barret UpperCut Effect");
		_smack_down_effect->AddComponent(new Transform);
		_smack_down_effect->AddComponent(new MeshRender);
		_smack_down_effect->AddComponent(new Animator2D);
		_smack_down_effect->AddComponent(new BarretSmackDownEffectScript);
		_smack_down_effect->SetLayerIndexByName(L"Effect");

		_smack_down_effect->GetTransform()->SetRelativeScale(vec3(5.f, 5.f, 1.0f));
		_smack_down_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_smack_down_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_smack_down_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_smack_down_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		ObjectPoolMgr::GetInst()->RegisterObject(_smack_down_effect, L"Barret SmackDown Effect");
	}

	void BarretLimitAtkState::Enter()
	{
		GameObject* _target = weapon->GetTarget();
		if (_target != nullptr)
		{
			limit_atk_obj = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret_LimitAttack");


			limit_atk_obj->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos());

			limit_atk_obj->GetScript<EffectScript>()->SetEnabled(true);
			//GameObject* explosion_effect = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Normal Attack Effect");
			//explosion_effect->GetTransform()->SetRelativePos(GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
			//velocity = vec3(0.0f, 1.0f, 0.0f);
			//GetCollider3D()->Disable();
			//time = 0.0f;

			//_other->GetOwner()->GetScript<CharacterDataScript>()->Hit(ATK_TYPE::DOWN, _dmg, GetTransform()->GetWorldPos() + GetCollider3D()->GetOffsetPos());
			//braver_slash_effect->SetActive(true);
			//braver_slash_effect->GetScript<EffectScript>()->SetEnabled(true);
		}
	}

	void BarretLimitAtkState::Excute()
	{
		BarretLimitAtkScript* _limit_atk = limit_atk_obj->GetScript<BarretLimitAtkScript>();
		if (_limit_atk != nullptr)
		{
			GameObject* _target = weapon->GetTarget();

			vec3 _obj_pos = limit_atk_obj->GetTransform()->GetWorldPos();
			_obj_pos += limit_atk_obj->GetCollider3D()->GetOffsetPos();
			vec3 _target_pos = _target->GetTransform()->GetWorldPos();
			_target_pos += _target->GetCollider3D()->GetOffsetPos();

			vec3 _limit_vel = _target_pos - _obj_pos;
			_limit_vel *= 2.0f;

			_limit_atk->SetVelocity(_limit_vel);
		}
	}

	void BarretLimitAtkState::Exit()
	{
	}

	void BarretLimitAtkState::CreateObjects()
	{
		//GameObject* _limit_attack = new GameObject();
		//
		//_limit_attack->AddComponent(new Transform);
		//_limit_attack->AddComponent(new MeshRender);
		//_limit_attack->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"DebugSphereMesh"));
		//_limit_attack->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugMtrl"));
		////_normal_attack->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		//_limit_attack->AddComponent(new Collider3D);
		//_limit_attack->AddComponent(new BarretLimitAtkScript);
		//Collider3D* _collider = _limit_attack->GetCollider3D();
		//_collider->SetDynamic(false);
		//_collider->SetTrigger(true);
		//_collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		//_limit_attack->SetLayerIndexByName(L"PlayerProjectile");
		//ObjectPoolMgr::GetInst()->RegisterObject(_limit_attack, L"Barret_LimitAttack");





		GameObject* _limit_attack_effect = new GameObject();


		_limit_attack_effect->SetName(L"Barret LimitAttack Effect");
		_limit_attack_effect->AddComponent(new Transform);
		_limit_attack_effect->AddComponent(new MeshRender);
		_limit_attack_effect->AddComponent(new Animator2D);
		_limit_attack_effect->AddComponent(new BarretLimitAtkEffectScript);
		_limit_attack_effect->SetLayerIndexByName(L"Effect");

		_limit_attack_effect->GetTransform()->SetRelativeScale(vec3(3.f, 3.f, 1.0f));
		_limit_attack_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_limit_attack_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_limit_attack_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_limit_attack_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));


		ObjectPoolMgr::GetInst()->RegisterObject(_limit_attack_effect, L"Barret LimitAttack Effect");
		GameObject* _limit_effect = new GameObject();


		_limit_effect->SetName(L"Barret LimitAttack Effect");
		_limit_effect->AddComponent(new Transform);
		//_limit_effect->GetParticleSystem()->SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Barret\\Mesh\\FX_PC0001_TwistLine_02_M.mesh"));
		_limit_effect->AddComponent(new ParticleSystem);
		_limit_effect->AddComponent(new BWeaponLimitEffectScript);
		_limit_effect->SetLayerIndexByName(L"Effect");

		_limit_effect->GetTransform()->SetRelativeScale(vec3(3.f, 3.f, 1.0f));
		_limit_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		//_limit_attack_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		//_limit_attack_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		//_limit_attack_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		ObjectPoolMgr::GetInst()->RegisterObject(_limit_effect, L"Barret Limit Effect");

		//cpaut = new GameObject;
		//cpaut->AddComponent(new Transform);
		//cpaut->SetName(L"particletest");
		//cpaut->AddComponent(new ParticleSystem);
		//cpaut->AddComponent(new BWeaponHitEffectScript);
		//cpaut->AddComponent(new Collider3D);
		////cpaut->GetParticleSystem()->SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Barret\\Mesh\\FX_PC0001_TwistLine_02_M.mesh"));

		//SpawnGameObject(cpaut, vec3(5.f, 5.f, 0.f), L"Default");

		Ptr<MeshData> _meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Sphere_01_M.gltf");
		if (_meshdata != nullptr)
		{
			GameObject* barret_limit_ball;
			barret_limit_ball = _meshdata->Instantiate();
			barret_limit_ball->SetName(L"Barret_LimitAttack");
			barret_limit_ball->GetTransform()->SetRelativeScale(3.f, 3.f, 3.f);
			barret_limit_ball->GetTransform()->SetRelativePos(0, 1.5f, 0);
			barret_limit_ball->GetTransform()->SetRelativeRot(0, Degree2Rad(-81.f), 0);
			barret_limit_ball->SetLayerIndex(4);
			barret_limit_ball->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
			barret_limit_ball->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(1.0f, 0.2f, 0.1f, 1.0f));
			barret_limit_ball->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0
				, ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Fire_UV_03_T.png"));

			barret_limit_ball->AddComponent(new EffectScript);
			barret_limit_ball->GetScript<EffectScript>()->SetIntensity(10.0f);
			/*barret_limit_ball->GetScript<EffectScript>()->SetIncreaseSpeed(3.5f);
			barret_limit_ball->GetScript<EffectScript>()->SetMaxUVLength(1.0f);*/
			//barret_limit_ball->GetScript<EffectScript>()->IsChangeRange(false, true);

			barret_limit_ball->GetScript<EffectScript>()->SetEnabled(false);
			barret_limit_ball->GetScript<EffectScript>()->SetChange(true);
			barret_limit_ball->SetActive(false);
			barret_limit_ball->AddComponent(new Collider3D);
			barret_limit_ball->AddComponent(new BarretLimitAtkScript);
			ObjectPoolMgr::GetInst()->RegisterObject(barret_limit_ball, L"Barret_LimitAttack");
		}
	}

	void BarretMaximumFuryState::Enter()
	{
		shoot_idx = 0;
	}

	void BarretMaximumFuryState::Excute()
	{
		
		GameObject* _target = weapon->GetTarget();
		if (_target != nullptr)
		{
			if (shoot_idx == 0)
			{
				sound_idx = SoundMgr::GetInst()->Play(L"sound\\bulst4_shot.wav", SOUND_OBJECT_TYPE::_3D, weapon->GetOwner(), 0.3f, vec2(1.0f, 20.0f), 0, false);
				flame_particle = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Maximum Shot Effect");
				weapon->GetTransform()->CalcWorldDir();
				flame_particle->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos());
				flame_particle->GetScript< BWeaponShotEffectScript>()->SetDir(weapon->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT));
			}
			shoot_idx++;
			GameObject* _muzzle = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret Muzzle Effect");
			_muzzle->GetTransform()->SetRelativePos(weapon->CalcMuzzlePos());
			

			GameObject* maximum = ObjectPoolMgr::GetInst()->GetGameObject(L"Barret_Maximum_Fury");

			vec3 _weapon_pos = weapon->GetTransform()->GetWorldPos();
			vec3 _target_pos = _target->GetTransform()->GetWorldPos();
			vec3 _target_scale = _target->GetCollider3D()->GetOffsetScale();
			_target_pos.y += _target_scale.y * 0.5f;

			vec3 _target_vec = _target_pos - _weapon_pos;
			vec3 _random_vec = vec3(randf(1.0f), randf(1.0f), randf(1.0f)) * 0.3f;
			maximum->GetTransform()->SetRelativePos(_weapon_pos + _target_vec.Normalize() * (_target_vec.Length() - _target_scale.x * 0.5f) + _random_vec);
			if (shoot_idx == 30)
			{
				flame_particle->SetActive(false);
				SoundMgr::GetInst()->Stop(sound_idx, weapon->GetOwner());
			}
		}
	}

	void BarretMaximumFuryState::Exit()
	{
	}

	void BarretMaximumFuryState::CreateObjects()
	{
		GameObject* _normal_attack = new GameObject();




		_normal_attack->AddComponent(new Transform);
		_normal_attack->AddComponent(new Collider3D);
		_normal_attack->AddComponent(new BarretMaximumFuryScript);
		Collider3D* _collider = _normal_attack->GetCollider3D();
		_collider->SetDynamic(false);
		_collider->SetTrigger(true);
		_collider->SetMove(false);
		_collider->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		_normal_attack->SetLayerIndexByName(L"PlayerProjectile");
		ObjectPoolMgr::GetInst()->RegisterObject(_normal_attack, L"Barret_Maximum_Fury");



		


		GameObject* _maximum_shot_effect = new GameObject;
		_maximum_shot_effect->AddComponent(new Transform);
		_maximum_shot_effect->AddComponent(new ParticleSystem);
		_maximum_shot_effect->AddComponent(new BWeaponShotEffectScript);
		_maximum_shot_effect->SetLayerIndexByName(L"Effect");
		_maximum_shot_effect->SetName(L"Barret Maximum Shot Effect");
		///*GameObject* hundred_lazer = new GameObject();
		//hundred_lazer->AddComponent(new Transform);
		//hundred_lazer->AddComponent(new Collider3D);
		//hundred_lazer->AddComponent(new ParticleSystem);
		//hundred_lazer->AddComponent(new HomingLaserScript);
		//hundred_lazer->SetName(L"Hundred Homing Laser");
		//hundred_lazer->SetLayerIndexByName(L"EnemyProjectile");*/
		ObjectPoolMgr::GetInst()->RegisterObject(_maximum_shot_effect, L"Barret Maximum Shot Effect");


		GameObject* _maximum_fury_effect = new GameObject();


		_maximum_fury_effect->SetName(L"Barret Maximum Fury Effect");
		_maximum_fury_effect->AddComponent(new Transform);
		_maximum_fury_effect->AddComponent(new MeshRender);
		_maximum_fury_effect->AddComponent(new Animator2D);
		_maximum_fury_effect->AddComponent(new BarretMaxFuryEffectScript);
		_maximum_fury_effect->SetLayerIndexByName(L"Effect");

		_maximum_fury_effect->GetTransform()->SetRelativeScale(vec3(2.f, 2.f, 1.0f));
		_maximum_fury_effect->GetTransform()->SetRelativeRot(vec3(0.f, 0.0f, 0.0f));
		_maximum_fury_effect->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_maximum_fury_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		_maximum_fury_effect->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(0.9f, 0.5f, 0.2f, 1.0f));
		ObjectPoolMgr::GetInst()->RegisterObject(_maximum_fury_effect, L"Barret Maximum Fury Effect");
	}

}