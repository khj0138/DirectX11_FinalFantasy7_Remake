#include "pch.h"
#include "ResourceMgr.h"
#include "PathMgr.h"

#include "Engine.h"
#include "SetColorShader.h"
#include "ParticleShader.h"
#include "Animation3DShader.h"
#include "BlurShader.h"
#include "CopyBoneShader.h"

// ====Test=====
#include "Device.h"

namespace ff7r
{
	void ResourceMgr::CreateDefaultMesh()
	{
		vector<Vtx> _vertex;
		vector<UINT> _index;
		Vtx _v;


		Ptr<Mesh> _mesh = nullptr;

		// ==============
		// PointMesh 생성
		// ==============
		_v.pos = vec3(0.f, 0.f, 0.f);
		_v.uv = vec2(0.f, 0.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);

		UINT _idx = 0;

		_mesh = new Mesh(true);
		_mesh->Create(&_v, 1, &_idx, 1);
		AddRes(L"PointMesh", _mesh);


		// =============
		// RectMesh 생성
		// =============
		// 0 --- 1 
		// |  \  |
		// 3 --- 2
		// 3 --- 2 
		// |  \  |
		// 0 --  1
		_v.pos = vec3(-0.5f, 0.5f, 0.f);
		_v.color = vec4(1.f, 0.f, 0.f, 1.f);

		_v.uv = vec2(0.f, 0.f);

		_v.normal = vec3(0.f, 0.f, -1.f);
		_v.tangent = vec3(1.f, 0.f, 0.f);
		_v.binormal = vec3(0.f, -1.f, 0.f);

		_vertex.push_back(_v);

		_v.pos = vec3(0.5f, 0.5f, 0.f);
		_v.color = vec4(0.f, 1.f, 0.f, 1.f);
		_v.uv = vec2(1.f, 0.f);

		_vertex.push_back(_v);

		_v.pos = vec3(0.5f, -0.5f, 0.f);
		_v.color = vec4(0.f, 0.f, 1.f, 1.f);
		_v.uv = vec2(1.f, 1.f);

		_vertex.push_back(_v);

		_v.pos = vec3(-0.5f, -0.5f, 0.f);
		_v.color = vec4(0.f, 0.f, 0.f, 1.f);
		_v.uv = vec2(0.f, 1.f);
		_vertex.push_back(_v);

		_index.push_back(3); 
		_index.push_back(1);
		_index.push_back(0);

		_index.push_back(1);
		_index.push_back(3);
		_index.push_back(2);

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes(L"RectMesh", _mesh);

		_index.clear();
		_index.push_back(0);
		_index.push_back(1);
		_index.push_back(2);
		_index.push_back(3);
		_index.push_back(0);

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes(L"RectMesh_Debug", _mesh);

		_vertex.clear();
		_index.clear();


		for (int i = 0; i < 42; i++)
		{
			Vtx _v;

			_vertex.push_back(_v);
		}

		for (int i = 0; i < 40; i+=2)
		{
			_index.push_back(i);
			_index.push_back(i + 1);
			_index.push_back(i + 2);

			_index.push_back(i + 2);
			_index.push_back(i + 1);
			_index.push_back(i + 3);
		}
		
		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size(), true);
		AddRes(L"TrailMesh", _mesh);

		_vertex.clear();
		_index.clear();

		// ===========
		// Circle Mesh
		// ===========
		//      3 
		//   4     2
		//  5 --0-- 1	
		// 반지름
		float fRadius = 0.5f;

		// 각도
		UINT Slice = 40;
		float fTheta = XM_2PI / (float)Slice;

		// 중심점
		_v.pos = vec3(0.f, 0.f, 0.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);
		_v.uv = vec2(0.5f, 0.5f);

		_v.normal = vec3(0.f, 0.f, -1.f);
		_v.tangent = vec3(1.f, 0.f, 0.f);
		_v.binormal = vec3(0.f, -1.f, 0.f);

		_vertex.push_back(_v);

		// 정점 위치 지정
		for (UINT i = 0; i < Slice; ++i)
		{
			_v.pos = vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
			_v.uv = vec2(_v.pos.x + 0.5f, -_v.pos.y + 0.5f);
			_vertex.push_back(_v);
		}

		// 인덱스 설정
		for (UINT i = 0; i < Slice - 1; ++i)
		{
			_index.push_back(0);
			_index.push_back(i + 2);
			_index.push_back(i + 1);
		}

		// 마지막 삼각형
		_index.push_back(0);
		_index.push_back(1);
		_index.push_back(Slice);

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes(L"CircleMesh", _mesh);

		_index.clear();
		for (UINT i = 0; i < Slice; ++i)
		{
			_index.push_back(i + 1);
		}
		_index.push_back(1);

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes(L"CircleMesh_Debug", _mesh);

		_vertex.clear();
		_index.clear();


		float _radius = 0.5f;

		// 각도
		UINT _slice = 40;
		float _theta = XM_2PI / (float)_slice;

