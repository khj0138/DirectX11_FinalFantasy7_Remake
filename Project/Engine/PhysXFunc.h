#pragma once

namespace ff7r
{
	//class XMFLOAT2;
	//class XMFLOAT3;
	//class XMFLOAT4;
	//class XMVECTOR;
	//class PxQuat;
	/*class PxVec2;
	class PxVec3;
	class PxVec4;
	class PxTransform;
	class PxQuat;*/


	// DirectXMath & PhysX conversions
	void storeXMFLOAT(vec2* _vec, XMFLOAT2* xmfloat);
	XMFLOAT2 loadXMFLOAT(vec2* _vec);
	void storeXMVECTOR(vec2* _vec, XMVECTOR* xmvector);
	XMVECTOR loadXMVECTOR(vec2* _vec);

	void storeXMFLOAT(vec3* _vec, XMFLOAT3* xmfloat);
	XMFLOAT3 loadXMFLOAT(vec3* _vec);
	void storeXMVECTOR(vec3* _vec, XMVECTOR* xmvector);
	XMVECTOR loadXMVECTOR3(vec3* _vec);

	void storeXMFLOAT(vec4* _vec, XMFLOAT4* xmfloat);
	XMFLOAT4 loadXMFLOAT(vec4* _vec);
	void storeXMVECTOR(vec4* _vec, XMVECTOR* xmvector);
	XMVECTOR loadXMVECTOR(vec4* _vec);
	physx::PxVec2 toPxVec2(vec2* _vec);
	physx::PxTransform toPxTransform(vec3* _vec);
	physx::PxVec3 toPxVec3(vec3* _vec);
	physx::PxVec4 toPxVec4(vec4* _vec);
	physx::PxQuat toPxQuat(vec4* _vec);
	vec3 posPxTransform(physx::PxTransform* _transform);
	Quaternion rotPxTransform(physx::PxTransform* _transform);
	void DecomposeEuler(vec3& scale, vec3& rot, vec3& pos, Matrix& mat);
	vec3 rotVector(vec3 _original, vec3 _rot);
}






