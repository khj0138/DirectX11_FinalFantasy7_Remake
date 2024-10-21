#pragma once
#include <PhysX/PxPhysicsAPI.h>
using namespace physx;
namespace ff7r
{
	class CollisionActor {
	public:
		CollisionActor(COLLISION_ACTOR_TYPE actorType);

		COLLISION_ACTOR_TYPE actorType = COLLISION_ACTOR_STATIC;
		PxActor* pActor = NULL;

		void setPosition(vec3 position);
		void setRotation(vec4 rotationQ);
		void setMass(float mass);
		void setVelocity(vec3 velocityVec);

		// Ragdoll
		CollisionActor* parentActor = NULL;
		PxTransform initialTransform;
		PxTransform jointPointParent;
		PxTransform jointPointSelf;
		float boneLength = 0.0f;

		// Pointer for CCT object
		PxController* pCharacterController = NULL;
	};
}