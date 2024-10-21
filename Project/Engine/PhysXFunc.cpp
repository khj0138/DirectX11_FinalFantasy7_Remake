//#include "pch.h"
//
//#include <PhysX/PxPhysicsAPI.h>
//using namespace physx;
//
//
//namespace ff7r
//{
//	// DirectXMath & PhysX conversions
//	void storeXMFLOAT(vec2* _vec, XMFLOAT2* xmfloat) {
//		_vec->x = xmfloat->x;
//		_vec->y = xmfloat->y;
//	}
//	XMFLOAT2 loadXMFLOAT(vec2* _vec) {
//		return XMFLOAT2(
//			_vec->x,
//			_vec->y
//		);
//	}
//	void storeXMVECTOR(vec2* _vec, XMVECTOR* xmvector) {
//		XMFLOAT2 xmfloat;
//
//		XMStoreFloat2(
//			&xmfloat,
//			*xmvector
//		);
//
//		storeXMFLOAT(_vec, &xmfloat);
//	}
//	XMVECTOR loadXMVECTOR(vec2* _vec) {
//		return XMVectorSet(
//			_vec->x,
//			_vec->y,
//			0,
//			0
//		);
//	}
//
//	void storeXMFLOAT(vec3* _vec, XMFLOAT3* xmfloat) {
//		_vec->x = xmfloat->x;
//		_vec->y = xmfloat->y;
//		_vec->z = xmfloat->z;
//	}
//	XMFLOAT3 loadXMFLOAT(vec3* _vec) {
//		return XMFLOAT3(
//			_vec->x,
//			_vec->y,
//			_vec->z
//		);
//	}
//	void storeXMVECTOR(vec3* _vec, XMVECTOR* xmvector) {
//		XMFLOAT3 xmfloat;
//
//		XMStoreFloat3(
//			&xmfloat,
//			*xmvector
//		);
//
//		storeXMFLOAT(_vec, &xmfloat);
//	}
//	XMVECTOR loadXMVECTOR3(vec3* _vec) {
//		return XMVectorSet(
//			_vec->x,
//			_vec->y,
//			_vec->z,
//			0
//		);
//	}
//
//
//	void storeXMFLOAT(vec4* _vec, XMFLOAT4* xmfloat) {
//		_vec->x = xmfloat->x;
//		_vec->y = xmfloat->y;
//		_vec->z = xmfloat->z;
//		_vec->w = xmfloat->w;
//	}
//	XMFLOAT4 loadXMFLOAT(vec4* _vec) {
//		return XMFLOAT4(
//			_vec->x,
//			_vec->y,
//			_vec->z,
//			_vec->w
//		);
//	}
//	void storeXMVECTOR(vec4* _vec, XMVECTOR* xmvector) {
//		XMFLOAT4 xmfloat;
//
//		XMStoreFloat4(
//			&xmfloat,
//			*xmvector
//		);
//
//		storeXMFLOAT(_vec, &xmfloat);
//	}
//	XMVECTOR loadXMVECTOR(vec4* _vec) {
//		return XMVectorSet(
//			_vec->x,
//			_vec->y,
//			_vec->z,
//			_vec->w
//		);
//	}
//	PxVec2 toPxVec2(vec2* _vec) {
//		return PxVec2(_vec->x, _vec->y);
//	}
//	PxTransform toPxTransform(vec3* _vec) {
//		PxVec3 _temp = PxVec3(_vec->x, _vec->y, _vec->z);
//		PxTransform _temp2 = PxTransform(_temp);
//		return _temp2;
//	}
//	PxVec3 toPxVec3(vec3* _vec) {
//		return PxVec3(_vec->x, _vec->y, _vec->z);
//	}
//	PxVec4 toPxVec4(vec4* _vec) {
//		return PxVec4(_vec->x, _vec->y, _vec->z, _vec->w);
//	}
//	PxQuat toPxQuat(vec4* _vec) {
//		return PxQuat(_vec->x, _vec->y, _vec->z, _vec->w);
//	}
//}
#include "pch.h"
#include <PhysX/PxPhysicsAPI.h>
#include "PhysXFunc.h"
using namespace physx;


namespace ff7r
{
	// DirectXMath & PhysX conversions
	void storeXMFLOAT(vec2* _vec, XMFLOAT2* xmfloat) {
		_vec->x = xmfloat->x;
		_vec->y = xmfloat->y;
	}
	XMFLOAT2 loadXMFLOAT(vec2* _vec) {
		return XMFLOAT2(
			_vec->x,
			_vec->y
		);
	}
	void storeXMVECTOR(vec2* _vec, XMVECTOR* xmvector) {
		XMFLOAT2 xmfloat;

		XMStoreFloat2(
			&xmfloat,
			*xmvector
		);

		storeXMFLOAT(_vec, &xmfloat);
	}
	XMVECTOR loadXMVECTOR(vec2* _vec) {
		return XMVectorSet(
			_vec->x,
			_vec->y,
			0,
			0
		);
	}

