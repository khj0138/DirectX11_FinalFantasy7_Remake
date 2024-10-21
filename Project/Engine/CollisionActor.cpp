#include "pch.h"
#include "CollisionActor.h"
#include "func.h"
#include "PhysXFunc.h"
#include "PhysXMgr.h"

using namespace physx;
namespace ff7r
{

	CollisionActor::CollisionActor(COLLISION_ACTOR_TYPE actorType) {
		this->actorType = actorType;
	}
	void CollisionActor::setPosition(vec3 position) {
		if (
			this->actorType == COLLISION_ACTOR_DYNAMIC ||
			this->actorType == COLLISION_ACTOR_KINEMATIC
			) {
			PxRigidDynamic* rigidDynamic = this->pActor->is<PxRigidDynamic>();

			rigidDynamic->setGlobalPose(
				PxTransform(
					toPxVec3(&position),
					rigidDynamic->getGlobalPose().q
				)
			);
		}
	}

	void CollisionActor::setRotation(vec4 rotationQ) {
		if (
			this->actorType == COLLISION_ACTOR_DYNAMIC ||
			this->actorType == COLLISION_ACTOR_KINEMATIC
			) {
			PxRigidDynamic* rigidDynamic = this->pActor->is<PxRigidDynamic>();

			rigidDynamic->setGlobalPose(
				PxTransform(
					rigidDynamic->getGlobalPose().p,
					toPxQuat(&rotationQ)
				)
			);
		}
	}

	void CollisionActor::setMass(float mass) {
		if (mass <= 0) {
			return;
		}

		if (
			this->actorType == COLLISION_ACTOR_DYNAMIC
			) {
			PxRigidDynamic* rigidDynamic = this->pActor->is<PxRigidDynamic>();

			rigidDynamic->setMass(mass);
		}
	}

	void CollisionActor::setVelocity(vec3 velocityVec) {
		if (
			this->actorType == COLLISION_ACTOR_DYNAMIC ||
			this->actorType == COLLISION_ACTOR_KINEMATIC
			) {
			PxRigidDynamic* rigidDynamic = this->pActor->is<PxRigidDynamic>();

			rigidDynamic->setLinearVelocity(
				toPxVec3(&velocityVec),
				true
			);
		}
	}
}