		// 중심점
		_v.pos = vec3(0.f, 0.f, 0.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);
		//_v.uv = vec2(0.5f, 0.5f);

		// 정점 위치 지정
		for (UINT i = 0; i < _slice; ++i)
		{
			_v.pos = vec3(_radius * cosf(_theta * (float)i), _radius * sinf(_theta * (float)i), 0.f);
			_vertex.push_back(_v);
		}

		for (UINT i = 0; i < _slice / 4 * 3; ++i)
		{
			_v.pos = vec3(_radius * cosf(_theta * (float)i), 0, _radius * sinf(_theta * (float)i));
			_vertex.push_back(_v);
		}

		for (UINT i = 0; i < _slice; ++i)
		{
			_v.pos = vec3(0.0f, _radius * sinf(_theta * (float)i + XM_PI - 0.0001f), _radius * cosf(_theta * (float)i + XM_PI - 0.0001f));
			_vertex.push_back(_v);
		}
			
		for (UINT i = _slice / 4 * 3; i < _slice; ++i)
		{
			_v.pos = vec3(_radius * cosf(_theta * (float)i), 0, _radius * sinf(_theta * (float)i));
			_vertex.push_back(_v);
		}

		for (int i = 0; i < _vertex.size(); i++)
		{
			_index.push_back(i);
		}

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes(L"DebugSphereMesh", _mesh);

		_vertex.clear();
		_index.clear();

		// ========
		// CubeMesh
		// ========
		Vtx arrCube[24] = {};

		// 윗면
		arrCube[0].pos = vec3(-0.5f, 0.5f, 0.5f);
		arrCube[0].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[0].uv = vec2(0.f, 0.f);
		arrCube[0].normal = vec3(0.f, 1.f, 0.f);

		arrCube[1].pos = vec3(0.5f, 0.5f, 0.5f);
		arrCube[1].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[1].uv = vec2(0.f, 0.f);
		arrCube[1].normal = vec3(0.f, 1.f, 0.f);

		arrCube[2].pos = vec3(0.5f, 0.5f, -0.5f);
		arrCube[2].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[2].uv = vec2(0.f, 0.f);
		arrCube[2].normal = vec3(0.f, 1.f, 0.f);

		arrCube[3].pos = vec3(-0.5f, 0.5f, -0.5f);
		arrCube[3].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[3].uv = vec2(0.f, 0.f);
		arrCube[3].normal = vec3(0.f, 1.f, 0.f);


		// 아랫 면	
		arrCube[4].pos = vec3(-0.5f, -0.5f, -0.5f);
		arrCube[4].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[4].uv = vec2(0.f, 0.f);
		arrCube[4].normal = vec3(0.f, -1.f, 0.f);

		arrCube[5].pos = vec3(0.5f, -0.5f, -0.5f);
		arrCube[5].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[5].uv = vec2(0.f, 0.f);
		arrCube[5].normal = vec3(0.f, -1.f, 0.f);

		arrCube[6].pos = vec3(0.5f, -0.5f, 0.5f);
		arrCube[6].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[6].uv = vec2(0.f, 0.f);
		arrCube[6].normal = vec3(0.f, -1.f, 0.f);

		arrCube[7].pos = vec3(-0.5f, -0.5f, 0.5f);
		arrCube[7].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[7].uv = vec2(0.f, 0.f);
		arrCube[7].normal = vec3(0.f, -1.f, 0.f);

		// 왼쪽 면
		arrCube[8].pos = vec3(-0.5f, 0.5f, 0.5f);
		arrCube[8].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[8].uv = vec2(0.f, 0.f);
		arrCube[8].normal = vec3(-1.f, 0.f, 0.f);

		arrCube[9].pos = vec3(-0.5f, 0.5f, -0.5f);
		arrCube[9].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[9].uv = vec2(0.f, 0.f);
		arrCube[9].normal = vec3(-1.f, 0.f, 0.f);

		arrCube[10].pos = vec3(-0.5f, -0.5f, -0.5f);
		arrCube[10].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[10].uv = vec2(0.f, 0.f);
		arrCube[10].normal = vec3(-1.f, 0.f, 0.f);

		arrCube[11].pos = vec3(-0.5f, -0.5f, 0.5f);
		arrCube[11].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[11].uv = vec2(0.f, 0.f);
		arrCube[11].normal = vec3(-1.f, 0.f, 0.f);

		// 오른쪽 면
		arrCube[12].pos = vec3(0.5f, 0.5f, -0.5f);
		arrCube[12].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[12].uv = vec2(0.f, 0.f);
		arrCube[12].normal = vec3(1.f, 0.f, 0.f);

		arrCube[13].pos = vec3(0.5f, 0.5f, 0.5f);
		arrCube[13].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[13].uv = vec2(0.f, 0.f);
		arrCube[13].normal = vec3(1.f, 0.f, 0.f);

		arrCube[14].pos = vec3(0.5f, -0.5f, 0.5f);
		arrCube[14].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[14].uv = vec2(0.f, 0.f);
		arrCube[14].normal = vec3(1.f, 0.f, 0.f);

