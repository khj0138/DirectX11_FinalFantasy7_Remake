#include "pch.h"

#include "EventMgr.h"
#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderMgr.h"
#include "ptr.h"
#include "ResourceMgr.h"
//#include "PhysXMgr.h"



namespace ff7r
{
	void SpawnGameObject(GameObject* _NewObject, vec3 _vWorldPos, int _LayerIdx)
	{
		_NewObject->GetTransform()->SetRelativePos(_vWorldPos);

		Event evn = {};

		evn.type = EVENT_TYPE::CREATE_OBJECT;
		evn.w_param = (DWORD_PTR)_NewObject;
		evn.l_param = _LayerIdx;

		EventMgr::GetInst()->AddEvent(evn);
	}

	void SpawnGameObject(GameObject* _NewObject, vec3 _vWorldPos, const wstring& _LayerName)
	{
		_NewObject->GetTransform()->SetRelativePos(_vWorldPos);

		Event evn = {};

		evn.type = EVENT_TYPE::CREATE_OBJECT;
		evn.w_param = (DWORD_PTR)_NewObject;
		evn.l_param = LevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_LayerName)->GetLayerIndex();

		EventMgr::GetInst()->AddEvent(evn);
	}

	void DestroyObject(GameObject* _DeletObject)
	{
		if (_DeletObject->IsDead())
			return;

		Event evn = {};

		evn.type = EVENT_TYPE::DELETE_OBJECT;
		evn.w_param = (DWORD_PTR)_DeletObject;

		EventMgr::GetInst()->AddEvent(evn);
	}



	void DrawDebugRect(vec3 _vWorldPos, vec2 _vWorldScale, vec4 _vColor
		, vec3 _vRotation, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.shape = SHAPE_TYPE::RECT;
		info.max_time = _fTime;
		info.world_pos = _vWorldPos;
		info.world_scale = vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
		info.world_rotation = _vRotation;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugRect(const Matrix& _matWorld, vec4 _vColor, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = _matWorld;
		info.shape = SHAPE_TYPE::RECT;
		info.max_time = _fTime;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCircle(vec3 _vWorldPos, float _fRadius, vec4 _vColor, vec3 _vRotation
		, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.shape = SHAPE_TYPE::CIRCLE;
		info.max_time = _fTime;
		info.world_pos = _vWorldPos;
		info.world_scale = vec3(_fRadius, _fRadius, 1.f);
		info.world_rotation = _vRotation;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCircle(const Matrix& _matWorld, vec4 _vColor, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = _matWorld;
		info.shape = SHAPE_TYPE::CIRCLE;
		info.max_time = _fTime;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCube(vec3 _vWorldPos, vec3 _vWorldScale, vec4 _vColor
		, vec3 _vRotation, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.shape = SHAPE_TYPE::CUBE;
		info.max_time = _fTime;
		info.world_pos = _vWorldPos;
		info.world_scale = vec3(_vWorldScale.x, _vWorldScale.y, _vWorldScale.z);
		info.world_rotation = _vRotation;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCube(const Matrix& _matWorld, vec4 _vColor, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = _matWorld;
		info.shape = SHAPE_TYPE::CUBE;
		info.max_time = _fTime;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugSphere(vec3 _vWorldPos, float _fRadius, vec4 _vColor
		, vec3 _vRotation, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.shape = SHAPE_TYPE::SPHERE;
		info.max_time = _fTime;
		info.world_pos = _vWorldPos;
		info.world_scale = vec3(_fRadius, _fRadius, _fRadius);
		info.world_rotation = _vRotation;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugSphere(vec3 _vWorldPos, vec3 _vWorldScale, vec4 _vColor
		, vec3 _vRotation, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.shape = SHAPE_TYPE::SPHERE;
		info.max_time = _fTime;
		info.world_pos = _vWorldPos;
		info.world_scale = vec3(_vWorldScale.x, _vWorldScale.y, _vWorldScale.z);
		info.world_rotation = _vRotation;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugSphere(const Matrix& _matWorld, vec4 _vColor, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = _matWorld;
		info.shape = SHAPE_TYPE::SPHERE;
		info.max_time = _fTime;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCapsule(vec3 _vWorldPos, vec3 _vWorldScale, vec4 _vColor, vec3 _vRotation, float _fTime, bool DepthTest)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.shape = SHAPE_TYPE::CAPSULE;
		info.max_time = _fTime;
		info.world_pos = _vWorldPos;
		info.world_scale = vec3(_vWorldScale.x, _vWorldScale.y, _vWorldScale.z);
		info.world_rotation = _vRotation;
		info.color = _vColor;
		info.depth_test = DepthTest;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	vec3 MatrixToRotation(const Matrix& _matrix)
	{
		Matrix _temp = _matrix;
		vec3 _rotation;
		_rotation[0] = atan2f(_matrix.m[1][2], _matrix.m[2][2]);
		_rotation[1] = atan2f(-_matrix.m[0][2], sqrtf(_matrix.m[1][2] * _matrix.m[1][2] + _matrix.m[2][2] * _matrix.m[2][2]));
		_rotation[2] = atan2f(_matrix.m[0][1], _matrix.m[0][0]);
		return _rotation;
	}

	float Rad2Degree(float _rad)
	{
		return _rad * 57.29578f; //180.f / 3.141592654f;
	}

	float Degree2Rad(float _degree)
	{
		return _degree * 3.141592654f / 180.f;
	}

	vec3 Rad2Degree(vec3 _rad)
	{
		return vec3(Rad2Degree(_rad.x), Rad2Degree(_rad.y), Rad2Degree(_rad.z));
	}

	vec3 Degree2Rad(vec3 _degree)
	{
		return vec3(Degree2Rad(_degree.x), Degree2Rad(_degree.y), Degree2Rad(_degree.z));
	}

	float CalcYAxisFromVector(vec3 _front, vec3 _target)
	{
		_front.y = 0.0f;
		_target.y = 0.0f;

		_front.Normalize();
		_target.Normalize();
	
		float _dot = XMVectorGetX(XMVector3Dot(_front, _target));
		_dot = MAX(-1.0f, MIN(1.0f, _dot));
	
		vec3 _cross = XMVector3Cross(_front, _target);
	
		float _angle = acos(_dot);
		if (_cross.y < 0)
		{
			_angle = -_angle;
		}
	
		return _angle;
	}

	vec3 RotateVec3YAxis(vec3 _vec, float _angle)
	{
		float _cos = cos(_angle);
		float _sin = sin(_angle);

		return vec3
		{
			_vec.x * _cos + _vec.z * _sin,
			_vec.y,
			_vec.z * _cos - _vec.x * _sin
		};
	}

	vec3 RotateVec3EulerAngle(vec3 _vec3, vec3 _angle)
	{
		XMMATRIX rotX = XMMatrixRotationX(_angle.x);
		XMMATRIX rotY = XMMatrixRotationY(_angle.y);
		XMMATRIX rotZ = XMMatrixRotationZ(_angle.z);

		XMMATRIX rotationMatrix = rotX * rotY * rotZ;
		vec3 _rotate_pos = XMVector3Transform(_vec3, rotationMatrix);

		return _rotate_pos;
	}

	vec3 CalcEulerAngleFromVectors(vec3 _origin, vec3 _target)
	{
		if (_origin.Length() == 0.0f)
		{
			return vec3(0.f, 0.f, 0.f);
		}

		_origin.Normalize();
		_target.Normalize();

		//Quaternion rotationQuat = Quaternion::CreateFromRotationMatrix(Matrix::CreateLookAt(vec3(0.f, 0.f, 0.f), _target - _origin, vec3(0.f, 1.f, 0.f)));

		//// 쿼터니언을 회전 행렬로 변환
		//Matrix rotationMatrix = Matrix::CreateFromQuaternion(rotationQuat);

		//// 회전 행렬에서 오일러 각 추출
		//Vector3 eulerAngles = MatrixToRotation(rotationMatrix);

		//return eulerAngles; // 오일러 각 반환

		vec3 rotationAxis = XMVector3Cross(_origin, _target);
		float dotProduct = XMVectorGetX(XMVector3Dot(_origin, _target));
		float angle = acos(dotProduct);

		XMVECTOR rotationQuaternion = XMQuaternionRotationAxis(rotationAxis, angle);
		XMFLOAT4 quaternion;
		XMStoreFloat4(&quaternion, rotationQuaternion);

		Matrix mat = {};
		mat = mat.CreateFromQuaternion(quaternion);

		vec3 rot;
		rot.x = atan2f(mat._23, mat._33);
		rot.y = atan2f(-mat._13, sqrtf(mat._23 * mat._23 + mat._33 * mat._33));
		rot.z = atan2f(mat._12, mat._11);

		return rot;
	}

	// 먼가 이상함 안쓰는 중
	vec3 Slerp(vec3 _start, vec3 _end, float _ratio)
	{
		_start = _start.Normalize();
		_end = _end.Normalize();

		// 두 벡터 사이의 각도를 계산합니다.
		float dot = _start.Dot(_end);
		dot = MAX(-1.0f, MIN(1.0f, dot));
		float theta = acos(dot) * _ratio;

		vec3 relativeVec = (_end - _start * dot).Normalize(); // 회전 축 방향을 찾습니다.
		return _start * cos(theta) + relativeVec * sin(theta);
	}

	bool IsValidObj(GameObject*& _Target)
	{
		if (nullptr == _Target)
			return false;

		if (_Target->IsDead())
		{
			_Target = nullptr;
			return false;
		}

		return true;
	}

	string WStrToStr(const wstring& _wstr)
	{
		string _str = string(_wstr.begin(), _wstr.end());

		return _str;
	}

	wstring StrToWStr(const string& _str)
	{
		wstring _wstr = wstring(_str.begin(), _str.end());

		return _wstr;
	}

	const char* ToString(RES_TYPE type)
	{
		return RES_TYPE_STR[(UINT)type];
	}

	const wchar_t* ToWString(RES_TYPE type)
	{
		return RES_TYPE_WSTR[(UINT)type];
	}


	const char* ToString(COMPONENT_TYPE type)
	{
		return COMPONENT_TYPE_STR[(UINT)type];
	}

	void SaveWString(const wstring& _str, FILE* _File)
	{
		UINT iLen = (UINT)_str.length();
		fwrite(&iLen, sizeof(UINT), 1, _File);
		fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
	}

	void LoadWString(wstring& _str, FILE* _File)
	{
		wchar_t szBuffer[256] = {};

		UINT iLen = 0;
		fread(&iLen, sizeof(UINT), 1, _File);
		fread(szBuffer, sizeof(wchar_t), iLen, _File);

		_str = szBuffer;
	}

	void SaveResRef(Ptr<Resource> _Res, FILE* _File)
	{
		int i = 0;
		if (nullptr == _Res)
		{
			fwrite(&i, sizeof(i), 1, _File);
		}
		else
		{
			i = 1;
			fwrite(&i, sizeof(i), 1, _File);
			if (_Res->GetKey() == L"SphereMesh")
			{
				SaveWString(_Res->GetKey(), _File);
				SaveWString(_Res->GetRelativePath(), _File);
			}
			else if (_Res->GetKey() == L"SkyBoxMtrl")
			{
				SaveWString(_Res->GetKey(), _File);
				SaveWString(_Res->GetRelativePath(), _File);
			}
			else
			{
			SaveWString( _Res->GetKey(), _File);
			SaveWString( _Res->GetRelativePath(), _File);

			}
		}
	}

	template<typename T>
	void LoadResRef(Ptr<T>& _Res, FILE* _File)
	{
		int i = 0;
		fread(&i, sizeof(i), 1, _File);

		if (i)
		{
			wstring strKey, strRelativePath;
			LoadWString(strKey, _File);
			LoadWString(strRelativePath, _File);

			_Res = ResourceMgr::GetInst()->MultiLoad<T>(strKey, strRelativePath);
		}
	}
	template void LoadResRef<Texture>(Ptr<Texture>& _res, FILE* _file);
	template void LoadResRef<Mesh>(Ptr<Mesh>& _res, FILE* _file);
	template void LoadResRef<Material>(Ptr<Material>& _res, FILE* _file);
	template void LoadResRef<GraphicsShader>(Ptr<GraphicsShader>& _res, FILE* _file);
	template void LoadResRef<Prefab>(Ptr<Prefab>& _res, FILE* _file);
	template void LoadResRef<Sound>(Ptr<Sound>& _res, FILE* _file);
	template void LoadResRef<Animation3D>(Ptr<Animation3D>& _res, FILE* _file);
	template void LoadResRef<MeshData>(Ptr<MeshData>& _res, FILE* _file);
	template void LoadResRef<AnimatorController>(Ptr<AnimatorController>& _res, FILE* _file);

	const wchar_t* ToWString(COMPONENT_TYPE type)
	{
		return COMPONENT_TYPE_WSTR[(UINT)type];
	}


}
