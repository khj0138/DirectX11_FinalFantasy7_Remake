#include "pch.h"
#include "TestLevel.h"

#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>

#include <Engine\ResourceMgr.h>
#include <Engine\CollisionMgr.h>
#include <Engine\PhysXMgr.h>

#include <Script\GameMgr.h>
#include <Script\CloudMoveScript.h>
#include <Script\FindEnemyScript.h>
#include <Script\CharacterDataScript.h>
#include <Script\CloudWeaponScript.h>
#include <Script\BarretMotionScript.h>
#include <Script\BarretWeaponScript.h>
#include <Script\AtkRangeScript.h>
#include <Script\SoldierScript.h>
#include <Script\LensEffectScript.h>
#include <Script\MagicFireScript.h>
#include <Script\MagicFireTrailScript.h>
#include <Script\MagicFireExplosionScript.h>
#include <Script\HundredBulletScript.h>
#include <Script\HomingLaserScript.h>
#include <Script\BladeScript.h>
#include <Script\BladeBurstScript.h>
#include <Script\ElectricEffectScript.h>
#include <Script\BreakEffectScript.h>
#include <Script\BWeaponHitEffectScript.h>
#include <Script\CWeaponHitEffectScript.h>
#include <Script\HealEffectScript.h>
#include <Script\BeamScript.h>
#include <Script\RockScript.h>
#include <Script\ChangeLevelScript.h>

#include "LevelSaveLoad.h"


// ========== Test Include Header =================
#include <Engine/GLTFLoader.h>
#include <Engine/Socket.h>
#include <Engine/ReflectionCapture.h>

#include <Engine/TrailRenderer.h>
#include <Engine/AnimatorController.h>
#include <Engine/RenderMgr.h>

#include <Script/PlayerCameraScript.h>
#include <Script/EffectScript.h>
#include <Script/BackWeaponScript.h>
#include <Script/HundredGunnerScript.h>
#include <Script/HundredGunnerRightArmScript.h>
#include <Script/SaturationFireScript.h>
#include <Script/ObjectPoolMgr.h>
#include <Script/SoldierWeaponScript.h>

#include <Engine\ThreadPool.h>
//#include <PhysX/PxPhysicsAPI.h>

