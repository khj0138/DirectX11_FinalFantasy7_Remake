#include "pch.h"
#include "Collider3D.h"

#include "Script.h"
#include "components.h"
#include "PhysXMgr.h"

namespace ff7r
{
	Collider3D::Collider3D()
		: Component(COMPONENT_TYPE::COLLIDER3D)
		, offset_scale(vec3(1.0f))
		, type(COLLIDER3D_TYPE::CUBE)
		, is_absolute(false)
		, is_trigger(false)
		, is_collision(false)
		, collision_cnt(0)
		, able_collision(true)
		, is_move(true)
		, mass(1.0f)
		, scene_destroy(true)
		
	{
		SetName(L"Collider3D");

	}

	Collider3D::~Collider3D()
	{
		//if(!(LevelMgr::GetInst()->GetDestroy()) && !(PhysXMgr::GetInst()->GetDestroy()))
		if(!scene_destroy)
			PhysXMgr::GetInst()->EraseTarget(this);
	}

	void Collider3D::Awake()
	{
		//PhysXMgr::GetInst()->AddTarget(this, is_trigger, is_dynamic);
	}

	void Collider3D::Start()
	{
		PhysXMgr::GetInst()->AddTarget(this, is_trigger, is_dynamic);
		scene_destroy = false;

		// SetCollision(true);
	}

	void Collider3D::FinalTick()
	{
		//return;
		// 충돌 회수가 음수인 경우
		assert(0 <= collision_cnt);

		mat_colli_world = XMMatrixScaling(offset_scale.x, offset_scale.y, offset_scale.z);
		mat_colli_world *= XMMatrixTranslation(offset_pos.x, offset_pos.y, offset_pos.z);

		//const Matrix& matWorld = GetTransform()->GetWorldMat();	

		vec3 _scale = GetTransform()->GetRelativeScale();
		vec3 _rot = GetTransform()->GetRelativeRot();
		vec3 _pos = GetTransform()->GetRelativePos();

		Matrix _mat_scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
		Matrix _mat_rotX = XMMatrixRotationX(_rot.x);
		Matrix _mat_rotY = XMMatrixRotationY(_rot.y);
		Matrix _mat_rotZ = XMMatrixRotationZ(_rot.z);
		Matrix _mat_pos = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		// 충돌체 월드 * 오브젝트 월드
		Matrix _matWorld = XMMatrixIdentity();
		_matWorld *= _mat_scale;
		_matWorld *= _mat_rotX;
		_matWorld *= _mat_rotY;
		_matWorld *= _mat_rotZ;
		_matWorld *= _mat_pos;

		if (GetOwner()->GetParent() /*&& GetOwner()->GetParent()->GetSocket()*/)
		{
			_matWorld = GetTransform()->GetWorldMat();
		}

		if (is_absolute)
		{
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, _mat_scale);
			//Matrix matParentScaleInv = XMMatrixInverse(nullptr, GetTransform()->GetWorldScaleMat());
			mat_colli_world = mat_colli_world * matParentScaleInv * _matWorld;
		}
		else
		{
			mat_colli_world *= _matWorld;
		}


		// DebugShape 요청
		vec4 vColor = vec4(0.f, 1.f, 0.f, 1.f);
		if (0 < collision_cnt)
			vColor = vec4(1.f, 0.f, 0.f, 1.f);
		 
