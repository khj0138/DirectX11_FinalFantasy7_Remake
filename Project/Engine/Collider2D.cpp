#include "pch.h"
#include "Collider2D.h"

#include "Script.h"
#include "components.h"

namespace ff7r
{
	Collider2D::Collider2D()
		: Component(COMPONENT_TYPE::COLLIDER2D)
		, type(COLLIDER2D_TYPE::RECT)
		, is_absolute(false)
		, collision_cnt(0)
	{
		SetName(L"Collider2D");
	}

	Collider2D::~Collider2D()
	{
	}


	void Collider2D::FinalTick()
	{
		// 충돌 회수가 음수인 경우
		assert(0 <= collision_cnt);

		mat_colli_world = XMMatrixScaling(offset_scale.x, offset_scale.y, offset_scale.z);
		mat_colli_world *= XMMatrixTranslation(offset_pos.x, offset_pos.y, offset_pos.z);

		const Matrix& matWorld = GetTransform()->GetWorldMat();

		if (is_absolute)
		{
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, GetTransform()->GetWorldScaleMat());
			mat_colli_world = mat_colli_world * matParentScaleInv * matWorld;
		}
		else
		{
			// 충돌체 월드 * 오브젝트 월드
			mat_colli_world *= matWorld;
		}

		// DebugShape 요청
		vec4 vColor = vec4(0.f, 1.f, 0.f, 1.f);
		if (0 < collision_cnt)
			vColor = vec4(1.f, 0.f, 0.f, 1.f);

		if (COLLIDER2D_TYPE::CIRCLE == type)
			DrawDebugCircle(mat_colli_world, vColor, 0.f);
		else
			DrawDebugRect(mat_colli_world, vColor, 0.f);
	}



	void Collider2D::BeginOverlap(Collider2D* _Other)
	{
		collision_cnt += 1;

		// Script 호출
		//const vector<Script*>& vecScript = GetOwner()->GetScripts();
		//for (size_t i = 0; i < vecScript.size(); ++i)
		//{
		//	vecScript[i]->BeginOverlap(_Other);
		//}
	}

	void Collider2D::OnOverlap(Collider2D* _Other)
	{
		// Script 호출
		//const vector<Script*>& vecScript = GetOwner()->GetScripts();
		//for (size_t i = 0; i < vecScript.size(); ++i)
		//{
		//	vecScript[i]->OnOverlap(_Other);
		//}
	}

	void Collider2D::EndOverlap(Collider2D* _Other)
	{
		collision_cnt -= 1;

		// Script 호출
		//const vector<Script*>& vecScript = GetOwner()->GetScripts();
		//for (size_t i = 0; i < vecScript.size(); ++i)
		//{
		//	vecScript[i]->EndOverlap(_Other);
		//}
	}

	void Collider2D::SaveToLevelFile(FILE* _File)
	{
		fwrite(&offset_pos, sizeof(vec3), 1, _File);
		fwrite(&offset_scale, sizeof(vec3), 1, _File);
		fwrite(&is_absolute, sizeof(bool), 1, _File);
		fwrite(&type, sizeof(UINT), 1, _File);
	}

	void Collider2D::LoadFromLevelFile(FILE* _File)
	{
		fread(&offset_pos, sizeof(vec3), 1, _File);
		fread(&offset_scale, sizeof(vec3), 1, _File);
		fread(&is_absolute, sizeof(bool), 1, _File);
		fread(&type, sizeof(UINT), 1, _File);
	}
}