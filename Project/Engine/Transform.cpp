#include "pch.h"
#include "Transform.h"

#include "Device.h"
#include "ConstBuffer.h"

#include "Animator3D.h"
#include "StructuredBuffer.h"
#include "MeshRender.h"

namespace ff7r
{
	Transform::Transform()
		: Component(COMPONENT_TYPE::TRANSFORM)
		, relative_scale(vec3(1.f, 1.f, 1.f))
		, is_absolute(true)
		, is_absolute_rot(false)
		, relative_dir{
			  vec3(1.f, 0.f, 0.f)
			, vec3(0.f, 1.f, 0.f)
			, vec3(0.f, 0.f, 1.f) }
	{
		SetName(L"Transform");
	}

	Transform::~Transform()
	{
	}

	void Transform::FinalTick()
	{
		mat_world_scale = XMMatrixIdentity();
		mat_world_scale = XMMatrixScaling(relative_scale.x, relative_scale.y, relative_scale.z);

		GameObject* pParent = GetOwner()->GetParent();

		Matrix matRot = XMMatrixIdentity();
		matRot = XMMatrixRotationX(relative_rotation.x);

		if (pParent)
		{
			matRot *= XMMatrixRotationY(relative_rotation.y);
		}
		else
		{
			matRot *= XMMatrixRotationY(relative_rotation.y);
		}
		//matRot *= XMMatrixRotationY(relative_rotation.y - 90.f * 3.141592 / 180.f);
		matRot *= XMMatrixRotationZ(relative_rotation.z);
		
		Matrix _matRot = XMMatrixIdentity();
		_matRot = XMMatrixRotationX(relative_rotation.x);
		_matRot *= XMMatrixRotationY(relative_rotation.y);
		_matRot *= XMMatrixRotationZ(relative_rotation.z);

		Matrix matTranslation = XMMatrixTranslation(relative_pos.x, relative_pos.y, relative_pos.z);

		mat_world = mat_world_scale * matRot * matTranslation;

		vec3 vDefaultDir[3] = {
			  vec3(1.f, 0.f, 0.f)
			, vec3(0.f, 1.f, 0.f)
			, vec3(0.f, 0.f, 1.f)
		};

		for (int i = 0; i < 3; ++i)
		{
			world_dir[i] = relative_dir[i] = XMVector3TransformNormal(vDefaultDir[i], _matRot);
		}

		// 부모 오브젝트 확인
		
		if (pParent)
		{
			if (is_absolute_rot)
			{
				const Matrix& parent_mat = pParent->GetTransform()->mat_world;

				mat_world = mat_world * XMMatrixTranslation(parent_mat._41, parent_mat._42, parent_mat._43);
			}
			else if (is_absolute)
			{
				Matrix matParentWorld = pParent->GetTransform()->mat_world;
				Matrix matParentScale = pParent->GetTransform()->mat_world_scale;
				Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

				// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
				mat_world = mat_world * matParentScaleInv * matParentWorld;
			}
			else
			{
				mat_world_scale *= pParent->GetTransform()->mat_world_scale;
				mat_world *= pParent->GetTransform()->mat_world;
			}


			for (int i = 0; i < 3; ++i)
			{
				world_dir[i] = XMVector3TransformNormal(vDefaultDir[i], mat_world);
				world_dir[i].Normalize();
			}
		}

		mat_world_inv = XMMatrixInverse(nullptr, mat_world);
	}

	void Transform::UpdateData()
	{
		// 위치값을 상수버퍼에 전달 및 바인딩		
		ConstBuffer* pTransformBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

		g_transform_data.mat_world = mat_world;
		g_transform_data.mat_world_inv = mat_world_inv;
		g_transform_data.mat_wv = g_transform_data.mat_world * g_transform_data.mat_view;
		g_transform_data.mat_wvp = g_transform_data.mat_wv * g_transform_data.mat_proj;

		pTransformBuffer->SetData(&g_transform_data);
		pTransformBuffer->UpdateData();
	}

	void Transform::CalcWorldDir()
	{
		Quaternion	_rotation;
		vec3		_position;
		vec3		_scale;

		mat_world.Decompose(_scale, _rotation, _position);

		Matrix _mat = {};
		_mat = _mat.CreateFromQuaternion(_rotation);

		vec3 _dir[3] = { vec3(1.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 1.f) };

		for (int i = 0; i < 3; ++i)
		{
			world_dir[i] = XMVector3TransformNormal(_dir[i], _mat);
		}
	}

	void Transform::SaveToLevelFile(FILE* _File)
	{
		fwrite(&relative_pos, sizeof(vec3), 1, _File);
		fwrite(&relative_scale, sizeof(vec3), 1, _File);
		fwrite(&relative_rotation, sizeof(vec3), 1, _File);
		fwrite(&is_absolute, sizeof(bool), 1, _File);
	}

	void Transform::LoadFromLevelFile(FILE* _FILE)
	{
		fread(&relative_pos, sizeof(vec3), 1, _FILE);
		fread(&relative_scale, sizeof(vec3), 1, _FILE);
		fread(&relative_rotation, sizeof(vec3), 1, _FILE);
		fread(&is_absolute, sizeof(bool), 1, _FILE);
	}
}