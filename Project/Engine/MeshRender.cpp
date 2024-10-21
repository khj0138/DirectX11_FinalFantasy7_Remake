#include "pch.h"
#include "MeshRender.h"

#include "Transform.h"
#include "Animator2D.h"
#include "Animator3D.h"

#include "StructuredBuffer.h"

namespace ff7r
{
	MeshRender::MeshRender()
		: RenderComponent(COMPONENT_TYPE::MESHRENDER)
	{
	}

	MeshRender::~MeshRender()
	{
	}

	void MeshRender::FinalTick()
	{
	}

	void MeshRender::Render()
	{
		Ptr<Mesh> _mesh = GetMesh();

		if (nullptr == _mesh || nullptr == GetMaterial())
			return;
		//// Transform �� UpdateData ��û
		GetTransform()->UpdateData();
		// Animator2D ������Ʈ�� �ִٸ�
		if (GetAnimator2D())
		{
			GetAnimator2D()->UpdateData();
		}

		if (GetAnimator3D())
		{
			GetAnimator3D()->UpdateData();

			for (UINT i = 0; i < GetMtrlCount(); ++i)
			{
				if (nullptr == GetMaterial(i))
					continue;

				GetMaterial(i)->SetAnimation3D(true); // Animation Mesh �˸���
				GetMaterial(i)->SetJointCount(_mesh->GetJointCount());
			}
		}

		for (size_t i = 0; i < _mesh->GetSubsetCount(); ++i)
		{
			UINT _mtrl_id = _mesh->GetMaterialID(i);

			GetMaterial(_mtrl_id)->UpdateData();
			_mesh->Render(i);
		}
			
		// Animation ���� ���� ����
		if (GetAnimator2D())
		{
			GetAnimator2D()->Clear();
		}
		if (GetAnimator3D())
		{


			GetAnimator3D()->ClearData();
		}
	}

	void MeshRender::Render(UINT _subset)
	{
		Ptr<Mesh> _mesh = GetMesh();

		if (nullptr == _mesh || nullptr == GetMaterial())
			return;
		//// Transform �� UpdateData ��û
		GetTransform()->UpdateData();
		// Animator2D ������Ʈ�� �ִٸ�
		if (GetAnimator2D())
		{
			GetAnimator2D()->UpdateData();
		}

		if (GetAnimator3D())
		{
			GetAnimator3D()->UpdateData();
			GetMaterial(_subset)->SetAnimation3D(true); // Animation Mesh �˸���
			GetMaterial(_subset)->SetJointCount(_mesh->GetJointCount());
		}


		GetMaterial(_subset)->UpdateData();
		_mesh->Render(_subset);

		if (GetAnimator2D())
		{
			GetAnimator2D()->Clear();
		}
		if (GetAnimator3D())
		{
			GetAnimator3D()->ClearData();
		}
	}
	
}