	void storeXMFLOAT(vec3* _vec, XMFLOAT3* xmfloat) {
		_vec->x = xmfloat->x;
		_vec->y = xmfloat->y;
		_vec->z = xmfloat->z;
	}
	XMFLOAT3 loadXMFLOAT(vec3* _vec) {
		return XMFLOAT3(
			_vec->x,
			_vec->y,
			_vec->z
		);
	}
	void storeXMVECTOR(vec3* _vec, XMVECTOR* xmvector) {
		XMFLOAT3 xmfloat;

		XMStoreFloat3(
			&xmfloat,
			*xmvector
		);

		storeXMFLOAT(_vec, &xmfloat);
	}
	XMVECTOR loadXMVECTOR3(vec3* _vec) {
		return XMVectorSet(
			_vec->x,
			_vec->y,
			_vec->z,
			0
		);
	}


	void storeXMFLOAT(vec4* _vec, XMFLOAT4* xmfloat) {
		_vec->x = xmfloat->x;
		_vec->y = xmfloat->y;
		_vec->z = xmfloat->z;
		_vec->w = xmfloat->w;
	}
	XMFLOAT4 loadXMFLOAT(vec4* _vec) {
		return XMFLOAT4(
			_vec->x,
			_vec->y,
			_vec->z,
			_vec->w
		);
	}
	void storeXMVECTOR(vec4* _vec, XMVECTOR* xmvector) {
		XMFLOAT4 xmfloat;

		XMStoreFloat4(
			&xmfloat,
			*xmvector
		);

		storeXMFLOAT(_vec, &xmfloat);
	}
	XMVECTOR loadXMVECTOR(vec4* _vec) {
		return XMVectorSet(
			_vec->x,
			_vec->y,
			_vec->z,
			_vec->w
		);
	}
	PxVec2 toPxVec2(vec2* _vec) {
		return PxVec2(_vec->x, _vec->y);
	}
	PxTransform toPxTransform(vec3* _vec) {
		PxVec3 _temp = PxVec3(_vec->x, _vec->y, _vec->z);
		PxTransform _temp2 = PxTransform(_temp);
		return _temp2;
	}
	vec3 posPxTransform(PxTransform* _transform) {
		PxVec3 _temp = _transform->p;
		vec3 _temp2 = vec3(_temp.x, _temp.y, _temp.z);
		return _temp2;
	}
	Quaternion rotPxTransform(PxTransform* _transform) {
		PxQuat _temp = _transform->q;
		Quaternion _temp2 = Quaternion(_temp.x, _temp.y, _temp.z, _temp.w);
		return _temp2;
	}
	PxVec3 toPxVec3(vec3* _vec) {
		return PxVec3(_vec->x, _vec->y, _vec->z);
	}
	PxVec4 toPxVec4(vec4* _vec) {
		return PxVec4(_vec->x, _vec->y, _vec->z, _vec->w);
	}
	PxQuat toPxQuat(vec4* _vec) {
		return PxQuat(_vec->x, _vec->y, _vec->z, _vec->w);
	}
	void DecomposeEuler(vec3& scale, vec3& rot, vec3& pos, Matrix& mat)
	{

		vec3 _right = vec3(mat._11, mat._12, mat._13);
		vec3 _up = vec3(mat._21, mat._22, mat._23);
		vec3 _dir = vec3(mat._31, mat._32, mat._33);

		scale.x = _right.Length();
		scale.y = _up.Length();
		scale.z = _dir.Length();

		_right.Normalize();
		_up.Normalize();
		_dir.Normalize();


		rot.x = atan2f(mat._23, mat._33) / XM_PI * 180.0f;
		rot.y = atan2f(-mat._13, sqrtf(mat._23 * mat._23 + mat._33 * mat._33)) / XM_PI * 180.0f;

		rot.z = atan2f(mat._12, mat._11) / XM_PI * 180.0f;

		pos = vec3(mat._41, mat._42, mat._43);

	}
	vec3 rotVector(vec3 _original, vec3 _rot)
	{
		vec3 result;
		Matrix _mat_original = XMMatrixTranslation(_original.x, _original.y, _original.z);

		vec3 _mat_rotation = _rot;
		Matrix _mat_rotX = XMMatrixRotationX(_mat_rotation.x);
		Matrix _mat_rotY = XMMatrixRotationY(_mat_rotation.y);
		Matrix _mat_rotZ = XMMatrixRotationZ(_mat_rotation.z);
		_mat_original *= _mat_rotX;
		_mat_original *= _mat_rotY;
		_mat_original *= _mat_rotZ;
		//_pos.y = _pos.y - _collider->GetOffsetPos().y;
		//_pos.x = _pos.x - _collider->GetOffsetPos().x;
		//_pos.z = _pos.z - _collider->GetOffsetPos().z;
		result.x = _mat_original.m[3][0];
		result.y = _mat_original.m[3][1];
		result.z = _mat_original.m[3][2];
		return result;
	}
}