//#include <Engine/PhysXMgr.h
namespace ff7r
{
	void SpawnEnemy();
	void CreatePoolObjects();
	GameObject* pMainCam;
	void CreateTestLevel()
	{
		Level* pCurLevel = LevelMgr::GetInst()->GetLevel(0);
		pCurLevel->SetName(L"Stage1 ");

		// Main Camera Object 생성
		pMainCam = new GameObject;
		pMainCam->SetName(L"MainCamera");

		pMainCam->AddComponent(new Transform);
		pMainCam->AddComponent(new Camera);
		pMainCam->AddComponent(new PlayerCameraScript);

		pMainCam->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pMainCam->GetCamera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pMainCam->GetCamera()->SetCameraIndex(0);		// MainCamera 로 설정
		pMainCam->GetCamera()->SetLayerMaskAll(true);	// 모든 레이어 체크
		pMainCam->GetCamera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.
		pMainCam->GetCamera()->SetPostProcess(Camera::CameraPostProcess::BLOOM, true);
		pMainCam->GetCamera()->SetPostProcess(Camera::CameraPostProcess::TONEMAPPING, true);
		pMainCam->GetCamera()->SetPostProcess(Camera::CameraPostProcess::FXAA, true);

		SpawnGameObject(pMainCam, vec3(0.f, 0.f, 0.f), 0);
		pMainCam->SetDestroy(false);
		// UI cameara
		GameObject* pUICam = new GameObject;
		pUICam->SetName(L"UICamera");

		pUICam->AddComponent(new Transform);
		pUICam->AddComponent(new Camera);

		pUICam->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICam->GetCamera()->SetCameraIndex(1);		// Sub 카메라로 지정
		pUICam->GetCamera()->SetLayerMaskAll(false);	// 모든 레이어 체크
		//pUICam->GetCamera()->SetFOV(Degree2Rad(180.f));
		pUICam->GetCamera()->SetLayerMask(31, true);	// 31번 레이어만 체크
		pUICam->GetCamera()->SetPostProcess(Camera::CameraPostProcess::FXAA, true);
		SpawnGameObject(pUICam, vec3(0.f, 0.f, 0.f), 0);
		pUICam->SetDestroy(false);


		GameObject* obj_pool = new GameObject;
		obj_pool->SetName(L"Object Pool");
		obj_pool->AddComponent(new Transform);
		obj_pool->AddComponent(new ObjectPoolMgr);
		//obj_pool->Awake();
		SpawnGameObject(obj_pool, vec3(0.f, 0.f, 0.f), L"Manager");
		obj_pool->SetDestroy(false);
		CreatePoolObjects();

		GameObject* teleport = new GameObject;
		teleport->SetName(L"Teleport");
		teleport->AddComponent(new Transform);
		teleport->AddComponent(new Collider3D);
		teleport->AddComponent(new ChangeLevelScript);

		SpawnGameObject(teleport, vec3(273.f, 10.f, -105.f), L"InteractObject");

		GameObject* pSkyBox = new GameObject;
		pSkyBox->SetName(L"SkyBox");

		pSkyBox->AddComponent(new Transform);
		pSkyBox->AddComponent(new SkyBox);

		pSkyBox->GetTransform()->SetRelativeScale(vec3(100.f, 100.f, 100));
		pSkyBox->GetSkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
		pSkyBox->GetSkyBox()->SetSkyBoxTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\T_Backcloth_Environment_Inside_0Highway_SE.hdr"));

		SpawnGameObject(pSkyBox, vec3(0.f, 0.f, 0.f), L"Skybox");


		GameObject* enemy = nullptr;
		Ptr<MeshData> _meshdata_t = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Enemy\\EN0000_00_SecuritySoldier_Standard\\Model\\EN0000_00.gltf");
		if (_meshdata_t != nullptr)
		{
			enemy = _meshdata_t->Instantiate();
			enemy->SetDestroy(false);
			enemy->SetName(L"Soldier A");
			enemy->GetTransform()->SetRelativeScale(1, 1, 1);
			enemy->AddComponent(new Collider3D);
			enemy->AddComponent(new SoldierScript);
		
			SpawnGameObject(enemy, vec3(132.f, 3.6f, 3.f), L"Enemy");
			enemy->GetAnimator3D()->SetBoneMotion(true);
		
			wstring _path = L"User Resource\\GameContents\\Character\\Enemy\\EN0000_00_SecuritySoldier_Standard\\AnimatorController\\";
			_path += L"soldier_animator_controller";
			_path += L".animctrl";
		
			Ptr<AnimatorController> _controller = ResourceMgr::GetInst()->FindRes<AnimatorController>(_path);
			if (_controller == nullptr)
			{
				_controller = ResourceMgr::GetInst()->CreateAnimatorController(_path);
			}
		
			enemy->GetAnimator3D()->SetAnimatorController(_controller);
			enemy->GetScript<SoldierScript>()->RegisterDeadEvent([]() {SpawnEnemy(); });
			GameObject* enemy_socket = new GameObject;
			enemy_socket->AddComponent(new Transform);
			enemy_socket->AddComponent(new Socket);
			enemy->AddChild(enemy_socket);
			enemy_socket->GetSocket()->SetSocket(L"R_Weapon_a");
			enemy_socket->SetName(L"Weapon Socket");
		}
		// Soldier Weapon
		_meshdata_t = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Weapon\\WE7000_00_SecuritySoldier_Gun\\Model\\WE7000_00.gltf");
		if (_meshdata_t != nullptr)
		{
			GameObject* _enemy_gun;
			_enemy_gun = _meshdata_t->Instantiate();
			_enemy_gun->SetName(L"Soldier Weapon");
			_enemy_gun->SetLayerIndexByName(L"Weapon");
			_enemy_gun->AddComponent(new SoldierWeaponScript);
			_enemy_gun->GetTransform()->SetRelativeScale(1, 1, 1);
			enemy->GetScript<SoldierScript>()->SetWeapon(_enemy_gun->GetScript<SoldierWeaponScript>());

			_enemy_gun->GetTransform()->SetRelativeRot(vec3(-90.f * 3.141592f / 180.f, 0.0f, 90.f * 3.141592f / 180.f));
		}
		// Cloud
		GameObject* _player = nullptr;
		Ptr<MeshData>_meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Player\\PC0000_00_Cloud_Standard\\Model\\PC0000_00.gltf");
		if (_meshdata != nullptr)
		{
			_player = _meshdata->Instantiate();
			_player->SetDestroy(false);
			_player->SetName(L"Cloud");
			_player->GetTransform()->SetRelativeScale(1, 1, 1);


			_player->AddComponent(new Collider3D);

			//SpawnGameObject(_player, vec3(0.f, -22.f, 68.f), L"Default");
			SpawnGameObject(_player, vec3(0.f, 0.0f, 0.f), L"Default");

			pMainCam->GetScript<PlayerCameraScript>()->SetPlayer(_player);
			//_object->AddComponent(new Collider3D);
			_player->GetAnimator3D()->SetBoneMotion(true);
			_player->AddComponent(new CloudMoveScript);
			_player->AddComponent(new FindEnemyScript);
			_player->AddComponent(new CharacterDataScript);
			//_object->AddChild(pMainCam);
			_player->GetScript<CloudMoveScript>()->SetPlayerCamera(pMainCam);
			wstring _path = L"User Resource\\GameContents\\Character\\Player\\PC0000_00_Cloud_Standard\\AnimatorController\\";
			_path += L"cloud_animator_controller";
			_path += L".animctrl";

			Ptr<AnimatorController> _controller = ResourceMgr::GetInst()->FindRes<AnimatorController>(_path);
			if (_controller == nullptr)
			{
				_controller = ResourceMgr::GetInst()->CreateAnimatorController(_path);
			}

			_player->GetAnimator3D()->SetAnimatorController(_controller);
			GameMgr::GetInst()->RegisterPlayer(_player);

			GameObject* _atk_trigger = new GameObject;
			_atk_trigger->AddComponent(new Transform);
			_atk_trigger->AddComponent(new Collider3D);
			_atk_trigger->AddComponent(new AtkRangeScript);
			_atk_trigger->GetScript<AtkRangeScript>()->SetPlayer(_player);
			_atk_trigger->SetLayerIndexByName(L"Player Range");
			_player->AddChild(_atk_trigger);

			_atk_trigger->SetName(L"Attack Range Trigger");


			GameObject* _object2 = new GameObject;
			_object2->AddComponent(new Transform);
			_object2->AddComponent(new Socket);
			_player->AddChild(_object2);
			_object2->GetSocket()->SetSocket(L"R_Weapon_a");
			_object2->SetName(L"Weapon Socket");

			GameObject* _object3 = new GameObject;
			_object3->AddComponent(new Transform);
			_object3->AddComponent(new Socket);
			_player->AddChild(_object3);
			_object3->GetSocket()->SetSocket(L"L_Weapon_a");
			_object3->SetName(L"Magic Socket");
		}

		// Cloud Weapon
		_meshdata = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Weapon\\WE0000_02_Cloud_HardBreaker\\Model\\WE0000_02.gltf");
		if (_meshdata != nullptr)
		{
			GameObject* _object;
			_object = _meshdata->Instantiate();
			_object->SetName(L"Cloud Weapon");
			_object->SetLayerIndexByName(L"Weapon");
			_object->AddComponent(new CloudWeaponScript);
			_object->GetTransform()->SetRelativeScale(1, 1, 1);
			_player->GetScript<CloudMoveScript>()->SetWeapon(_object->GetScript<CloudWeaponScript>());
			//SpawnGameObject(_object, vec3(0.f, 0.f, 0.f), L"Weapon");

			GameObject* _object2 = new GameObject;
			_object2->AddComponent(new Transform);
			_object2->AddComponent(new Socket);
			_object->GetTransform()->SetRelativeRot(vec3(90.f * 3.141592f / 180.f, 0.0f, 90.f * 3.141592f / 180.f));

			_object->AddChild(_object2);
			_object2->GetSocket()->SetSocket(L"C_TrailA_a");
			_object2->SetName(L"Trail Socket A");

			GameObject* _object3 = new GameObject;
			_object3->AddComponent(new Transform);
			_object3->AddComponent(new Socket);
			_object->AddChild(_object3);
			_object3->GetSocket()->SetSocket(L"C_TrailA_b");
			_object3->SetName(L"Trail Socket B");

			GameObject* _trail = new GameObject;
			_trail->AddComponent(new Transform);
			_trail->AddComponent(new TrailRenderer);
			_object->AddChild(_trail);
			_trail->GetTrailRenderer()->SetTrailSocket(_object2, _object3);
			_trail->GetTrailRenderer()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DistoritionMtrl"));
			_trail->GetTrailRenderer()->SetTrailColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
			_trail->GetTrailRenderer()->GetMaterial()->SetTexParam(TEX_0, RenderMgr::GetInst()->GetBlurRenderTarget());

			_object->AddComponent(new Collider3D);

		}


		GameObject* _player2 = nullptr;
		Ptr<MeshData>_meshdata2 = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Player\\PC0001_00_Barret_Standard\\Model\\PC0001_00.gltf");
		if (_meshdata2 != nullptr)
		{
			_player = _meshdata2->Instantiate();
			_player->SetDestroy(false);
			_player->SetName(L"Barret");
			_player->GetTransform()->SetRelativeScale(1, 1, 1);
		
		
			_player->AddComponent(new Collider3D);
			Collider3D* _col = _player->GetCollider3D();
			_col->SetDynamic(true);
			_col->SetTrigger(false);
			_col->SetOffsetPos(vec3(0, 0.5, 0));
			
		
			SpawnGameObject(_player, vec3(0.f, 0.f, 0.f), L"Default");
		
			//pMainCam->GetScript<PlayerCameraScript>()->SetPlayer(_player);
			//_object->AddComponent(new Collider3D);
			_player->GetAnimator3D()->SetBoneMotion(true);
			_player->AddComponent(new BarretMotionScript);
			_player->AddComponent(new FindEnemyScript);
			_player->GetScript<FindEnemyScript>()->SetMaxDistance(20.0f);
			_player->AddComponent(new CharacterDataScript);
			//_object->AddChild(pMainCam);
			_player->GetScript<BarretMotionScript>()->SetPlayerCamera(pMainCam);
			wstring _path = L"User Resource\\GameContents\\Character\\Player\\PC0001_00_Barret_Standard\\AnimatorController\\";
			_path += L"barret_animator_controller";
			_path += L".animctrl";
		
			Ptr<AnimatorController> _controller = ResourceMgr::GetInst()->FindRes<AnimatorController>(_path);
			if (_controller == nullptr)
			{
				_controller = ResourceMgr::GetInst()->CreateAnimatorController(_path);
			}
		
			_player->GetAnimator3D()->SetAnimatorController(_controller);
			GameMgr::GetInst()->RegisterPlayer(_player);
		
			GameObject* _atk_trigger = new GameObject;
			_atk_trigger->AddComponent(new Transform);
			_atk_trigger->AddComponent(new Collider3D);
			_atk_trigger->AddComponent(new AtkRangeScript);
			_atk_trigger->GetScript<AtkRangeScript>()->SetPlayer(_player);
			_atk_trigger->SetLayerIndexByName(L"Player Range");
			_player->AddChild(_atk_trigger);
		
			_atk_trigger->SetName(L"Attack Range Trigger");
		
		
			GameObject* _object2 = new GameObject;
			_object2->AddComponent(new Transform);
			_object2->AddComponent(new Socket);
			_player->AddChild(_object2);
			_object2->GetSocket()->SetSocket(L"R_Forearm_a");
			_object2->SetName(L"Weapon Socket");
			//SpawnGameObject(_object2, vec3(0.f, 0.f, 0.f), L"Default");
		}
		
		// Barret Weapon
		ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Barret\\Model\\FX_PC0001_TwistLine_02_M.gltf");
		_meshdata2 = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Weapon\\WE0001_03_Barret_LargeMouth\\Model\\WE0001_03.gltf");
		if (_meshdata2 != nullptr)
		{
			GameObject* _object;
			_object = _meshdata2->Instantiate();
			_object->SetName(L"Barret Weapon");
			_object->SetLayerIndexByName(L"Weapon");
			_object->AddComponent(new BarretWeaponScript);
			_object->GetTransform()->SetRelativeScale(1, 1, 1);
			_object->GetTransform()->SetRelativeRot(vec3(-90.f * 3.141592f / 180.f, 0.0f, 90.f * 3.141592f / 180.f));
			_player->GetScript<BarretMotionScript>()->SetWeapon(_object->GetScript<BarretWeaponScript>());
		
			_object->AddComponent(new Collider3D);
		
		}

	}