		//if (COLLIDER3D_TYPE::SPHERE == type)
		//	DrawDebugCircle(mat_colli_world, vColor, 0.f);
		//else
		//	DrawDebugCube(mat_colli_world, vColor, 0.f);
	}



	void Collider3D::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		is_collision = true;

		const vector<Script*>& scripts = GetOwner()->GetScripts();
		for (size_t i = 0; i < scripts.size(); i++)
		{
			scripts[i]->BeginOverlap(_other, _contact_pos);
		}
	}

	void Collider3D::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		is_collision = true;

		const vector<Script*>& scripts = GetOwner()->GetScripts();
		for (size_t i = 0; i < scripts.size(); i++)
		{
			scripts[i]->OnOverlap(_other, _contact_pos);
		}

	}

	void Collider3D::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		is_collision = false;

		const vector<Script*>& scripts = GetOwner()->GetScripts();
		for (size_t i = 0; i < scripts.size(); i++)
		{
			scripts[i]->EndOverlap(_other, _contact_pos);
		}
	}

	void Collider3D::SetCollision(bool _collision)
	{
		if (_collision != able_collision)
		{
			if (is_trigger)
				PhysXMgr::GetInst()->SetActorTrigger(this->GetID(), _collision);
			else
				PhysXMgr::GetInst()->SetActorCollision(this->GetID(), _collision);

			able_collision = _collision;
		}
	}

	void Collider3D::Disable()
	{
		PhysXMgr::GetInst()->SetDisable(this);
	}

	//void Collider3D::BeginOverlap(Collider3D* _Other)
	//{
	//	collision_cnt += 1;
	//
	//	// Script 호출
	//	const vector<Script*>& vecScript = GetOwner()->GetScripts();
	//	for (size_t i = 0; i < vecScript.size(); ++i)
	//	{
	//		vecScript[i]->BeginOverlap(_Other);
	//	}
	//}
	//
	//void Collider3D::OnOverlap(Collider3D* _Other)
	//{
	//	// Script 호출
	//	const vector<Script*>& vecScript = GetOwner()->GetScripts();
	//	for (size_t i = 0; i < vecScript.size(); ++i)
	//	{
	//		vecScript[i]->OnOverlap(_Other);
	//	}
	//}
	//
	//void Collider3D::EndOverlap(Collider3D* _Other)
	//{
	//	collision_cnt -= 1;
	//
	//	// Script 호출
	//	const vector<Script*>& vecScript = GetOwner()->GetScripts();
	//	for (size_t i = 0; i < vecScript.size(); ++i)
	//	{
	//		vecScript[i]->EndOverlap(_Other);
	//	}
	//}

	//bool            able_collision;
	//bool            is_trigger;
	//bool            is_dynamic;
	//bool            is_collision;
	//bool            is_move;

	void Collider3D::SaveToLevelFile(FILE* _File)
	{
		fwrite(&offset_pos, sizeof(vec3), 1, _File);
		fwrite(&offset_scale, sizeof(vec3), 1, _File);
		fwrite(&is_absolute, sizeof(bool), 1, _File);
		fwrite(&type, sizeof(UINT), 1, _File);
		fwrite(&mass, sizeof(float), 1, _File);
		fwrite(&able_collision, sizeof(bool), 1, _File);
		fwrite(&is_trigger, sizeof(bool), 1, _File);
		fwrite(&is_dynamic, sizeof(bool), 1, _File);
		fwrite(&is_collision, sizeof(bool), 1, _File);
		fwrite(&is_move, sizeof(bool), 1, _File);
	}

	void Collider3D::LoadFromLevelFile(FILE* _File)
	{
		fread(&offset_pos, sizeof(vec3), 1, _File);
		fread(&offset_scale, sizeof(vec3), 1, _File);
		fread(&is_absolute, sizeof(bool), 1, _File);
		fread(&type, sizeof(UINT), 1, _File);
		fread(&mass, sizeof(float), 1, _File);
		fread(&able_collision, sizeof(bool), 1, _File);
		fread(&is_trigger, sizeof(bool), 1, _File);
		fread(&is_dynamic, sizeof(bool), 1, _File);
		fread(&is_collision, sizeof(bool), 1, _File);
		fread(&is_move, sizeof(bool), 1, _File);

	}
	void Collider3D::OnDisable()
	{
		//Disable();
	}
	vec3 Collider3D::GetWorldPos()
	{
		return GetTransform()->GetWorldPos() + offset_pos;
	}

	void Collider3D::SetGravity(bool _gravity)
	{
		PhysXMgr::GetInst()->SetGravity(this, _gravity);
	}

	void Collider3D::AddForce(vec3 _force)
	{
		PhysXMgr::GetInst()->AddForce(this, _force);
	}
	void Collider3D::SetRootBone()
	{
		MTJoint _root_joint = GetMeshRender()->GetMesh()->GetJoint(0);
		Matrix _root_joint_mat = _root_joint.mat_bone;
		SetOffsetPos(vec3(_root_joint_mat._41, _root_joint_mat._42, _root_joint_mat._43));
	}
}