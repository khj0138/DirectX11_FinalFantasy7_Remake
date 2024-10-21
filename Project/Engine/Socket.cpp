#include "pch.h"

#include "MeshRender.h"
#include "Animator3D.h"
#include "Transform.h"
#include "StructuredBuffer.h"

#include "Socket.h"
namespace ff7r
{
	Socket::Socket()
		: Component(COMPONENT_TYPE::SOCKET)
		, socket_idx(0)
	{
	}
	Socket::Socket(const Socket& _Other)
		: Component(COMPONENT_TYPE::SOCKET)
		, socket_idx(_Other.socket_idx)
	{
	}

	Socket::~Socket()
	{
	}

	void Socket::Begin()
	{
	}

	void Socket::FinalTick()
	{
		GameObject* _parent = GetOwner()->GetParent();
		if (_parent)
		{
			MeshRender* _parent_renderer = _parent->GetMeshRender();
			if (_parent_renderer)
			{
				if (_parent->GetAnimator3D())
				{
					UINT _cnt = _parent_renderer->GetMesh()->GetJointCount();

					vector<Matrix> _joint_mat;
					_joint_mat.resize(_cnt);
					
					StructuredBuffer* buff = _parent->GetAnimator3D()->GetCurJointMat();
					buff->GetData(_joint_mat.data());
					_joint_mat[socket_idx]._44 = 1;
					
					GetTransform()->mat_world = _joint_mat[socket_idx] * GetTransform()->mat_world;
					GetTransform()->mat_world_inv = XMMatrixInverse(nullptr, GetTransform()->mat_world);
					GetTransform()->CalcWorldDir();
				}
				else
				{
					auto& joints = _parent_renderer->GetMesh()->GetJoints();

					GetTransform()->mat_world = joints[socket_idx].mat_bone * GetTransform()->mat_world;
					GetTransform()->mat_world_inv = XMMatrixInverse(nullptr, GetTransform()->mat_world);
					GetTransform()->CalcWorldDir();
				}
			}
		}
	}

	void Socket::SetSocket(wstring _name)
	{
		GameObject* _parent = GetOwner()->GetParent();
		if (_parent)
		{
			Ptr<Mesh> _mesh = _parent->GetMeshRender()->GetMesh();

			if (_mesh != nullptr)
			{
				auto joints = _mesh->GetJoints();

				for (UINT i = 0; i < joints.size(); i++)
				{
					if (joints[i].name == _name)
					{
						socket_idx = i;
						return;
					}
				}
			}
		}
	}

	void Socket::SaveToLevelFile(FILE* _file)
	{
		fwrite(&socket_idx, sizeof(UINT), 1, _file);
	}

	void Socket::LoadFromLevelFile(FILE* _file)
	{
		fread(&socket_idx, sizeof(UINT), 1, _file);
	}
}