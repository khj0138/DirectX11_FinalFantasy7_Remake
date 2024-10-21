#include "pch.h"

#include "Animator2D.h"
#include "Animator3D.h"
#include "ResourceMgr.h"
#include "Transform.h"
#include "RenderMgr.h"

#include "TrailRenderer.h"
namespace ff7r
{
	TrailRenderer::TrailRenderer()
		: RenderComponent(COMPONENT_TYPE::TRAILRENDERER)
		, trail_socket_a(nullptr)
		, trail_socket_b(nullptr)
		, vtx_cnt(42)
	{
		Ptr<Mesh> _mesh = ResourceMgr::GetInst()->FindRes<Mesh>(L"TrailMesh");

		SetMesh(_mesh);
		vertex.resize(vtx_cnt);
	}

	TrailRenderer::~TrailRenderer()
	{
	}

	void TrailRenderer::FinalTick()
	{
		if (trail_socket_a && trail_socket_b)
		{
			Matrix _a = trail_socket_a->GetTransform()->GetWorldMat();
			Matrix _b = trail_socket_b->GetTransform()->GetWorldMat();

			vec3 _vtx_a = vec3(_a._41, _a._42, _a._43);
			vec3 _vtx_b = vec3(_b._41, _b._42, _b._43);

			socket_pos.push_back(_vtx_a);
			socket_pos.push_back(_vtx_b);
			
			while (socket_pos.size() > vtx_cnt)
			{
				socket_pos.pop_front();
			}

			for (int i = 0; i < socket_pos.size(); i++)
			{
				vertex[i].pos = socket_pos[i];
			}

			GetMesh()->UpdateVertex(vertex.data());
		}
	}

	void TrailRenderer::Render()
	{
		Ptr<Mesh> _mesh = GetMesh();

		if (nullptr == _mesh || nullptr == GetMaterial())
			return;

		// Transform 에 UpdateData 요청
		GetTransform()->UpdateData();

		// Animator2D 컴포넌트가 있다면
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

				GetMaterial(i)->SetAnimation3D(true); // Animation Mesh 알리기
				GetMaterial(i)->SetJointCount(_mesh->GetJointCount());
			}
		}

		for (size_t i = 0; i < _mesh->GetSubsetCount(); ++i)
		{
			UINT _mtrl_id = _mesh->GetMaterialID(i);

			GetMaterial(_mtrl_id)->UpdateData();
			_mesh->Render(i);
		}

		// Animation 관련 정보 제거
		if (GetAnimator2D())
		{
			GetAnimator2D()->Clear();
		}
		if (GetAnimator3D())
			GetAnimator3D()->ClearData();
	}
	void TrailRenderer::SetTrailColor(vec4 _color)
	{
		GetMaterial()->SetScalarParam(VEC4_0, _color);
	}
}