		arrCube[15].pos = vec3(0.5f, -0.5f, -0.5f);
		arrCube[15].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[15].uv = vec2(0.f, 0.f);
		arrCube[15].normal = vec3(1.f, 0.f, 0.f);

		// 뒷 면
		arrCube[16].pos = vec3(0.5f, 0.5f, 0.5f);
		arrCube[16].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[16].uv = vec2(0.f, 0.f);
		arrCube[16].normal = vec3(0.f, 0.f, 1.f);

		arrCube[17].pos = vec3(-0.5f, 0.5f, 0.5f);
		arrCube[17].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[17].uv = vec2(0.f, 0.f);
		arrCube[17].normal = vec3(0.f, 0.f, 1.f);

		arrCube[18].pos = vec3(-0.5f, -0.5f, 0.5f);
		arrCube[18].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[18].uv = vec2(0.f, 0.f);
		arrCube[18].normal = vec3(0.f, 0.f, 1.f);

		arrCube[19].pos = vec3(0.5f, -0.5f, 0.5f);
		arrCube[19].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[19].uv = vec2(0.f, 0.f);
		arrCube[19].normal = vec3(0.f, 0.f, 1.f);

		// 앞 면
		arrCube[20].pos = vec3(-0.5f, 0.5f, -0.5f);;
		arrCube[20].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[20].uv = vec2(0.f, 0.f);
		arrCube[20].normal = vec3(0.f, 0.f, -1.f);

		arrCube[21].pos = vec3(0.5f, 0.5f, -0.5f);
		arrCube[21].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[21].uv = vec2(0.f, 0.f);
		arrCube[21].normal = vec3(0.f, 0.f, -1.f);

		arrCube[22].pos = vec3(0.5f, -0.5f, -0.5f);
		arrCube[22].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[22].uv = vec2(0.f, 0.f);
		arrCube[22].normal = vec3(0.f, 0.f, -1.f);

		arrCube[23].pos = vec3(-0.5f, -0.5f, -0.5f);
		arrCube[23].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[23].uv = vec2(0.f, 0.f);
		arrCube[23].normal = vec3(0.f, 0.f, -1.f);

		// 인덱스
		for (int i = 0; i < 12; i += 2)
		{
			_index.push_back(i * 2);
			_index.push_back(i * 2 + 1);
			_index.push_back(i * 2 + 2);

			_index.push_back(i * 2);
			_index.push_back(i * 2 + 2);
			_index.push_back(i * 2 + 3);
		}

		_mesh = new Mesh(true);
		_mesh->Create(arrCube, 24, _index.data(), (UINT)_index.size());
		AddRes<Mesh>(L"CubeMesh", _mesh);
		_index.clear();


		_mesh = new Mesh(true);
		_index.push_back(0); _index.push_back(1); _index.push_back(2); _index.push_back(3); _index.push_back(0);
		_index.push_back(7); _index.push_back(6); _index.push_back(1); _index.push_back(2); _index.push_back(5);
		_index.push_back(6); _index.push_back(7); _index.push_back(4); _index.push_back(3); _index.push_back(2);
		_index.push_back(5); _index.push_back(4);

		_mesh->Create(arrCube, 24, _index.data(), (UINT)_index.size());
		AddRes<Mesh>(L"CubeMesh_Debug", _mesh);
		_index.clear();





		// ===========
		// Sphere Mesh
		// ===========
		fRadius = 0.5f;

		// Top
		_v.pos = vec3(0.f, fRadius, 0.f);
		_v.uv = vec2(0.5f, 0.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);
		_v.normal = _v.pos;
		_v.normal.Normalize();
		_v.tangent = vec3(1.f, 0.f, 0.f);
		_v.binormal = vec3(0.f, 0.f, -1.f);
		_vertex.push_back(_v);

		// Body
		UINT iStackCount = 40; // 가로 분할 개수
		UINT iSliceCount = 40; // 세로 분할 개수

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				_v.pos = vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

				_v.uv = vec2(fUVXStep * j, fUVYStep * i);
				_v.color = vec4(1.f, 1.f, 1.f, 1.f);
				_v.normal = _v.pos;
				_v.normal.Normalize();

				_v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				_v.tangent.y = 0.f;
				_v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				_v.tangent.Normalize();

				_v.normal.Cross(_v.tangent, _v.binormal);
				_v.binormal.Normalize();

