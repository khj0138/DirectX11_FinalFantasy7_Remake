#include "pch.h"

#include "Device.h"
#include "GLTFLoader.h"
#include "PathMgr.h"
#include "StructuredBuffer.h"
#include "InstancingBuffer.h"

#include "Mesh.h"
namespace ff7r
{
	Mesh::Mesh(bool _bEngine)
		: Resource(RES_TYPE::MESH, _bEngine)
		, is_anim_mesh(false)
		, inverse_joint_buff(nullptr)
		, bounding_radius(0.0f)
	{
	}

	Mesh::~Mesh()
	{
		for (size_t i = 0; i < subsets.size(); ++i)
		{
			if (nullptr != subsets[i].index_sys_mem)
			{
				delete subsets[i].index_sys_mem;
				subsets[i].index_sys_mem = nullptr;
			}
			if (nullptr != subsets[i].vertex_sys_mem)
			{
				delete subsets[i].vertex_sys_mem;
				subsets[i].vertex_sys_mem = nullptr;
			}
		}

		if (nullptr != inverse_joint_buff)
		{
			delete inverse_joint_buff;
			inverse_joint_buff = nullptr;
		}
	}

	void Mesh::Create(void* _vtx_sys_mem, UINT _vtx_cnt, void* _idx_sys_mem, UINT _idx_cnt, bool _updata_vtx)
	{
		SubsetInfo _subset = {};

		_subset.vertex_cnt = _vtx_cnt;
		_subset.vertex_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		if (_updata_vtx)
		{
			_subset.vertex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			_subset.vertex_desc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else
		{
			_subset.vertex_desc.CPUAccessFlags = 0;
			_subset.vertex_desc.Usage = D3D11_USAGE_DEFAULT;
		}
		_subset.vertex_desc.ByteWidth = sizeof(Vtx) * _vtx_cnt;
		_subset.vertex_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA _sub = {};
		_sub.pSysMem = _vtx_sys_mem;
		if (FAILED(DEVICE->CreateBuffer(&_subset.vertex_desc, &_sub, _subset.vertex_buff.GetAddressOf())))
		{
			assert(nullptr);
		}

		_subset.vertex_sys_mem = new Vtx[_vtx_cnt];
		memcpy(_subset.vertex_sys_mem, _vtx_sys_mem, sizeof(Vtx) * _vtx_cnt);

		// Index 버퍼 생성
		_subset.index_cnt = _idx_cnt;
		_subset.index_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		_subset.index_desc.CPUAccessFlags = 0;
		_subset.index_desc.Usage = D3D11_USAGE_DEFAULT;
		_subset.index_desc.ByteWidth = sizeof(UINT) * _idx_cnt;
		_subset.index_desc.StructureByteStride = 0;
		_subset.index_desc.MiscFlags = 0;

		_sub = {};
		_sub.pSysMem = _idx_sys_mem;
		if (FAILED(DEVICE->CreateBuffer(&_subset.index_desc, &_sub, _subset.index_buff.GetAddressOf())))
		{
			assert(nullptr);
		}

		_subset.index_sys_mem = new UINT[_idx_cnt];
		memcpy(_subset.index_sys_mem, _idx_sys_mem, sizeof(UINT) * _idx_cnt);

		subsets.push_back(_subset);
	}

	void Mesh::UpdateVertex(void* _vtx_sys_mem)
	{
		D3D11_MAPPED_SUBRESOURCE _sub = {};

		HRESULT hr = CONTEXT->Map(subsets[0].vertex_buff.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &_sub);
		if (SUCCEEDED(hr))
		{
			memcpy(_sub.pData, _vtx_sys_mem, sizeof(Vtx) * subsets[0].vertex_cnt);
			CONTEXT->Unmap(subsets[0].vertex_buff.Get(), 0);
		}
	}

	void Mesh::UpdateData(UINT _idx)
	{
		UINT iStride = sizeof(Vtx);
		UINT iOffset = 0;

		CONTEXT->IASetVertexBuffers(0, 1, subsets[_idx].vertex_buff.GetAddressOf(), &iStride, &iOffset);
		CONTEXT->IASetIndexBuffer(subsets[_idx].index_buff.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::UpdateDataInst(UINT _idx)
	{
		if (_idx >= subsets.size())
		{
			assert(nullptr);
		}

		ID3D11Buffer* arrBuffer[2] = { subsets[_idx].vertex_buff.Get(), InstancingBuffer::GetInst()->GetBuffer().Get() };
		UINT		  iStride[2] = { sizeof(Vtx), sizeof(InstancingData) };
		UINT		  iOffset[2] = { 0, 0 };

		CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, iStride, iOffset);
		CONTEXT->IASetIndexBuffer(subsets[_idx].index_buff.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render(UINT _idx)
	{
		UpdateData(_idx);

		CONTEXT->DrawIndexed(subsets[_idx].index_cnt, 0, 0);
	}

	void Mesh::RenderParticle(UINT _particle_cnt)
	{
		UpdateData(0);

		// 인스턴싱
		CONTEXT->DrawIndexedInstanced(subsets[0].index_cnt, _particle_cnt, 0, 0, 0);
	}

	void Mesh::RenderInstancing(UINT _subset)
	{
		UpdateDataInst(_subset);

		CONTEXT->DrawIndexedInstanced(subsets[_subset].index_cnt
			, InstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);
	}

	Mesh* Mesh::CreateFromGLTF(GLTFLoader& _loader)
	{
		const Container* container = &_loader.GetContainer();

		Mesh* _mesh = new Mesh;

		for (UINT i = 0; i < container->vertex.size(); i++)
		{
			SubsetInfo _subset = {};

			UINT _vtx_cnt = container->vertex[i].size();

			_subset.vertex_cnt = _vtx_cnt;
			_subset.vertex_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			_subset.vertex_desc.CPUAccessFlags = 0;
			_subset.vertex_desc.Usage = D3D11_USAGE_DEFAULT;
			_subset.vertex_desc.ByteWidth = sizeof(Vtx) * _vtx_cnt;
			_subset.vertex_desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA _sub = {};
			_sub.pSysMem = (void*)container->vertex[i].data();
			if (FAILED(DEVICE->CreateBuffer(&_subset.vertex_desc, &_sub, _subset.vertex_buff.GetAddressOf())))
			{
				assert(nullptr);
			}

			_subset.vertex_sys_mem = new Vtx[_vtx_cnt];
			memcpy(_subset.vertex_sys_mem, (void*)container->vertex[i].data(), sizeof(Vtx) * _vtx_cnt);

			UINT _idx_cnt = container->index[i].size();

			_subset.index_cnt = _idx_cnt;
			_subset.index_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			_subset.index_desc.CPUAccessFlags = 0;
			_subset.index_desc.Usage = D3D11_USAGE_DEFAULT;
			_subset.index_desc.ByteWidth = sizeof(UINT) * _idx_cnt;
			_subset.index_desc.StructureByteStride = 0;
			_subset.index_desc.MiscFlags = 0;

			_sub = {};
			_sub.pSysMem = (void*)container->index[i].data();
			if (FAILED(DEVICE->CreateBuffer(&_subset.index_desc, &_sub, _subset.index_buff.GetAddressOf())))
			{
				assert(nullptr);
			}

			_subset.index_sys_mem = new UINT[_idx_cnt];
			memcpy(_subset.index_sys_mem, (void*)container->index[i].data(), sizeof(UINT) * _idx_cnt);

			_subset.mtrl_id = container->material_id[i];

			_mesh->subsets.push_back(_subset);
		}

		vector<Joint*>& joints = _loader.GetBones();

		if (joints.empty())
		{
			return _mesh;
		}

		for (UINT i = 0; i < joints.size(); i++)
		{
			MTJoint joint = {};
			joint.depth = joints[i]->depth;
			joint.parent_id = joints[i]->parent_id;
			joint.mat_bone = joints[i]->mat_joint;
			joint.mat_inverse = joints[i]->mat_inverse;
			joint.name = joints[i]->name;

			_mesh->joints.push_back(joint);
		}

		vector<Matrix> _inverse_mat;

		for (size_t i = 0; i < _mesh->joints.size(); ++i)
		{
			_inverse_mat.push_back(_mesh->joints[i].mat_inverse);
		}

		_mesh->inverse_joint_buff = new StructuredBuffer;
		_mesh->inverse_joint_buff->Create(sizeof(Matrix), (UINT)_inverse_mat.size(), SB_TYPE::READ_ONLY, false, _inverse_mat.data());

		if (!_loader.GetAnimClip().empty())
		{
			_mesh->is_anim_mesh = true;
		}

		return _mesh;
	}

	int Mesh::Save(const wstring& _relative_path)
	{
		// 상대경로 저장
		SetRelativePath(_relative_path);

		// 파일 경로 만들기
		wstring _file_path = PathMgr::GetInst()->GetContentPath() + _relative_path;

		path dirPath = path(_file_path).parent_path();
		create_directories(dirPath);

		FILE* _file = nullptr;
		errno_t err = _wfopen_s(&_file, _file_path.c_str(), L"wb");
		assert(_file);

		// 키값, 상대 경로	
		SaveWString(GetName(), _file);
		SaveWString(GetKey(), _file);
		SaveWString(GetRelativePath(), _file);

		// 정점 데이터 저장				

		int _subset_size = subsets.size();
		fwrite(&_subset_size, sizeof(int), 1, _file);

		for (int i = 0; i < _subset_size; i++)
		{
			fwrite(&subsets[i], sizeof(SubsetInfo), 1, _file);
			fwrite(subsets[i].index_sys_mem, subsets[i].index_cnt * sizeof(UINT), 1, _file);
			fwrite(subsets[i].vertex_sys_mem, subsets[i].vertex_cnt * sizeof(Vtx), 1, _file);
		}

		UINT _joint_cnt = (UINT)joints.size();
		fwrite(&_joint_cnt, sizeof(UINT), 1, _file);

		for (UINT i = 0; i < _joint_cnt; ++i)
		{
			SaveWString(joints[i].name, _file);
			fwrite(&joints[i].depth, sizeof(int), 1, _file);
			fwrite(&joints[i].parent_id, sizeof(int), 1, _file);
			fwrite(&joints[i].mat_bone, sizeof(Matrix), 1, _file);
			fwrite(&joints[i].mat_inverse, sizeof(Matrix), 1, _file);
		}

		fwrite(&is_anim_mesh, sizeof(bool), 1, _file);
		
		fclose(_file);

		return S_OK;
	}

	int Mesh::Load(const wstring& _file_path)
	{
		FILE* _file = nullptr;
		_wfopen_s(&_file, _file_path.c_str(), L"rb");

		// 키값, 상대경로
		wstring _name, _key, _path;
		LoadWString(_name, _file);
		LoadWString(_key, _file);
		LoadWString(_path, _file);

		SetName(_name);
		SetKey(_key);
		SetRelativePath(_path);


		int _subset_size = 0;
		fread(&_subset_size, sizeof(int), 1, _file);

		vec3 min;
		vec3 max;
		vec3 center;
		bool first = true;
		D3D11_SUBRESOURCE_DATA _sub = {};
		for (int i = 0; i < _subset_size; i++)
		{
			SubsetInfo _info = {};

			fread(&_info, sizeof(SubsetInfo), 1, _file);

			UINT _byte_width = _info.index_cnt * sizeof(UINT);
			void* _index_sys_mem = malloc(_byte_width);
			_info.index_sys_mem = _index_sys_mem;
			fread(_info.index_sys_mem, _byte_width, 1, _file);

			_sub.pSysMem = _info.index_sys_mem;
			if (FAILED(DEVICE->CreateBuffer(&_info.index_desc, &_sub, _info.index_buff.GetAddressOf())))
			{
				assert(nullptr);
			}

			_byte_width = _info.vertex_cnt * sizeof(Vtx);
			_info.vertex_sys_mem = malloc(_byte_width);
			fread(_info.vertex_sys_mem, _byte_width, 1, _file);

			Vtx* test = static_cast<Vtx*>(_info.vertex_sys_mem);
			vector<Vtx> a(test, test + _info.vertex_cnt);

			if (first)
			{
				min = a[0].pos;
				max = a[0].pos;
				first = false;
			}

			for (auto& t : a) 
			{
				min.x = MIN(t.pos.x, min.x);
				max.x = MAX(t.pos.x, max.x);

				min.y = MIN(t.pos.y, min.y);
				max.y = MAX(t.pos.y, max.y);

				min.z = MIN(t.pos.z, min.z);
				max.z = MAX(t.pos.z, max.z);
			}
			

			_sub.pSysMem = _info.vertex_sys_mem;
			if (FAILED(DEVICE->CreateBuffer(&_info.vertex_desc, &_sub, _info.vertex_buff.GetAddressOf())))
			{
				assert(nullptr);
			}

			subsets.push_back(_info);
		}
		
		center = (min + max) / 2.0f;

		vec3 len = max - min;
		bounding_radius = MAX(MAX(len.x, len.y), len.z);
		bounding_offset = center;

		UINT _joint_cnt = 0;
		fread(&_joint_cnt, sizeof(int), 1, _file);

		joints.resize(_joint_cnt);

		for (UINT i = 0; i < _joint_cnt; ++i)
		{
			Joint _joint = {};

			LoadWString(joints[i].name, _file);
			fread(&joints[i].depth, sizeof(int), 1, _file);
			fread(&joints[i].parent_id, sizeof(int), 1, _file);
			fread(&joints[i].mat_bone, sizeof(Matrix), 1, _file);
			fread(&joints[i].mat_inverse, sizeof(Matrix), 1, _file);
		}

		fread(&is_anim_mesh, sizeof(bool), 1, _file);

		if (is_anim_mesh)
		{
			vector<Matrix> _inverse_mat;

			for (size_t i = 0; i < joints.size(); ++i)
			{
				_inverse_mat.push_back(joints[i].mat_inverse);
			}

			inverse_joint_buff = new StructuredBuffer;
			inverse_joint_buff->Create(sizeof(Matrix), (UINT)_inverse_mat.size(), SB_TYPE::READ_ONLY, false, _inverse_mat.data());
		}

		fclose(_file);

		return S_OK;
	}
}