#pragma once
#include "Resource.h"

namespace ff7r
{
	struct SubsetInfo
	{
		ComPtr<ID3D11Buffer>    index_buff;
		D3D11_BUFFER_DESC       index_desc;
		UINT				    index_cnt;
		void*					index_sys_mem;

		ComPtr<ID3D11Buffer>	vertex_buff;
		D3D11_BUFFER_DESC		vertex_desc;
		UINT					vertex_cnt;
		void*					vertex_sys_mem;

		UINT					mtrl_id;
	};

	class GLTFLoader;
	class StructuredBuffer;
	class Mesh : public Resource
	{
	public:
		Mesh(bool _is_engine = false);
		~Mesh();

		void Create(void* _vtx_sys_mem, UINT _vtx_cnt, void* _idx_sys_mem, UINT _idx_cnt, bool _updata_vtx = false);
		void UpdateVertex(void* _vtx_sys_mem);

		void Render(UINT _idx = 0);
		void RenderParticle(UINT _particle_cnt);
		void RenderInstancing(UINT _subset);

		UINT GetSubsetCount() { return (UINT)subsets.size(); }
		UINT GetMaterialID(UINT _idx) { return subsets[_idx].mtrl_id; }

		const vector<MTJoint>& GetJoints() { return joints; }
		const MTJoint GetJoint(int _idx) { return joints[_idx]; }
		UINT GetJointCount() { return (UINT)joints.size(); }

		StructuredBuffer* GetJointInverseBuffer() { return  inverse_joint_buff; }
		const vector<SubsetInfo>& GetSubSet() { return subsets; }

		vec3 GetBoundingOffset() { return bounding_offset; }
		float GetBoundingRadius() { return bounding_radius; }

		static Mesh* CreateFromGLTF(GLTFLoader& _loader);
		virtual int Save(const wstring& _relative_path);

	private:
		virtual int Load(const wstring& _file_path);
		virtual void UpdateData(UINT _idx = 0);
		void UpdateDataInst(UINT _idx = 0);

		vector<SubsetInfo>	subsets;
		vector<MTJoint>		joints;
		StructuredBuffer*	inverse_joint_buff;

		vec3				bounding_offset;
		float				bounding_radius;
		bool				is_anim_mesh;
	};
}