#include "pch.h"
#include "PhysXMgr.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider3D.h"
#include "TimeMgr.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"

//#include <PhysX/PxPhysicsAPI.h>
using namespace physx;

#include "PhysXFunc.h"

//#include <PhysX/PxPhysics.h>

//#include <PhysX/foundation/PxVec3.h>
//using namespace physx;
namespace ff7r
{
	CollisionCallback Collision_Callback = nullptr; // 전역 변수 선언
	UINT	PhysXMgr::collision_layer[MAX_LAYER] = {};

	PhysXMgr::PhysXMgr()
		: m_hMainHwnd(nullptr)
		, px_foundation(NULL)
		, px_physics(NULL)
		, px_dispatcher(NULL)
		, px_scene(NULL)
		, px_pvd(NULL)
		, px_material(NULL)
		, target_objects({})
		, is_destroy(false)
		, stop_update(false)

	{

	}

	PhysXMgr::~PhysXMgr()
	{
		//return;
		//delete px_physics;
		//delete px_dispatcher;
		PhysXMgr::GetInst()->SetDestroy();
		for (auto _pair : target_objects)
		{
			Actor* _actor = _pair.second;
			_actor->Col->SceneDestroy();
			delete _actor;
			_actor = nullptr;
		}
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);


		if (nbActors > 0)
		{

			std::vector<PxRigidActor*> actors(nbActors);
			px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			for (int i = 0; i < nbActors; i++)
			{
				/*const char* _name = actors[i]->getName();
				if (_name != nullptr)
					delete _name;*/
				UINT* _trigger_id = static_cast<UINT*>(actors[i]->userData);
				delete _trigger_id;
				_trigger_id = nullptr;
				actors[i]->release();

			}
		}

		CustomFilterCallback* _scene_filter_callback = dynamic_cast<CustomFilterCallback*>(px_scene->getFilterCallback());
		if (_scene_filter_callback != nullptr)
		{
			delete _scene_filter_callback;
		}
		ContactReportCallback* _scene_event_callback = dynamic_cast<ContactReportCallback*>(px_scene->getSimulationEventCallback());
		if (_scene_event_callback != nullptr)
		{
			delete _scene_event_callback;
		}