	vector<wstring>     m_vecResPath;
	std::mutex a;
	GameObject* t;
	void Create(wstring strExt, wstring strFile, int i)
	{
		if (strExt == L".gltf")
		{
			Ptr<MeshData> _meshdata = ResourceMgr::GetInst()->LoadGLTF(m_vecResPath[i], L"Stage2\\");
			if (_meshdata != nullptr)
			{
				GameObject* _object;
				_object = _meshdata->Instantiate();
				_object->SetName(strFile);
				_object->GetTransform()->SetRelativeScale(1, 1, 1);
				_object->SetLayerIndexByName(L"Tile");
				std::unique_lock<std::mutex> lock(a);
				t->AddChild(_object);

				//_main_obj->AddChild(_object);
			}
		}
	}
	
	void FindFileName(const wstring& _FolderPath)
	{
		WIN32_FIND_DATA FindData = {};

		wstring FolderPath = _FolderPath + L"*.*";

		HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

		while (FindNextFile(hFindHandle, &FindData))
		{
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!wcscmp(FindData.cFileName, L".."))
				{
					continue;
				}

				FindFileName(_FolderPath + FindData.cFileName + L"\\");
				continue;
			}

			wstring strContentPath = PathMgr::GetInst()->GetContentPath();
			wstring RelativePath = _FolderPath + FindData.cFileName;
			RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