				_vertex.push_back(_v);
			}
		}

		// Bottom
		_v.pos = vec3(0.f, -fRadius, 0.f);
		_v.uv = vec2(0.5f, 1.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);
		_v.normal = _v.pos;
		_v.normal.Normalize();

		_v.tangent = vec3(1.f, 0.f, 0.f);
		_v.binormal = vec3(0.f, 0.f, -1.f);
		_vertex.push_back(_v);

		// 인덱스
		// 북극점
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			_index.push_back(0);
			_index.push_back(i + 2);
			_index.push_back(i + 1);
		}

		// 몸통
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				_index.push_back((iSliceCount + 1) * (i)+(j)+1);
				_index.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				_index.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				_index.push_back((iSliceCount + 1) * (i)+(j)+1);
				_index.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				_index.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// 남극점
		UINT iBottomIdx = (UINT)_vertex.size() - 1;
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			_index.push_back(iBottomIdx);
			_index.push_back(iBottomIdx - (i + 2));
			_index.push_back(iBottomIdx - (i + 1));
		}

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes<Mesh>(L"SphereMesh", _mesh);
		_vertex.clear();
		_index.clear();


		// ===========
		// Capsule Mesh
		// ===========
		fRadius = 0.25f;

		// Top
		_v.pos = vec3(0.5f, 0.f,  0.f);
		_v.uv = vec2(0.5f, 0.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);
		_v.normal = _v.pos;
		_v.normal.Normalize();
		_v.tangent = vec3(1.f, 0.f, 0.f);
		_v.binormal = vec3(0.f, 0.f, -1.f);
		_vertex.push_back(_v);

		// Body
		iStackCount = 10; // 가로 분할 개수
		iSliceCount = 10; // 세로 분할 개수

		fStackAngle = XM_PI / iStackCount;
		fSliceAngle = XM_2PI / iSliceCount;

		fUVXStep = 1.f / (float)iSliceCount;
		fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			UINT _temp_i = i;
			float _weight = 0.25f;
			if (i >= 5)
			{
				_weight *= -1.0f;
			}
			/*if (i < 10)
			{
				_temp_i *= 2;
				_weight = 0.25f;
			}
			else if (i >= 10 && i <= 30)
			{
				_temp_i = 20;
			}
			else
			{
				_temp_i *= 2;
				_temp_i -= 40;
				_weight = -0.25f;
			}*/

			float phi = _temp_i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				UINT _temp_j = j;

				_v.pos = vec3(
					fRadius * cosf(_temp_i * fStackAngle) + _weight,
					fRadius * sinf(_temp_i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * sinf(_temp_i * fStackAngle) * sinf(j * fSliceAngle));

				_v.uv = vec2(fUVXStep * j, fUVYStep * _temp_i);
				_v.color = vec4(1.f, 1.f, 1.f, 1.f);
				_v.normal = _v.pos;
				_v.normal.Normalize();
				_v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				_v.tangent.y = 0.f;
				_v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				_v.tangent.Normalize();

				_v.normal.Cross(_v.tangent, _v.binormal);
				_v.binormal.Normalize();

				_vertex.push_back(_v);
			}
		}

		// Bottom
		_v.pos = vec3(-0.5f, 0.0f,  0.0f);
		_v.uv = vec2(0.5f, 1.f);
		_v.color = vec4(1.f, 1.f, 1.f, 1.f);
		_v.normal = _v.pos;
		_v.normal.Normalize();

		_v.tangent = vec3(1.f, 0.f, 0.f);
		_v.binormal = vec3(0.f, 0.f, -1.f);
		_vertex.push_back(_v);

		// 인덱스
		// 북극점
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			_index.push_back(0);
			_index.push_back(i + 2);
			_index.push_back(i + 1);
		}

		// 몸통
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				_index.push_back((iSliceCount + 1) * (i)+(j)+1);
				_index.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				_index.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				_index.push_back((iSliceCount + 1) * (i)+(j)+1);
				_index.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				_index.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// 남극점
		iBottomIdx = (UINT)_vertex.size() - 1;
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			_index.push_back(iBottomIdx);
			_index.push_back(iBottomIdx - (i + 2));
			_index.push_back(iBottomIdx - (i + 1));
		}

		_mesh = new Mesh(true);

		//XMMATRIX rotX = XMMatrixRotationX(0.f);
		//XMMATRIX rotY = XMMatrixRotationY(0.f);
		//XMMATRIX rotZ = XMMatrixRotationZ(90.f * 3.141592f / 180.f);
		//XMMATRIX rotationMatrix = rotX * rotY * rotZ;

		//for (auto& vtx : _vertex)
		//{
		//	vtx.pos = XMVector3Transform(vtx.pos, rotationMatrix);
		//}

		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size());
		AddRes<Mesh>(L"CapsuleMesh", _mesh);
		_vertex.clear();
		_index.clear();
	}

	void ResourceMgr::CreateDefaultGraphicsShader()
	{
		Ptr<GraphicsShader> _shader = nullptr;

		// ============================
		// SkyBoxShader
		// RS_TYPE : CULL_FRONT
		// DS_TYPE : LESS
		// BS_TYPE : DEFAULT
		// Domain : MASK
		// ============================
		_shader = new GraphicsShader;
		_shader->SetKey(L"SkyBoxShader");

		_shader->CreateVertexShader(L"shader\\SkyBox_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\SkyBox_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_FRONT);
		_shader->SetDSType(DS_TYPE::LESS_EQUAL);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

		// Parameter	
		_shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(_shader->GetKey(), _shader);
		// ==================================================================
		_shader = new GraphicsShader;
		_shader->SetKey(L"BillboardShader");

		_shader->CreateVertexShader(L"shader\\Billboard_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Default2D_PS.hlsl", "main");

		_shader->SetTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		_shader->SetRSType(RS_TYPE::CULL_BACK);
		//_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		//_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		// Parameter	
		//_shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(_shader->GetKey(), _shader);

		// ==================================================================
		_shader = new GraphicsShader;
		_shader->SetKey(L"DefaultLightShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\DefaultLight_PS.hlsl", "main");

		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		AddRes(_shader->GetKey(), _shader);

		// ==================================================================
		_shader = new GraphicsShader;
		_shader->SetKey(L"DeferredDefault3DShader");
		_shader->CreateVertexShader(L"shader\\DeferredDefault3D_VS.hlsl", "main");
		_shader->CreateInstVertexShader(L"shader\\DeferredDefault3DInst_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\DeferredDefault3D_PS.hlsl", "main");

		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
		_shader->SetRSType(RS_TYPE::CULL_BACK);
		_shader->SetDSType(DS_TYPE::LESS_EQUAL);
		_shader->AddScalarParam(FLOAT_0, "Emissive Factor : ");
		_shader->AddScalarParam(VEC4_0, "Emissive Factor : ");

		AddRes(_shader->GetKey(), _shader);

		// ==================================================================
		_shader = new GraphicsShader;
		_shader->SetKey(L"AmbientIBLShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\AmbientIBL_PS.hlsl", "main");

		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		AddRes(_shader->GetKey(), _shader);

		// ==================================================================
		_shader = new GraphicsShader;
		_shader->SetKey(L"MergeAmbientShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\MergeAmbient_PS.hlsl", "main");

		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		AddRes(_shader->GetKey(), _shader);

		// ==================================================================
		_shader = new GraphicsShader;
		_shader->SetKey(L"MergeLightShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\MergeLight_PS.hlsl", "main");

		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"PrefilterMapShader");
		_shader->CreateVertexShader(L"shader\\PrefilterMap_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\PrefilterMap_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"BRDFLookUpShader");
		_shader->CreateVertexShader(L"shader\\BRDFLookUp_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\BRDFLookUp_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"IrradianceShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Irradiance_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"CubeMapShader");
		_shader->CreateVertexShader(L"shader\\CubeMap_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\CubeMap_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"ToneMappingShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\ToneMapping_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"BloomExtractShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\BloomExtract_PS.hlsl", "main");

		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"FullScreenRenderShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\FullScreenRender_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"DistoritionShader");
		_shader->CreateVertexShader(L"shader\\Distorition_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Distorition_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"ParticleShader");
		_shader->CreateVertexShader(L"shader\\Particle_VS.hlsl", "main");
		_shader->CreateGeometryShader(L"shader\\Particle_GS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Particle_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"FXAAShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\FXAA_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
		_shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"GrayScaleShader");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\GrayScale_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"FlashSahder");
		_shader->CreateVertexShader(L"shader\\FullScreen_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Flash_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"LensShader");
		_shader->CreateVertexShader(L"shader\\DeferredDefault3D_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Lens_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::DEFAULT);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"Effect3DShader");
		_shader->CreateVertexShader(L"shader\\Effect3D_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Effect3D_PS.hlsl", "main");
			
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_WRITE);
		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		_shader->AddTexParam(TEX_0, "Color Texture");
		_shader->AddScalarParam(VEC4_0, "Color");
		_shader->AddScalarParam(VEC2_0, "UV");
		_shader->AddScalarParam(FLOAT_0, "Emissive Factor");

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"BillboardEffectShader");
		_shader->CreateVertexShader(L"shader\\Billboard_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Effect3D_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		_shader->AddTexParam(TEX_0, "Color Texture");
		_shader->AddScalarParam(VEC4_0, "Color");
		_shader->AddScalarParam(VEC2_0, "UV");
		_shader->AddScalarParam(FLOAT_0, "Emissive Factor");

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"DebugShader");
		_shader->CreateVertexShader(L"shader\\Debug_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Debug_PS.hlsl", "main");

		//_shader->SetTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		_shader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		_shader->SetRSType(RS_TYPE::CULL_NONE);
		//_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
		_shader->SetDSType(DS_TYPE::LESS);
		_shader->SetBSType(BS_TYPE::DEFAULT);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"ForwardDefault3DShader");
		_shader->CreateVertexShader(L"shader\\ForwardDefault3D_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\ForwardDefault3D_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"UIShader");

		_shader->CreateVertexShader(L"shader\\UI_VS.hlsl", "main");
		//_shader->CreatePixelShader(L"shader\\Debug_PS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\UI_PS.hlsl", "main");

		//_shader->SetTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		//_shader->SetDSType(DS_TYPE::NO_WRITE);
		//_shader->SetDSType(DS_TYPE::NO_TEST);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);


		//_shader->SetRSType(RS_TYPE::CULL_BACK);
		//_shader->SetDSType(DS_TYPE::LESS_EQUAL);
		////_shader->SetDSType(DS_TYPE::NO_WRITE);
		////_shader->SetDSType(DS_TYPE::NO_TEST);
		//_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		// Parameter	
		//_shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"FillUIShader");

		_shader->CreateVertexShader(L"shader\\UI_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\FillUI_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);


		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
		//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"TextShader");

		_shader->CreateVertexShader(L"shader\\Text_VS.hlsl", "main");
		//_shader->CreatePixelShader(L"shader\\Debug_PS.hlsl", "main");
		_shader->CreateGeometryShader(L"shader\\Text_GS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Text_PS.hlsl", "main");

		_shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		_shader->SetRSType(RS_TYPE::CULL_NONE);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		//_shader->SetDSType(DS_TYPE::NO_WRITE);
		//_shader->SetDSType(DS_TYPE::NO_TEST);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);
		AddRes(_shader->GetKey(), _shader);

		//*******************************************************
	//*******************************************************

		_shader = new GraphicsShader;
		_shader->SetKey(L"DecalShader");

		_shader->CreateVertexShader(L"shader\\Decal_VS.hlsl", "main");
		_shader->CreatePixelShader(L"shader\\Decal_PS.hlsl", "main");

		_shader->SetRSType(RS_TYPE::CULL_FRONT);
		_shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		_shader->SetBSType(BS_TYPE::ALPHA_BLEND);
		_shader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);
		AddRes(_shader->GetKey(), _shader);

		//_shader->SetRSType(RS_TYPE::CULL_BACK);
		//_shader->SetDSType(DS_TYPE::LESS_EQUAL);
		////_shader->SetDSType(DS_TYPE::NO_WRITE);
		////_shader->SetDSType(DS_TYPE::NO_TEST);
		//_shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

		// Parameter	
		//_shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(_shader->GetKey(), _shader);
	}

	void ResourceMgr::CreateDefaultComputeShader()
	{
		Ptr<ComputeShader> _cs = nullptr;

		// Animation Matrix Update 쉐이더
		_cs = new Animation3DShader(256, 1, 1);
		_cs->SetKey(L"Animation3DUpdateCS");
		_cs->CreateComputeShader(L"shader\\Animation3D_CS.hlsl", "main");
		AddRes(_cs->GetKey(), _cs);

		// Animation Matrix Update 쉐이더
		_cs = new ParticleShader(256, 1, 1);
		_cs->SetKey(L"ParticleCS");
		_cs->CreateComputeShader(L"shader\\Particle_CS.hlsl", "main");
		AddRes(_cs->GetKey(), _cs);


		// Animation Matrix Update 쉐이더
		_cs = new BlurShader(32, 32, 1);
		_cs->SetKey(L"BlurCS");
		_cs->CreateComputeShader(L"shader\\Blur_CS.hlsl", "main");
		AddRes(_cs->GetKey(), _cs);

		_cs = new CopyBoneShader(1024, 1, 1);
		_cs->SetKey(L"CopyBone_CS");
		_cs->CreateComputeShader(L"shader\\CopyBone_CS.hlsl", "main");
		AddRes(_cs->GetKey(), _cs);
	}

	void ResourceMgr::CreateDefaultMaterial()
	{
		Ptr<Material> _mtrl = nullptr;
		// SkyBoxMtrl
		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"SkyBoxShader"));
		AddRes(L"SkyBoxMtrl", _mtrl);

		// DecalMtrl
		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DecalShader"));
		AddRes(L"DecalMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DeferredDecalShader"));
		AddRes(L"DeferredDecalMtrl", _mtrl);

		// MergeMtrl
		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"MergeShader"));
		AddRes(L"MergeMtrl", _mtrl);

		// ShadowMapMtrl
		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"ShadowMapShader"));
		AddRes(L"ShadowMapMtrl", _mtrl);

		// TessMtrl
		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"TessShader"));
		AddRes(L"TessMtrl", _mtrl);

		//=============================================
		//=============================================
		//=============================================


		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DeferredDefault3DShader"));
		//_mtrl->SetScalarParam()
		AddRes(L"DeferredDefault3DMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DefaultLightShader"));
		AddRes(L"DefaultLightMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"AmbientIBLShader"));
		AddRes(L"AmbientIBLMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"MergeLightShader"));
		AddRes(L"MergeLightMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"PrefilterMapShader"));
		AddRes(L"PrefilterMapMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"BRDFLookUpShader"));
		AddRes(L"BRDFLookUpMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"IrradianceShader"));
		AddRes(L"IrradianceMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"CubeMapShader"));
		AddRes(L"CubeMapMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"MergeAmbientShader"));
		AddRes(L"MergeAmbientMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"ParticleShader"));
		AddRes(L"ParticleMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"ToneMappingShader"));
		AddRes(L"ToneMappingMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"BloomExtractShader"));
		AddRes(L"BloomExtractMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"FullScreenRenderShader"));
		AddRes(L"FullScreenRenderMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DistoritionShader"));
		AddRes(L"DistoritionMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"FXAAShader"));
		AddRes(L"FXAAMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"BillboardShader"));
		AddRes(L"BillboardMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"Effect3DShader"));
		AddRes(L"Effect3DMtrl", _mtrl);

		_mtrl = new Material(true);
		//_mtrl->SetShader(FindRes<GraphicsShader>(L"BillboardShader"));
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DebugShader"));
		AddRes(L"DebugMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"ForwardDefault3DShader"));
		AddRes(L"ForwardDefault3DMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"UIShader"));
		AddRes(L"UIMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"FillUIShader"));
		AddRes(L"FillUIMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"TextShader"));
		AddRes(L"TextMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"GrayScaleShader"));
		AddRes(L"GrayScaleMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"LensShader"));
		AddRes(L"LensMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"BillboardEffectShader"));
		AddRes(L"BillboardEffectMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"DecalShader"));
		AddRes(L"DecalMtrl", _mtrl);

		_mtrl = new Material(true);
		_mtrl->SetShader(FindRes<GraphicsShader>(L"FlashSahder"));
		AddRes(L"FlashMtrl", _mtrl);
		// =======================	
	}

	void ResourceMgr::CreateDefaultTexture()
	{
		D3D11_TEXTURE2D_DESC cubeDesc = {};
		cubeDesc.Width = 1024;  // 큐브맵의 각 면의 크기
		cubeDesc.Height = 1024;
		cubeDesc.MipLevels = 1;
		cubeDesc.ArraySize = 6;  // 큐브맵은 6개의 면을 가짐
		cubeDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;  // 텍스처 포맷
		cubeDesc.Usage = D3D11_USAGE_DEFAULT;
		cubeDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		cubeDesc.CPUAccessFlags = 0;
		cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		cubeDesc.SampleDesc.Count = 1;
		cubeDesc.SampleDesc.Quality = 0;
		// Direct3D 디바이스를 사용하여 텍스처 생성
		CreateTexture(L"CubeMap", cubeDesc);

		D3D11_TEXTURE2D_DESC prefiltermap = {};
		prefiltermap.Width = 512;  // 큐브맵의 각 면의 크기
		prefiltermap.Height = 512;
		prefiltermap.MipLevels = 6;
		prefiltermap.ArraySize = 6;  // 큐브맵은 6개의 면을 가짐
		prefiltermap.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;  // 텍스처 포맷
		prefiltermap.Usage = D3D11_USAGE_DEFAULT;
		prefiltermap.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		prefiltermap.CPUAccessFlags = 0;
		prefiltermap.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

		prefiltermap.SampleDesc.Count = 1;
		prefiltermap.SampleDesc.Quality = 0;
		// Direct3D 디바이스를 사용하여 텍스처 생성
		CreateTexture(L"DefaultPrefilterMap", prefiltermap);

		CreateTexture(L"BRDFLookUp", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

		D3D11_VIEWPORT _viewport;
		_viewport.TopLeftX = 0.f;
		_viewport.TopLeftY = 0.f;
		_viewport.MinDepth = 0.f;
		_viewport.MaxDepth = 1.f;

		_viewport.Width = 1024;
		_viewport.Height = 1024;

		CONTEXT->RSSetViewports(1, &_viewport);

		Ptr<Mesh> _mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh");
		Ptr<Material> _mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"BRDFLookUpMtrl");

		_mtrl->UpdateData();
		Ptr<Texture> lookup = ResourceMgr::GetInst()->FindRes<Texture>(L"BRDFLookUp");
		CONTEXT->OMSetRenderTargets(1, lookup->GetRTV().GetAddressOf(), nullptr);
		_mesh->Render();


		// ==========================================================================
		// ==========================================================================

		Ptr<Texture> cubemap = ResourceMgr::GetInst()->FindRes<Texture>(L"CubeMap");
		Ptr<Texture> hdr = ResourceMgr::GetInst()->Load<Texture>(L"texture\\hayloft_4k.hdr", L"texture\\hayloft_4k.hdr");
		//Ptr<Texture> hdr = ResourceMgr::GetInst()->Load<Texture>(L"texture\\hayloft_4k.hdr", L"texture\\hayloft_4k.hdr");
		_viewport.Width = 1024;
		_viewport.Height = 1024;

		CONTEXT->RSSetViewports(1, &_viewport);
		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"CubeMapMtrl");
		_mtrl->SetTexParam(TEX_0, hdr);

		for (int i = 0; i < 6; i++)
		{
			CONTEXT->OMSetRenderTargets(1, cubemap->GetRTV(i).GetAddressOf(), nullptr);
			_mtrl->SetScalarParam(INT_0, &i);
			_mtrl->UpdateData();
			_mesh->Render();
		}

		// ==========================================================================
		// ==========================================================================

		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"PrefilterMapMtrl");
		Ptr<Texture> prefilter = ResourceMgr::GetInst()->FindRes<Texture>(L"DefaultPrefilterMap");

		_viewport.Width = 512;
		_viewport.Height = 512;
		CONTEXT->RSSetViewports(1, &_viewport);

		_mtrl->SetTexParam(TEX_CUBE_0, cubemap);
		for (int i = 0; i < 6; i++)
		{
			CONTEXT->OMSetRenderTargets(1, prefilter->GetRTV(i).GetAddressOf(), nullptr);
			_mtrl->SetScalarParam(INT_0, &i);
			_mtrl->UpdateData();
			_mesh->Render();
		}
		CONTEXT->GenerateMips(prefilter->GetSRV().Get());
		CONTEXT->PSSetShaderResources(15, 1, prefilter.Get()->GetSRV().GetAddressOf());

		// ==========================================================================
		// ==========================================================================

		_viewport.Width = 32;
		_viewport.Height = 32;

		CONTEXT->RSSetViewports(1, &_viewport);

		CreateTexture(L"DefaultIrradianceMap", 32, 32, DXGI_FORMAT_R16G16B16A16_FLOAT
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

		_mtrl = ResourceMgr::GetInst()->FindRes<Material>(L"IrradianceMtrl");
		Ptr<Texture> irradiance_map = ResourceMgr::GetInst()->FindRes<Texture>(L"DefaultIrradianceMap");

		_mtrl->SetTexParam(TEX_CUBE_0, cubemap);
		_mtrl->UpdateData();
		CONTEXT->OMSetRenderTargets(1, irradiance_map->GetRTV().GetAddressOf(), nullptr);
		CONTEXT->Draw(4, 0);

		_viewport.Width = 1600;
		_viewport.Height = 900;

		CONTEXT->RSSetViewports(1, &_viewport);


		// ==========================================================================
		// ==========================================================================
		ResourceMgr::GetInst()->CreateTexture(L"CaptureLightTarget", 512, 512, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		vec2 _resolution = Engine::GetInst()->GetWindowResolution();

		ResourceMgr::GetInst()->CreateTexture(L"BloomExtract", (UINT)_resolution.x, (UINT)_resolution.y, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ResourceMgr::GetInst()->CreateTexture(L"BloomResolutionDivided2", (UINT)(_resolution.x / 2.0f), (UINT)(_resolution.y / 2.0f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ResourceMgr::GetInst()->CreateTexture(L"BloomResolutionDivided2_2", (UINT)(_resolution.x / 2.0f), (UINT)(_resolution.y / 2.0f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ResourceMgr::GetInst()->CreateTexture(L"BloomResolutionDivided4", (UINT)(_resolution.x / 4.0f), (UINT)(_resolution.y / 4.0f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ResourceMgr::GetInst()->CreateTexture(L"BloomResolutionDivided4_2", (UINT)(_resolution.x / 4.0f), (UINT)(_resolution.y / 4.0f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ResourceMgr::GetInst()->CreateTexture(L"BloomResolutionDivided8", (UINT)(_resolution.x / 8.0f), (UINT)(_resolution.y / 8.0f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ResourceMgr::GetInst()->CreateTexture(L"BloomResolutionDivided8_2", (UINT)(_resolution.x / 8.0f), (UINT)(_resolution.y / 8.0f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

	}

	void ResourceMgr::Test()
	{

		for (auto iter = resources[(UINT)RES_TYPE::MESHDATA].begin(); iter!= resources[(UINT)RES_TYPE::MESHDATA].end();)
		{
			if (iter->second->ref_cnt == 1)
			{
				iter = resources[(UINT)RES_TYPE::MESHDATA].erase(iter);
			}
			else
			{
				iter++;
			}
		}

		for (auto iter = resources[(UINT)RES_TYPE::MESH].begin(); iter != resources[(UINT)RES_TYPE::MESH].end();)
		{
			if (!iter->second->is_engine_res)
			{
				if (iter->second->ref_cnt == 1)
				{
					iter = resources[(UINT)RES_TYPE::MESH].erase(iter);
				}
				else
				{
					
					iter++;
				}
			}
			else
			{

			iter++;
			}
		}

		for (auto iter = resources[(UINT)RES_TYPE::MATERIAL].begin(); iter != resources[(UINT)RES_TYPE::MATERIAL].end();)
		{
			if(!iter->second->is_engine_res)
			{
				if (iter->second->ref_cnt == 1)
				{
					iter = resources[(UINT)RES_TYPE::MATERIAL].erase(iter);
				}
				else
				{
					iter++;
				}
			}
			else
			{

				iter++;
			}
		}

		for (auto iter = resources[(UINT)RES_TYPE::TEXTURE].begin(); iter != resources[(UINT)RES_TYPE::TEXTURE].end();)
		{
			if (!iter->second->is_engine_res)
			{
				if (iter->second->ref_cnt == 1)
				{
					iter = resources[(UINT)RES_TYPE::TEXTURE].erase(iter);
				}
				else
				{
					iter++;
				}
			}
			else
			{

				iter++;
			}
		}
	}

}