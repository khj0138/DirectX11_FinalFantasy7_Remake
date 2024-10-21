#pragma once
#include "Singleton.h"


#include <PhysX/PxPhysicsAPI.h>

namespace ff7r
{
	typedef bool (*CollisionCallback)(UINT _left, UINT _right);

	extern CollisionCallback Collision_Callback; // 전역 변수 선언

	inline void SetCollisionCallback(CollisionCallback callback) // 함수 선언
	{
		Collision_Callback = callback;
	}
	inline bool IsCollision(UINT _left, UINT _right) // 함수 선언
	{
		if (Collision_Callback)
		{
			return Collision_Callback(_left, _right);
		}
		return false;
	}

	class GameObject;
	class Collider3D;
	class PhysXMgr : public Singleton<PhysXMgr>
	{

	private:
		SINGLE(PhysXMgr)

			struct Actor
		{
			UINT Layer;
			Collider3D* Col;
		};
		HWND                m_hMainHwnd;

		HANDLE              m_hObserver;

	public:

		void Init();

		void progress();

	public:
		HWND GetMainHwnd() { return m_hMainHwnd; }
		void AddTarget(Collider3D* _target_col, bool _is_trigger, bool _is_dynamic);
		void EraseTarget(Collider3D* _target_col);
		void SetGeometry(physx::PxShape* _shape, COLLIDER3D_TYPE _type);
		physx::PxShape* CreateGeometry(COLLIDER3D_TYPE _type, vec3 _scale, const physx::PxMaterial& _material, bool _is_exclusive, physx::PxShapeFlags _flags);
		void DrawShape(physx::PxShape* _shape, vec3 _vWorldPos, vec3 _vWorldScale, vec3 _vRotation, Collider3D* _collider);

		static bool IsLayerCollision(UINT _left_layer, UINT _right_layer);
		static UINT	collision_layer[MAX_LAYER];

		void SetActorFlag(UINT _collider_id, physx::PxActorFlag::Enum _actor_flag, bool _option);
		void SetActorCollision(UINT _collider_id, bool _collision);
		void SetActorTrigger(UINT _collider_id, bool _collision);
		void SetActorGravity(UINT _collider_id, bool _collision);
		void SetGravity(Collider3D* _collider, bool _gravity);
		void SetStopUpdate(bool _is_stop) { stop_update = _is_stop; }

		void SetLayerFlag(UINT _layer, UINT _actor_flag, bool _option);
		//void SetLayerCollision(UINT _left_layer, UINT _right_layer, COLLISION_TYPE _type);
		void SetLayerCollision(UINT _left_layer, UINT _right_layer, bool _option);
		void SetDisable(Collider3D* _collider);
		bool IsDisable(UINT _collider_id) { return(post_disable.find(_collider_id) != post_disable.end()); }
		void AddForce(Collider3D* _collider, vec3 _force);
		void AddActorForce(UINT _collider_id, vec3 _force);
		void SetDestroy() { is_destroy = true; };
		bool GetDestroy() { return is_destroy; }
	private:
		void ObserveContent();

		void Begin();
		void Tick();
		void FinalTick();
		void Render();

		std::map<UINT, Collider3D*> pre_disable;
		std::map<UINT, Collider3D*> post_disable;
		std::map<UINT, vec3> add_force;
		std::map<UINT, bool> gravity_disable;

		//physx::PxActor* FindActor(physx::PxActor* _actor, wstring _key);

		physx::PxDefaultAllocator		px_allocator;
		physx::PxDefaultErrorCallback	px_error_callback;
		physx::PxFoundation* px_foundation;
		physx::PxPhysics* px_physics;
		physx::PxDefaultCpuDispatcher* px_dispatcher;
		physx::PxScene* px_scene;
		physx::PxControllerManager* px_con_manager;


		physx::PxMaterial* px_material;
		physx::PxPvd* px_pvd;
		//map<wstring, Actor*> target_objects;
		map<UINT, Actor*> target_objects;

		map<UINT_PTR, bool>		map_colli_id;

		double delta_time;

		bool is_destroy;
		bool stop_update;

	public:
		class CustomFilterCallback : public physx::PxSimulationFilterCallback {
		public:
			virtual		physx::PxFilterFlags	pairFound(physx::PxU64 pairID,
				physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, const physx::PxActor* a0, const physx::PxShape* s0,
				physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, const physx::PxActor* a1, const physx::PxShape* s1,
				physx::PxPairFlags& pairFlags) PX_OVERRIDE
			{
				//pairFlags = physx::PxPairFlag::eSOLVE_CONTACT |
				//	physx::PxPairFlag::eDETECT_DISCRETE_CONTACT |
				//	physx::PxPairFlag::eDETECT_CCD_CONTACT | // CCD(Continuous Collision Detection) 사용 시
				//	physx::PxPairFlag::eNOTIFY_TOUCH_FOUND |
				//	physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
				//	physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
				//	physx::PxPairFlag::eNOTIFY_CONTACT_POINTS; // 충돌 지점 정보 추출 활성화

				return physx::PxFilterFlags();
			}



			virtual void	pairLost(physx::PxU64 pairID,
				physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
				physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
				bool objectRemoved) PX_OVERRIDE {
				// 필요하다면 처리 가능
				int a = 0;
			}

			virtual		bool	statusChange(physx::PxU64& pairID, physx::PxPairFlags& pairFlags, physx::PxFilterFlags& filterFlags) PX_OVERRIDE {
				return false;
			}
		};
		class ContactReportCallback : public physx::PxSimulationEventCallback
		{
			void onConstraintBreak(physx::PxConstraintInfo* /*constraints*/, physx::PxU32 /*count*/)	PX_OVERRIDE
			{
				printf("onConstraintBreak\n");
			}

			void onWake(physx::PxActor** /*actors*/, physx::PxU32 /*count*/)	PX_OVERRIDE
			{
				printf("onWake\n");
			}

			void onSleep(physx::PxActor** /*actors*/, physx::PxU32 /*count*/)	PX_OVERRIDE
			{
				printf("onSleep\n");
			}

			void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)	PX_OVERRIDE;


			void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32)	PX_OVERRIDE
			{
				printf("onAdvance\n");
			}

			void onContact(const physx::PxContactPairHeader& /*pairHeader*/, const physx::PxContactPair* pairs, physx::PxU32 count)	PX_OVERRIDE;
		};
	};

	static	physx::PxFilterFlags triggersUsingFilterCallback(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* /*constantBlock*/, physx::PxU32 /*constantBlockSize*/)
	{
		//	printf("contactReportFilterShader\n");

		//PX_ASSERT(getImpl() == FILTER_CALLBACK);
		UINT _left = filterData0.word0;
		UINT _right = filterData1.word0;
		if (_left > _right)
		{
			UINT temp = _left;
			_left = _right;
			_right = temp;
		}
		if (IsCollision(_left, _right))
		{
			pairFlags = physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND 
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS/* | physx::PxPairFlag::eSOLVE_CONTACT*/;
			//if (IsContact(_left, _right))
			if(!physx::PxFilterObjectIsTrigger(attributes0) && !physx::PxFilterObjectIsTrigger(attributes1))
				pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;
		}

		return physx::PxFilterFlag::eCALLBACK;
	}
	/*class ResourceMgr : public Singleton<ResourceMgr>
	{
	private:
		SINGLE(ResourceMgr)
	};*/
}