			m_vecResPath.push_back(RelativePath);
		}

		FindClose(hFindHandle);
	}

	void CreatePoolObjects()
	{
		GameObject* fire_trail = new GameObject;
		fire_trail->SetName(L"Magic Fire Effect");
		fire_trail->AddComponent(new Transform);
		fire_trail->AddComponent(new MeshRender);
		fire_trail->AddComponent(new Animator2D);
		fire_trail->AddComponent(new MagicFireTrailScript);
		fire_trail->GetTransform()->SetRelativeScale(vec3(2.f, 2.f, 1.0f));
		fire_trail->GetTransform()->SetRelativeRot(vec3(0.f, 0.f, 0.0f));
		fire_trail->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		fire_trail->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		fire_trail->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 1.5f, 0.3f, 1.0f));
		fire_trail->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(fire_trail, L"Magic Fire Trail Effect");

		GameObject* fire_explosion = new GameObject;
		fire_explosion->SetName(L"Magic Fire Explosion Effect");
		fire_explosion->AddComponent(new Transform);
		fire_explosion->AddComponent(new MeshRender);
		fire_explosion->AddComponent(new Animator2D);
		fire_explosion->AddComponent(new MagicFireExplosionScript);
		fire_explosion->GetTransform()->SetRelativeScale(vec3(3.f, 3.f, 1.0f));
		fire_explosion->GetTransform()->SetRelativeRot(vec3(0.f, 0.f, 0.0f));
		fire_explosion->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		fire_explosion->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"BillboardMtrl"));
		fire_explosion->GetMeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, vec4(3.0f, 0.8f, 0.3f, 1.0f));
		fire_explosion->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(fire_explosion, L"Magic Fire Explosion Effect");

		GameObject* magice_fire = new GameObject;
		magice_fire->SetName(L"Magic Fire");
		magice_fire->AddComponent(new Transform);
		magice_fire->AddComponent(new Collider3D);
		magice_fire->AddComponent(new MagicFireScript);
		magice_fire->SetLayerIndexByName(L"PlayerProjectile");

		ObjectPoolMgr::GetInst()->RegisterObject(magice_fire, L"Magic Fire");

		GameObject* hundred_bullet = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_BulletGun_02_M.gltf")->Instantiate();
		hundred_bullet->AddComponent(new HundredBulletScript);
		hundred_bullet->AddComponent(new Collider3D);
		hundred_bullet->SetName(L"Hundred Bullet");
		hundred_bullet->GetTransform()->SetRelativeScale(3, 3, 3);
		hundred_bullet->SetLayerIndexByName(L"EnemyProjectile");

		ObjectPoolMgr::GetInst()->RegisterObject(hundred_bullet, L"Hundred Bullet");


		GameObject* hundred_lazer = new GameObject();
		hundred_lazer->AddComponent(new Transform);
		hundred_lazer->AddComponent(new Collider3D);
		hundred_lazer->AddComponent(new ParticleSystem);
		hundred_lazer->AddComponent(new HomingLaserScript);
		hundred_lazer->SetName(L"Hundred Homing Laser");
		hundred_lazer->SetLayerIndexByName(L"EnemyProjectile");

		ObjectPoolMgr::GetInst()->RegisterObject(hundred_lazer, L"Hundred Homing Laser");


		GameObject* blade = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Cylinder_01_M.gltf")->Instantiate();
		blade->SetName(L"Blade");
		blade->AddComponent(new Collider3D);
		blade->AddComponent(new BladeScript);
		blade->AddComponent(new EffectScript);
		blade->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(blade, L"Blade");

		GameObject* blade_burst = new GameObject;
		blade_burst->SetName(L"Blade Burst");
		blade_burst->AddComponent(new Transform);
		blade_burst->AddComponent(new Decal);
		blade_burst->AddComponent(new BladeBurstScript);
		blade_burst->SetLayerIndexByName(L"PlayerProjectile");

		ObjectPoolMgr::GetInst()->RegisterObject(blade_burst, L"Blade Burst");


		GameObject* electric_effect = new GameObject;
		electric_effect->AddComponent(new Transform);
		electric_effect->AddComponent(new Decal);
		electric_effect->AddComponent(new EffectScript);
		electric_effect->AddComponent(new ElectricEffectScript);
		electric_effect->SetName(L"Electric Effect");
		electric_effect->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(electric_effect, L"Electric Effect");

		GameObject* break_effect = new GameObject;
		break_effect->AddComponent(new Transform);
		break_effect->AddComponent(new BreakEffectScript);
		break_effect->SetName(L"Break Effect");
		break_effect->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(break_effect, L"Break Effect");


		GameObject* c_hit_effect = new GameObject;
		c_hit_effect->AddComponent(new Transform);
		c_hit_effect->AddComponent(new ParticleSystem);
		c_hit_effect->AddComponent(new CWeaponHitEffectScript);
		c_hit_effect->SetName(L"CWeapon Hit Effect");
		c_hit_effect->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(c_hit_effect, L"CWeapon Hit Effect");



		GameObject* heal_effect = ResourceMgr::GetInst()->FindRes<MeshData>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0003_Aerith\\MeshData\\FX_PC0003_Wind_01_M.mdat")->Instantiate();
		heal_effect->SetName(L"Heal Effect");
		heal_effect->AddComponent(new EffectScript);
		heal_effect->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		heal_effect->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Wave_UV_01_T.png"));
		heal_effect->GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, vec4(32.f / 255.f, 245.f / 255.f, 55.f / 255.f, 1.0f));
		heal_effect->GetScript<EffectScript>()->SetIntensity(5.0f);
		heal_effect->GetScript<EffectScript>()->SetMaxUVLength(0.3f);
		heal_effect->GetScript<EffectScript>()->SetIncreaseSpeed(0.5f);
		heal_effect->GetScript<EffectScript>()->IsChangeRange(true, false);
		heal_effect->GetScript<EffectScript>()->SetDisableEffect(2.0f);

		heal_effect->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(heal_effect, L"Heal Effect");



		GameObject* heal_particle_effect = new GameObject;
		heal_particle_effect->AddComponent(new Transform);
		heal_particle_effect->AddComponent(new ParticleSystem);
		heal_particle_effect->AddComponent(new HealEffectScript);
		heal_particle_effect->SetName(L"Heal Particel Effect");
		heal_particle_effect->SetLayerIndexByName(L"Effect");

		ObjectPoolMgr::GetInst()->RegisterObject(heal_particle_effect, L"Heal Particel Effect");



		GameObject* beam = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Character\\Enemy\\EB0018_HundredGunner\\Model\\FX_EB0018_Beam_01_M.gltf")->Instantiate();
		beam->AddComponent(new EffectScript);
		beam->AddComponent(new BeamScript);
		beam->SetName(L"Beam");
		beam->GetTransform()->SetRelativeScale(6, 0.2, 0.2);
		beam->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Effect3DMtrl"));
		beam->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"FF7R Resource\\GameContents\\Effect\\Resident\\Texture\\FX_R_Grd_03_T.png"));
		beam->GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, vec4(129.f / 255.f, 245.f / 255.f, 196.f / 255.f, 1.0f));
		beam->GetScript<EffectScript>()->SetIntensity(2.0f);

		ObjectPoolMgr::GetInst()->RegisterObject(beam, L"Beam");

		GameObject* rock = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Rock_01_M.gltf")->Instantiate();
		rock->AddComponent(new Collider3D);
		rock->AddComponent(new RockScript);
		rock->GetTransform()->SetRelativeScale(3.f, 3.f, 3.f);
		rock->SetLayerIndex(12);
		rock->SetName(L"Rock 1");
		rock->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\T_F59BigPillarMarble_01_0ShinBuilding_C.png"));

		ObjectPoolMgr::GetInst()->RegisterObject(rock, L"Rock 1", 30);

		rock = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Rock_02_M.gltf")->Instantiate();
		rock->AddComponent(new Collider3D);
		rock->AddComponent(new RockScript);
		rock->GetTransform()->SetRelativeScale(3.f, 3.f, 3.f);
		rock->SetName(L"Rock 2");
		rock->SetLayerIndex(12);
		rock->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\T_F59BigPillarMarble_01_0ShinBuilding_C.png"));

		ObjectPoolMgr::GetInst()->RegisterObject(rock, L"Rock 2", 30);

		rock = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Effect\\Resident\\Model\\FX_R_Rock_03_M.gltf")->Instantiate();
		rock->AddComponent(new Collider3D);
		rock->AddComponent(new RockScript);
		rock->GetTransform()->SetRelativeScale(3.f,3.f, 3.f);
		rock->SetLayerIndex(12);
		rock->SetName(L"Rock 3");
		rock->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0,
			ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\T_F59BigPillarMarble_01_0ShinBuilding_C.png"));

		ObjectPoolMgr::GetInst()->RegisterObject(rock, L"Rock 3", 30);
	}
	void SpawnEnemy()
	{
		GameObject* enemy = nullptr;
		Ptr<MeshData> _meshdata_t = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Enemy\\EN0000_00_SecuritySoldier_Standard\\Model\\EN0000_00.gltf");
		if (_meshdata_t != nullptr)
		{
			enemy = _meshdata_t->Instantiate();
			enemy->SetDestroy(false);
			enemy->SetName(L"Soldier B");
			enemy->GetTransform()->SetRelativeScale(1, 1, 1);
			enemy->AddComponent(new Collider3D);
			enemy->AddComponent(new SoldierScript);

			SpawnGameObject(enemy, vec3(210.f, 3.6f, 3.f), L"Enemy");
			enemy->GetAnimator3D()->SetBoneMotion(true);

			wstring _path = L"User Resource\\GameContents\\Character\\Enemy\\EN0000_00_SecuritySoldier_Standard\\AnimatorController\\";
			_path += L"soldier_animator_controller";
			_path += L".animctrl";

			Ptr<AnimatorController> _controller = ResourceMgr::GetInst()->FindRes<AnimatorController>(_path);
			if (_controller == nullptr)
			{
				_controller = ResourceMgr::GetInst()->CreateAnimatorController(_path);
			}

			enemy->GetAnimator3D()->SetAnimatorController(_controller);

			GameObject* enemy_socket = new GameObject;
			enemy_socket->AddComponent(new Transform);
			enemy_socket->AddComponent(new Socket);
			enemy->AddChild(enemy_socket);
			enemy_socket->GetSocket()->SetSocket(L"R_Weapon_a");
			enemy_socket->SetName(L"Weapon Socket");
		}
		_meshdata_t = ResourceMgr::GetInst()->LoadGLTF(L"FF7R Resource\\GameContents\\Character\\Weapon\\WE7000_00_SecuritySoldier_Gun\\Model\\WE7000_00.gltf");
		if (_meshdata_t != nullptr)
		{
			GameObject* _enemy_gun;
			_enemy_gun = _meshdata_t->Instantiate();
			_enemy_gun->SetName(L"Soldier Weapon");
			_enemy_gun->SetLayerIndexByName(L"Weapon");
			_enemy_gun->AddComponent(new SoldierWeaponScript);
			_enemy_gun->GetTransform()->SetRelativeScale(1, 1, 1);
			enemy->GetScript<SoldierScript>()->SetWeapon(_enemy_gun->GetScript<SoldierWeaponScript>());
			_enemy_gun->GetTransform()->SetRelativeRot(vec3(-90.f * 3.141592f / 180.f, 0.0f, 90.f * 3.141592f / 180.f));
		}
	}
}