		px_scene->release();
		px_material->release();
		px_dispatcher->release();
		px_pvd->release();
		px_physics->release();
		px_foundation->release();
	}


	void PhysXMgr::Init()
	{
		SetLayerCollision(0, 3, true);
		SetLayerCollision(1, 3, true);
		SetLayerCollision(1, 4, true);
		SetLayerCollision(3, 4, true);	
		SetLayerCollision(3, 7, true);
		SetLayerCollision(3, 9, true);
		SetLayerCollision(0, 1, true);
		SetLayerCollision(12, 12, true);
		SetLayerCollision(12, 1, true);
		SetLayerCollision(0, 5, true);
		SetLayerCollision(0, 13, true);

		//return;
		//m_hMainHwnd = _hWnd;
		//return;
		//PhysX Foundation 초기화
		SetCollisionCallback([](UINT a, UINT b)->bool {return PhysXMgr::IsLayerCollision(a, b); });

		px_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, px_allocator, px_error_callback);
		if (px_foundation == NULL)
		{
			assert(true);
		}
		PxTolerancesScale _px_tolerances_scale = PxTolerancesScale();


		// physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("hi", 5425, 10);
		// px_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		px_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *px_foundation, _px_tolerances_scale, true, px_pvd);
		if (px_physics == NULL)
		{
			assert(true);
		}

		//PhysX 물리 sdk 초기화
		if (px_foundation)
		{
			px_pvd = PxCreatePvd(*px_foundation);
		}
		if (px_pvd == NULL)
		{
			assert(true);
		}

		// Extensions 초기화
		//PxInitExtensions(*px_physics, px_pvd);

		// CPU dispatcher
		//scene Desc용 Dispatcher를 생성
		//gDispatcher = PxDefaultCpuDispatcherCreate(2);
		px_dispatcher = PxDefaultCpuDispatcherCreate(2, NULL);



		//물리 시뮬레이션 씬 Desc 설정
		PxSceneDesc sceneDesc(px_physics->getTolerancesScale());
		//sceneDesc.gravity = PxVec3(0.0f, -19.62f, 0.0f);
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		//sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
		sceneDesc.cpuDispatcher = px_dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		sceneDesc.filterShader = triggersUsingFilterCallback;
		CustomFilterCallback* _callback = new CustomFilterCallback();
		sceneDesc.filterCallback = (PxSimulationFilterCallback*)_callback;
		ContactReportCallback* _call = new ContactReportCallback;
		sceneDesc.simulationEventCallback = (PxSimulationEventCallback*)_call;
		//씬의 설정값을 토대로 씬을 생성

		px_scene = px_physics->createScene(sceneDesc);

		if (px_scene == NULL)
		{
			assert(true);
		}
		px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

		// Debugger client.
		PxPvdSceneClient* pvdClient = px_scene->getScenePvdClient();
		if (pvdClient) 
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		// Controller Manager for CCT (Character Controller)
		//px_con_manager = PxCreateControllerManager(*px_scene);


		// Material을 만들어 줍니다.
		px_material = px_physics->createMaterial(1.0f, 1.0f, 0.0f); // 충돌체 마찰력,Dynamic마찰력, 탄성력

		PxRigidStatic* groundPlane = PxCreatePlane(*px_physics, PxPlane(0, 1, 0, -1), *px_material);
		//px_scene->addActor(*groundPlane);


		//for (auto iter = target_objects.begin(); iter != target_objects.end(); iter++)
		//{
		//    Actor* _target = iter->second;
		//    GameObject* _object = _target->object;
		//    Transform* _transform = _object->GetTransform();
		//    Collider3D* _collider = _object->GetCollider3D();

		//    Matrix _target_world_mat = _collider->GetColWorld();
		//    PxTransform _px_transform = PxTransform(PxVec3(_target_world_mat._41, _target_world_mat._42, _target_world_mat._43));
		//    
		//    vec3 _target_scale = _collider->GetOffsetScale();
		//    //PxMaterial
		//    //PxShape* _shape = px_physics->createShape(PxBoxGeometry(_target_scale.x, _target_scale.y, _target_scale.z), *px_material);
		//    
		//    //layer 설정
		//    PxShape* _shape = px_physics->createShape(PxBoxGeometry(100.0f, 100.0f, 100.0f), *px_material);
		//    PxFilterData _filterData;
		//    _filterData.word0 = _target->Layer;
		//    _shape->setSimulationFilterData(_filterData);

		//    PxTransform localTm(PxVec3(PxReal(0), PxReal(0), 0) * 1.0f);
		//    PxRigidDynamic* body = px_physics->createRigidDynamic(_px_transform.transform(localTm));
		//    body->attachShape(*_shape);

		//    wstring _wstr_target_name = iter->first;
		//    string _str_target_name = string(_wstr_target_name.begin(), _wstr_target_name.end());
		//    //const char name[] = "abcdef";
		//    char* name = new char[_str_target_name.size() + 1];
		//    strcpy_s(name, _str_target_name.size() + 1, _str_target_name.c_str());

		//    //body->setName(name);
		//    PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		//    PxActor* _actor = body;
		//    _actor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
		//    _actor->setName(name);

		//    px_scene->addActor(*_actor);
		//    _shape->release();
		//    
		//}

	   /* PxFilterData _filterData;
		_filterData.word0 = _target->Layer;
		_shape->setSimulationFilterData(_filterData);*/
		//pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

		//px_scene->setFilterShaderData();
		/*float stackZ = 10.0f;
		for (PxU32 i = 0; i < 5; i++)
		{
			PxTransform t = PxTransform(PxVec3(0, 0, stackZ -= 10.0f));
			int size = 10;
			float halfExtent = 2.0f;
			PxShape* shape = px_physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), px_material);
			for (PxU32 i = 0; i < size; i++)
			{
				for (PxU32 j = 0; j < size - i; j++)
				{
					PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
					PxRigidDynamic* body = px_physics->createRigidDynamic(t.transform(localTm));
					body->attachShape(*shape);
					PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
					px_scene->addActor(*body);
				}
			}
			shape->release();
		}*/
		/*bool interactive = false;
		if (!interactive)
		{
			PxTransform t = PxTransform(PxVec3(0, 40, 100));
			PxGeometry* geometry = &PxSphereGeometry(10);
			PxVec3 velocity = PxVec3(0, -50, -100);
			PxRigidDynamic* dynamic = PxCreateDynamic(*px_physics, t, *geometry, *px_material, 10.0f);
			dynamic->setAngularDamping(0.5f);
			dynamic->setLinearVelocity(velocity);
			px_scene->addActor(*dynamic);
		}*/


		// 평지를 만들어 줍니다.
		//PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *m_pMaterial);
		//m_pScene->addActor(*groundPlane);
	}

	void PhysXMgr::progress()
	{
		if (stop_update)
		{
			return;
		}

		delta_time += DT;

		while (true)
		{
			if (delta_time > 1. / 60.)
			{
				delta_time -= 1. / 60.;

				Begin();
				Tick();
				FinalTick();
			}
			else
			{
				break;
			}
		}
		return;
		Render();
	}

	void PhysXMgr::AddTarget(Collider3D* _target_col, bool _is_trigger, bool _is_dynamic)
	{
		Actor* _actor = new Actor();
		GameObject* _object = _target_col->GetOwner();
		_actor->Col = _target_col;
		_actor->Layer = _object->GetLayerIndex();
		target_objects.insert(make_pair(_actor->Col->GetID(), _actor));

		Transform* _transform = _object->GetTransform();
		Collider3D* _collider = _object->GetCollider3D();

		Matrix _target_world_mat = _collider->GetColWorld();
		PxTransform _px_transform = PxTransform(PxVec3(_target_world_mat._41, _target_world_mat._42, _target_world_mat._43));

		vec3 _target_scale = _collider->GetOffsetScale();
		//PxMaterial
		//PxShape* _shape = px_physics->createShape(PxBoxGeometry(_target_scale.x, _target_scale.y, _target_scale.z), *px_material);

		//layer 설정
		PxActor* _px_actor;


		PxShape* _shape;
		if (_is_trigger)
		{
			const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;
			// | PxShapeFlag::eSIMULATION_SHAPE;
			//shape = gPhysics->createShape(geom, *gMaterial, isExclusive, shapeFlags);

			//_shape = px_physics->createShape(PxBoxGeometry(1.0f, 1.0f, 1.0f), *px_material, true, shapeFlags);
			//SetGeometry(_shape, _collider->GetType());
			_shape = CreateGeometry(_collider->GetType(), _target_scale, *px_material, true, shapeFlags);

		}
		else
		{
			//_shape = px_physics->createShape(PxBoxGeometry(1.0f, 1.0f, 1.0f), *px_material, false);
			const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;
			//SetGeometry(_shape, _collider->GetType());
			_shape = CreateGeometry(_collider->GetType(), _target_scale, *px_material, false, shapeFlags);

		}
		PxFilterData _filterData;
		_filterData.word0 = _actor->Layer;
		_shape->setSimulationFilterData(_filterData);

		PxTransform localTm(PxVec3(PxReal(0), PxReal(0), 0) * 1.0f);

		UINT* _actor_id = new UINT;
		*_actor_id = _collider->GetID();
		/*_px_actor->setName
		wstring _wstr_target_name = _object->GetName();
		string _str_target_name = string(_wstr_target_name.begin(), _wstr_target_name.end());
		strcpy_s(name, _str_target_name.size() + 1, _str_target_name.c_str());
		char* name = new char[_str_target_name.size() + 1];*/
		static int dsf = 0;
		if (_is_dynamic)
		{
			PxRigidDynamic* body = px_physics->createRigidDynamic(_px_transform.transform(localTm));
			//PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);


				body->setMass(_collider->GetMass());
			


			//if(dsf <= 0)
			//{
			//    body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			//    dsf++;
			//}
			//else
			//{
			//    body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
			//}
			body->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
			body->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
			
			body->attachShape(*_shape);
			_px_actor = body;
		}

		else
		{
			PxRigidStatic* body = px_physics->createRigidStatic(_px_transform.transform(localTm));
			body->attachShape(*_shape);
			_px_actor = body;
		}
		_px_actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);



		/*if (_is_trigger)
		{
			_px_actor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
		}

		else
		{
			_px_actor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
		}*/
		//_px_actor->setName(name);
		_px_actor->userData = _actor_id;
		px_scene->addActor(*_px_actor);
		UINT a = _px_actor->getActorFlags();
		_shape->release();
	}

	void PhysXMgr::EraseTarget(Collider3D* _target_col)
	{
		UINT _id = _target_col->GetID();
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors > 0)
		{

			std::vector<PxRigidActor*> actors(nbActors);
			px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			for (int i = 0; i < nbActors; i++)
			{
				const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));
				if (_id == _actor_id)
				{
					UINT* _trigger_id = static_cast<UINT*>(actors[i]->userData);
					delete _trigger_id;
					_trigger_id = nullptr;
					actors[i]->userData = nullptr;
					actors[i]->release();
				}
			}
			auto iter = target_objects.find(_id);
			if (iter != target_objects.end())
				target_objects.erase(iter);
		}
	}

	void PhysXMgr::SetGeometry(physx::PxShape* _shape, COLLIDER3D_TYPE _type)
	{
		switch (_type)
		{
		case COLLIDER3D_TYPE::CUBE:
		{
			_shape->setGeometry(PxBoxGeometry(1.0f, 1.0f, 1.0f));
			break;
		}
		case COLLIDER3D_TYPE::SPHERE:
		{
			_shape->setGeometry(PxSphereGeometry(1.0f));
			break;
		}
		case COLLIDER3D_TYPE::CAPSULE:
		{
			PxCapsuleGeometry a = PxCapsuleGeometry(1.0f, 1.0f);
			_shape->setGeometry(a);
			//_shape->setGeometry(PxCapsuleGeometry(1.0f, 1.0f));
			break;
		}
		case COLLIDER3D_TYPE::CONVEXMESH:
		{
			break;
		}
		default:
			break;
		}

	}

	PxShape* PhysXMgr::CreateGeometry(COLLIDER3D_TYPE _type, vec3 _scale, const PxMaterial& _material, bool _is_exclusive, PxShapeFlags _flags)
	{
		PxShape* _shape = nullptr;
		switch (_type)
		{
		case COLLIDER3D_TYPE::CUBE:
		{
			_shape = px_physics->createShape(PxBoxGeometry(0.5f * _scale.x, 0.5f * _scale.y, 0.5f * _scale.z), *px_material, _is_exclusive, _flags);
			break;
		}
		case COLLIDER3D_TYPE::SPHERE:
		{
			_shape = px_physics->createShape(PxSphereGeometry(0.5f * _scale.x), *px_material, _is_exclusive, _flags);
			break;
		}
		case COLLIDER3D_TYPE::CAPSULE:
		{

			vec3 _fixed_scale;


			PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
			_shape = px_physics->createShape(PxCapsuleGeometry(0.25f * _scale.x, 0.25f * (_scale.y * 2.f - _scale.x)), *px_material, _is_exclusive, _flags);
			_shape->setLocalPose(relativePose);
			break;
		}
		case COLLIDER3D_TYPE::CONVEXMESH:
		{
			break;
		}
		default:
			break;
		}
		return _shape;
	}

	void PhysXMgr::DrawShape(physx::PxShape* _shape, vec3 _vWorldPos, vec3 _vWorldScale, vec3 _vRotation, Collider3D* _collider)
	{

		//enum Enum
		//{
		//    eSPHERE,
		//    ePLANE,
		//    eCAPSULE,
		//    eBOX,
		//    eCONVEXMESH,
		//    ePARTICLESYSTEM,
		//    eTETRAHEDRONMESH,
		//    eTRIANGLEMESH,
		//    eHEIGHTFIELD,
		//    eHAIRSYSTEM,
		//    eCUSTOM,

		//    eGEOMETRY_COUNT,	//!< internal use only!
		//    eINVALID = -1		//!< internal use only!
		//};
		vec4 color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		if (_collider->IsCollision())
		{
			color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		switch (_shape->getGeometry().getType())
		{
		case PxGeometryType::Enum::eBOX:
		{
			DrawDebugCube(_vWorldPos, _collider->GetOffsetScale(), color, _vRotation / 180.0f * XM_PI, 0.0f, false);
			break;
		}
		case PxGeometryType::Enum::eSPHERE:
		{
			DrawDebugSphere(_vWorldPos, _collider->GetOffsetScale(), color, _vRotation / 180.0f * XM_PI, 0.0f, false);
			break;
		}
		case PxGeometryType::Enum::eCAPSULE:
		{
			DrawDebugCapsule(_vWorldPos, _collider->GetOffsetScale(), color, _vRotation / 180.0f * XM_PI, 0.0f, false);
			break;
		}
		case PxGeometryType::Enum::eCONVEXMESH:
		{

			break;
		}
		default:
			break;
		}
	}



	void PhysXMgr::Begin()
	{

	}

	void PhysXMgr::Tick()
	{

		/*for (const auto& pair : m_mapUI)
		{
			pair.second->Tick();
		}*/
		for (auto iter = pre_disable.begin(); iter != pre_disable.end(); iter++)
		{
			post_disable[iter->first] = iter->second;
		}
		pre_disable.clear();

		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors)
		{
			std::vector<PxRigidActor*> actors(nbActors);
			px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			for (int i = 0; i < nbActors; i++)
			{
				PxRigidActor* _actor = actors[i];
				const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));

				Collider3D* _collider = target_objects[_actor_id]->Col;

				PxRigidDynamic* abc = _actor->is<PxRigidDynamic>();

				if (abc != nullptr)
				{
					// if (i == 0)
					//     PxRigidBodyExt::updateMassAndInertia(*abc, 1.0f);
					// else if (i == 1)
					//     PxRigidBodyExt::updateMassAndInertia(*abc, 10000.0f);


					if (_actor->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY)
					{
						abc->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
						abc->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
					}
				}
				PxShape* shapes[256];

				const PxU32 nbShapes = _actor->getNbShapes();
				_actor->getShapes(shapes, nbShapes);


				Matrix _target_world_mat = _collider->GetColWorld();

				Vector3 _mat_target_pos;
				Quaternion _mat_target_rot;
				Vector3 _dummy;
				_target_world_mat.Decompose(_dummy, _mat_target_rot, _mat_target_pos);
				//PxTransform _target_pos = PxTransform(PxVec3(_target_world_mat._41, _target_world_mat._42, _target_world_mat._43));
				PxTransform _target_pos = PxTransform(PxVec3(_mat_target_pos.x, _mat_target_pos.y, _mat_target_pos.z), PxQuat(_mat_target_rot.x, _mat_target_rot.y, _mat_target_rot.z, _mat_target_rot.w));
				_actor->setGlobalPose(_target_pos);

				//const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[0], *actor));
				//PxTransform b = PxShapeExt::getGlobalPose(*shapes[0], *_actor);
				//vec4 _rot = vec4::Transform(_rot,rotPxTransform(&b));
				//vec3 _pos = posPxTransform(&b);
				////vec3 _pos = vec3(0.0f);
				//
				//vec4 color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
				///*
				//
				//vec4 
				//DrawDebugCube(mat_colli_world, color, 0.f);
				//
				//DrawDebugCircle(vec3 _vWorldPos, float _fRadius, vec4 _vColor, vec3 _vRotation
				//    , float _fTime, bool DepthTest)*/
				//DrawDebugCube(_pos, 10.0f, color, vec3(_rot.x, _rot.y, _rot.z));
			}

		}

		px_scene->simulate(1. / 60.);
		//px_scene->simulate(DT);
		px_scene->fetchResults(true);
	}

	void PhysXMgr::FinalTick()
	{
		for (auto iter = post_disable.begin(); iter != post_disable.end(); iter++)
		{
			iter->second->SetCollision(false);
		}
		post_disable.clear();
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
		if (nbActors)
		{

			std::vector<PxRigidActor*> actors(nbActors);
			px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC /* | PxActorTypeFlag::eRIGID_STATIC*/, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			for (int i = 0; i < nbActors; i++)
			{
				PxRigidActor* _actor = actors[i];
				const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));

				//const char* _name = _actor->getName();
				//if (_name == nullptr)
				//    continue;
				//string _str_actor_name = string(_actor->getName());
				Collider3D* _collider = target_objects[_actor_id]->Col;

				const PxU32 nbShapes = _actor->getNbShapes();
				std::vector<PxShape*> shapes(nbShapes);
				//PxShape* shapes[256];

				_actor->getShapes(reinterpret_cast<PxShape**>(&shapes[0]), nbShapes);

				for (int j = 0; j < nbShapes; j++)
				{
					PxTransform _px_transform = PxShapeExt::getGlobalPose(*shapes[j], *_actor);

					vec3 _px_pos = posPxTransform(&_px_transform);
					Quaternion _px_rot = rotPxTransform(&_px_transform);

					Matrix _mat_trans = XMMatrixTranslation(_px_pos.x, _px_pos.y, _px_pos.z);
					Matrix _mat_rot = XMMatrixRotationQuaternion(_px_rot);
					Matrix _world = _mat_rot * _mat_trans;

					vec3 _pos;
					vec3 _rot;
					vec3 _scale;

					DecomposeEuler(_scale, _rot, _pos, _world);

					//PxRigidDynamic* abc = _actor->is<PxRigidDynamic>();
					//if (abc != nullptr)
					//{
					//    // if (i == 0)
					//    //     PxRigidBodyExt::updateMassAndInertia(*abc, 1.0f);
					//    // else if (i == 1)
					//    //     PxRigidBodyExt::updateMassAndInertia(*abc, 10000.0f);

					//    PxActorFlags a = _actor->getActorFlags();
					//    if (_actor->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY)
					//    {
					//        abc->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
					//    }
					//}
					//eSPHERE,
					//    ePLANE,++
					//    eCAPSULE,
					//    eBOX,
					//    eCONVEXMESH,
					//DrawShape((UINT)_collider->GetType(), _pos, _scale, _rot);


					//if(shapes[j]->getGeometry().getType() == PxGeometryType::Enum::eBOX)
					//DrawDebugCube(_pos, _scale, color, _rot / 180.0f * XM_PI, 0.0f, false);


					if (!_collider->IsMove())
					{
						continue;
					}

					if (!(shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE))
					{
						Transform* _target_trans = _collider->GetTransform();
						GameObject* _parent = _collider->GetOwner()->GetParent();
						if (_parent)
						{

							Matrix _parent_inverse_mat = XMMatrixInverse(nullptr, _parent->GetTransform()->GetWorldMat());
							Matrix _parent_scale = _parent->GetTransform()->GetWorldScaleMat();
							_world = _world * _parent_inverse_mat * _parent_scale;

							DecomposeEuler(_scale, _rot, _pos, _world);

						}
						float _y = _target_trans->GetTransform()->GetRelativePos().y;
						_pos = _pos - rotVector(_collider->GetOffsetPos(), _collider->GetTransform()->GetRelativeRot());
						//if (_pos.y <= 0.0f)
						//{
						//    _actor->setActorFlags(_actor->getActorFlags() | PxActorFlag::eDISABLE_GRAVITY);
						//    _pos.y = 0.0f;
						//    _collider->SetGravity(false);
						//}

						if (_actor->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY)
						{

							_target_trans->SetRelativePos(_pos.x, _pos.y, _pos.z);
						}
						else
						{
							_target_trans->SetRelativePos(_pos.x, _pos.y, _pos.z);
						}
					}

				}
				if (gravity_disable.find(_actor_id) != gravity_disable.end())
				{
					SetActorGravity(_actor_id, gravity_disable[_actor_id]);
				}
				if (add_force.find(_actor_id) != add_force.end())
				{
					AddActorForce(_actor_id, add_force[_actor_id]);
				}

			}

		}
		add_force.clear();
		gravity_disable.clear();

		PxU32 nbActors_static = px_scene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors_static)
		{

			std::vector<PxRigidActor*> actors(nbActors_static);
			px_scene->getActors(PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors_static);
			for (int i = 0; i < nbActors_static; i++)
			{
				PxRigidActor* _actor = actors[i];
				const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));

				//const char* _name = _actor->getName();
				//if (_name == nullptr)
				//    continue;
				//string _str_actor_name = string(_actor->getName());
				Collider3D* _collider = target_objects[_actor_id]->Col;


				const PxU32 nbShapes = _actor->getNbShapes();
				std::vector<PxShape*> shapes(nbShapes);
				//PxShape* shapes[256];

				_actor->getShapes(reinterpret_cast<PxShape**>(&shapes[0]), nbShapes);

				for (int j = 0; j < nbShapes; j++)
				{
					PxTransform _px_transform = PxShapeExt::getGlobalPose(*shapes[j], *_actor);

					vec3 _px_pos = posPxTransform(&_px_transform);
					Quaternion _px_rot = rotPxTransform(&_px_transform);

					Matrix _mat_trans = XMMatrixTranslation(_px_pos.x, _px_pos.y, _px_pos.z);
					Matrix _mat_rot = XMMatrixRotationQuaternion(_px_rot);
					Matrix _world = _mat_rot * _mat_trans;

					vec3 _pos;
					vec3 _rot;
					vec3 _scale;

					DecomposeEuler(_scale, _rot, _pos, _world);


				}
			}
		}
	}

	void PhysXMgr::Render()
	{
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors)
		{
			std::vector<PxRigidActor*> actors(nbActors);
			px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			for (int i = 0; i < nbActors; i++)
			{
				PxRigidActor* _actor = actors[i];
				if (_actor->getActorFlags() & PxActorFlag::Enum::eDISABLE_SIMULATION)
				{
					continue;
				}
				const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));

				Collider3D* _collider = target_objects[_actor_id]->Col;

				const PxU32 nbShapes = _actor->getNbShapes();
				std::vector<PxShape*> shapes(nbShapes);

				_actor->getShapes(reinterpret_cast<PxShape**>(&shapes[0]), nbShapes);

				for (int j = 0; j < nbShapes; j++)
				{
					PxTransform _px_transform = PxShapeExt::getGlobalPose(*shapes[j], *_actor);

					vec3 _px_pos = posPxTransform(&_px_transform);
					Quaternion _px_rot = rotPxTransform(&_px_transform);

					Matrix _mat_trans = XMMatrixTranslation(_px_pos.x, _px_pos.y, _px_pos.z);
					Matrix _mat_rot = XMMatrixRotationQuaternion(_px_rot);
					Matrix _world = _mat_rot * _mat_trans;

					vec3 _pos;
					vec3 _rot;
					vec3 _scale;

					DecomposeEuler(_scale, _rot, _pos, _world);

					vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

					vec3 _draw_pos = _pos;
					DrawShape(shapes[j], _draw_pos, _scale, _rot, _collider);

				}

			}

		}

	}

	void PhysXMgr::SetLayerFlag(UINT _layer, UINT _actor_flag, bool _option)
	{
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		PxActor** actors = new PxActor * [nbActors];
		px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, actors, nbActors);
		for (auto iter = target_objects.begin(); iter != target_objects.end(); iter++)
		{
			GameObject* _object = iter->second->Col->GetOwner();
			if (_object->GetLayerIndex() == _layer)
			{
				//wstring _wstr_object_name = iter->first;
				//string _str_object_name = string(_wstr_object_name.begin(), _wstr_object_name.end());
				//const char* _object_name = _str_object_name.c_str();
				UINT _collider_id = iter->first;
				for (int i = 0; i < nbActors; i++)
				{
					//string _str_actor_name = string(actors[i]->getName());
					//wstring _wstr_actor_name = wstring(_str_actor_name.begin(), _str_actor_name.end());
					const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));
					if (_collider_id == _actor_id)
					{
						actors[i]->setActorFlag((PxActorFlag::Enum)_actor_flag, _option);
					}
				}
			}
		}
		delete[] actors;
	}

	void PhysXMgr::SetLayerCollision(UINT _left_layer, UINT _right_layer, bool _option)
	{
		UINT row = (UINT)_left_layer;
		UINT col = (UINT)_right_layer;

		if (row > col)
		{
			UINT iTemp = col;
			col = row;
			row = iTemp;
		}
		if (_option)
		{
			collision_layer[row] |= (1 << col);
		}
		else
		{
			collision_layer[row] &= ~(1 << col);
		}
	}

	void PhysXMgr::SetDisable(Collider3D* _collider)
	{
		if (target_objects.find(_collider->GetID()) != target_objects.end())
		{
			pre_disable.insert(std::make_pair(_collider->GetID(), _collider));
		}
	}

	void PhysXMgr::AddActorForce(UINT _collider_id, vec3 _force)
	{
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);// | PxActorTypeFlag::eRIGID_STATIC);
		std::vector<PxRigidActor*> actors(nbActors);
		px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC/* | PxActorTypeFlag::eRIGID_STATIC*/, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		PxRigidActor* _actor = nullptr;

		for (int i = 0; i < nbActors; i++)
		{
			const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));
			if (_collider_id == _actor_id)
			{
				_actor = dynamic_cast<PxRigidActor*>(actors[i]);
				break;
			}
		}
		if (_actor == nullptr)
		{
			return;
		}
		PxRigidDynamic* _actor_rigid = _actor->is<PxRigidDynamic>();
		if (_actor_rigid != nullptr)
		{
			PxVec3 _px_force = PxVec3(_force.x, _force.y, _force.z);
			_actor_rigid->addForce(_px_force);
		}
		SetActorGravity(_collider_id, true);

	}

	void PhysXMgr::AddForce(Collider3D* _collider, vec3 _force)
	{
		add_force.insert(std::make_pair(_collider->GetID(), _force));
	}


	bool PhysXMgr::IsLayerCollision(UINT _left_layer, UINT _right_layer)
	{
		if (collision_layer[_left_layer] & (1 << _right_layer))
			return true;

		return false;
	}

	void PhysXMgr::SetActorFlag(UINT _collider_id, PxActorFlag::Enum _actor_flag, bool _option)
	{
		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		std::vector<PxRigidActor*> actors(nbActors);
		px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		for (int i = 0; i < nbActors; i++)
		{
			const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));
			if (_collider_id == _actor_id)
			{
				PxActorFlags a = actors[i]->getActorFlags();
				//if (_option)
				//{
				//    PxRigidDynamic* abc = actors[i]->is<PxRigidDynamic>();
				//    abc->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
				//}
				actors[i]->setActorFlag((PxActorFlag::Enum)_actor_flag, _option);
				PxActorFlags n = actors[i]->getActorFlags();


				break;
			}
		}
	}

	void PhysXMgr::SetActorCollision(UINT _collider_id, bool _collision)
	{
		SetActorFlag(_collider_id, PxActorFlag::Enum::eDISABLE_SIMULATION, !_collision);
	}

	void PhysXMgr::SetActorTrigger(UINT _collider_id, bool _collision)
	{


		PxU32 nbActors = px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		std::vector<PxRigidActor*> actors(nbActors);
		px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		PxRigidActor* _actor = nullptr;

		for (int i = 0; i < nbActors; i++)
		{
			const UINT _actor_id = *(static_cast<const UINT*>(actors[i]->userData));
			if (_collider_id == _actor_id)
			{
				_actor = dynamic_cast<PxRigidActor*>(actors[i]);
				break;
			}
		}
		if (_actor == nullptr)
		{
			return;
		}
		const PxU32 nbShapes = _actor->getNbShapes();
		std::vector<PxShape*> shapes(nbShapes);
		//PxShape* shapes[256];

		_actor->getShapes(reinterpret_cast<PxShape**>(&shapes[0]), nbShapes);

		for (int j = 0; j < nbShapes; j++)
		{
			PxShape* _shape = shapes[j];
			//PxShapeFlag::Enum
			PxShapeFlags _shape_flags = _shape->getFlags();
			if (_collision)
			{
				_shape_flags |= PxShapeFlag::Enum::eTRIGGER_SHAPE;
			}
			else
			{
				_shape_flags &= ~(PxShapeFlag::Enum::eTRIGGER_SHAPE);
			}
			_shape->setFlags(_shape_flags);
		}
	}

	void PhysXMgr::SetActorGravity(UINT _collider_id, bool _collision)
	{
		SetActorFlag(_collider_id, PxActorFlag::Enum::eDISABLE_GRAVITY, !_collision);
	}

	void PhysXMgr::SetGravity(Collider3D* _collider, bool _gravity)
	{
		gravity_disable.insert(std::make_pair(_collider->GetID(), _gravity));
	}



	void PhysXMgr::ObserveContent()
	{
		//DWORD dwWaitStatus = WaitForSingleObject(m_hObserver, 0);

		//if (dwWaitStatus == WAIT_OBJECT_0)
		//{
		//    // content 폴더에 변경점이 생겼다.
		//    ContentUI* UI = (ContentUI*)FindUI("##Content");
		//    UI->Reload();

		//    FindNextChangeNotification(m_hObserver);
		//}
	}
	void PhysXMgr::ContactReportCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		{
			// tri
			//		printf("onTrigger: %d trigger pairs\n", count);
			while (count--)
			{
				const physx::PxTriggerPair& current = *pairs++;
				if (current.status)
				{
					void* _trigger_data = current.triggerActor->userData;
					void* _other_data = current.otherActor->userData;
					//const char* _name = current.triggerActor->getName();
					//string _str_name = string(_name);
					//wstring _wstr_name = wstring(_str_name.begin(), _str_name.end());
					//const char* _name2 = current.otherActor->getName();
					//string _str_name2 = string(_name);
					//wstring _wstr_name2 = wstring(_str_name.begin(), _str_name.end());

					if ((_trigger_data == nullptr) != (_other_data == nullptr))
					{
						return;
					}

					const UINT _trigger_id = *(static_cast<const UINT*>(_trigger_data));

					const UINT _other_id = *(static_cast<const UINT*>(_other_data));



					// trigger
					Collider3D* target_col = PhysXMgr::GetInst()->target_objects[_trigger_id]->Col;
					// 강체
					Collider3D* other_col = PhysXMgr::GetInst()->target_objects[_other_id]->Col;


					// collider3D에 actor 주소 할당
					//

					vector<vec3> _none_vec = { vec3(0.0f, 0.0f, 0.0f) };

					bool _collision_disable = false;
					_collision_disable |= PhysXMgr::GetInst()->IsDisable(target_col->GetID());
					_collision_disable |= PhysXMgr::GetInst()->IsDisable(other_col->GetID());

					if (_collision_disable)
					{
						target_col->EndOverlap(other_col, _none_vec);
						other_col->EndOverlap(target_col, _none_vec);
					}
					else
					{
						if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
						{
							target_col->BeginOverlap(other_col, _none_vec);
							other_col->BeginOverlap(target_col, _none_vec);
						}
						else if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
						{
							target_col->OnOverlap(other_col, _none_vec);
							other_col->OnOverlap(target_col, _none_vec);
						}
						else if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
						{
							target_col->EndOverlap(other_col, _none_vec);
							other_col->EndOverlap(target_col, _none_vec);
						}
					}
				}
			}
		}
	}

	void PhysXMgr::ContactReportCallback::onContact(const physx::PxContactPairHeader& pair_header, const physx::PxContactPair* pairs, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; i++)
		{
			const physx::PxContactPair& cp = pairs[i];


			if (cp.events & (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_LOST/* | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eSOLVE_CONTACT*/))
			{
				void* _actor0_data = pair_header.actors[0]->userData;
				void* _actor1_data = pair_header.actors[1]->userData;

				if ((_actor0_data == nullptr) || (_actor1_data == nullptr) )
				{
					continue;
				}

				const UINT _actor0_id = *(static_cast<const UINT*>(_actor0_data));
				const UINT _actor1_id = *(static_cast<const UINT*>(_actor1_data));

				Collider3D* collider0 = PhysXMgr::GetInst()->target_objects[_actor0_id]->Col;
				Collider3D* collider1 = PhysXMgr::GetInst()->target_objects[_actor1_id]->Col;

				// 충돌 지점 정보 추출
				physx::PxContactPairPoint contactPoints[20]; // 예제를 위한 배열 크기 설정
				physx::PxU32 nbContacts = cp.extractContacts(contactPoints, 20);

				vector<vec3> contact_pos;
				for (size_t i = 0; i < (size_t)nbContacts; i++)
				{
					physx::PxVec3& point = contactPoints[i].position;
					contact_pos.push_back(vec3(point.x, point.y, point.z));
				}

				bool _collision_disable = false;
				_collision_disable |= PhysXMgr::GetInst()->IsDisable(collider0->GetID());
				_collision_disable |= PhysXMgr::GetInst()->IsDisable(collider1->GetID());

				if (_collision_disable)
				{
					collider0->EndOverlap(collider1, contact_pos);
					collider1->EndOverlap(collider0, contact_pos);
				}
				else
				{
					if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
					{
						collider0->BeginOverlap(collider1, contact_pos);
						collider1->BeginOverlap(collider0, contact_pos);
					}
					else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
					{
						collider0->OnOverlap(collider1, contact_pos);
						collider1->OnOverlap(collider0, contact_pos);
					}
					else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
					{
						collider0->EndOverlap(collider1, contact_pos);
						collider1->EndOverlap(collider0, contact_pos);
					}
				}
			}
		}
	}
}
