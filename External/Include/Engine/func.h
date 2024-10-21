#pragma once



namespace ff7r
{
	// 오브젝트 생성
	class GameObject;
	void SpawnGameObject(GameObject* _NewObject, vec3 _vWorldPos, int _LayerIdx);
	void SpawnGameObject(GameObject* _NewObject, vec3 _vWorldPos, const wstring& _LayerName);

	// 오브젝트 삭제
	void DestroyObject(GameObject* _DeletObject);

	// DrawDebugShape
	void DrawDebugRect(vec3 _vWorldPos, vec2 _vWorldScale, vec4 _vColor, vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
	void DrawDebugRect(const Matrix& _matWorld, vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

	void DrawDebugCircle(vec3 _vWorldPos, float _fRadius, vec4 _vColor, vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
	void DrawDebugCircle(const Matrix& _matWorld, vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

	void DrawDebugCube(vec3 _vWorldPos, vec3 _vWorldScale, vec4 _vColor, vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
	void DrawDebugCube(const Matrix& _matWorld, vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

	void DrawDebugSphere(vec3 _vWorldPos, float _fRadius, vec4 _vColor, vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
	void DrawDebugSphere(vec3 _vWorldPos, vec3 _vWorldScale, vec4 _vColor, vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
	void DrawDebugSphere(const Matrix& _matWorld, vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

	void DrawDebugCapsule(vec3 _vWorldPos, vec3 _vWorldScale, vec4 _vColor, vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);


	// GameObject 유효성 체크
	bool IsValidObj(GameObject*& _Target);

	string WStrToStr(const wstring& _wstr);
	wstring StrToWStr(const string& _str);

	const char* ToString(RES_TYPE);
	const wchar_t* ToWSTring(RES_TYPE);

	const char* ToString(COMPONENT_TYPE);
	const wchar_t* ToWSTring(COMPONENT_TYPE);

	// Save / Load
	void SaveWString(const wstring& _str, FILE* _File);
	void LoadWString(wstring& _str, FILE* _File);

	class Resource;
	template<typename T>
	class Ptr;

	void SaveResRef(Ptr<Resource> _Res, FILE* _File);

	vec3 MatrixToRotation(const Matrix& _matrix);

	float Rad2Degree(float _rad);
	float Degree2Rad(float _degree);
	
	vec3 Rad2Degree(vec3 _rad);
	vec3 Degree2Rad(vec3 _degree);

	float CalcYAxisFromVector(vec3 _front, vec3 _target);
	vec3 RotateVec3YAxis(vec3 _vec, float _angle);
	vec3 RotateVec3EulerAngle(vec3 _vec3, vec3 _angle);
	vec3 CalcEulerAngleFromVectors(vec3 _origin, vec3 _target);

	vec3 Slerp(vec3 _start, vec3 _end, float _ratio);

	template<typename T>
	void LoadResRef(Ptr<T>& _Res, FILE* _File);


	template<typename T, UINT Size>
	void SafeDelArray(T* (&arr)[Size])
	{
		for (UINT i = 0; i < Size; ++i)
		{
			if (nullptr != arr[i])
				delete arr[i];
		}
	}

	template<typename T>
	void SafeDelVector(vector<T*>& _vec)
	{
		for (size_t i = 0; i < _vec.size(); ++i)
		{
			if (nullptr != _vec[i])
			{
				delete _vec[i];
			}
		}	
		_vec.clear();
	}

	template<typename T1, typename T2>
	void SafeDelMap(map<T1, T2>& _map)
	{
		for (const auto& pair : _map)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		_map.clear();
	}


}

