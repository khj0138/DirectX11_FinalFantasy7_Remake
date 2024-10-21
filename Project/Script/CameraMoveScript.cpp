#include "pch.h"
#include "CameraMoveScript.h"

#include <Engine\Transform.h>
#include <Engine\Camera.h>

#include <Engine\RenderMgr.h>
#include "func.h"

namespace ff7r
{
	CameraMoveScript::CameraMoveScript()
		: Script((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
		, camera_speed(1.f)
	{
	}

	CameraMoveScript::~CameraMoveScript()
	{
	}

	void CameraMoveScript::Tick()
	{
		if (PROJ_TYPE::ORTHOGRAPHIC == GetCamera()->GetProjType())
			Camera2DMove();
		else
			Camera3DMove();
	}

	void CameraMoveScript::Camera2DMove()
	{
		// 키 입력에 따른 카메라 이동
		vec3 pos = GetTransform()->GetRelativePos();

		float speed = camera_speed;
		if (KEY_PRESSED(KEY::LSHIFT))
			speed *= 5.f;

		if (KEY_PRESSED(KEY::W))
		{
			pos.y += DT * speed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			pos.y -= DT * speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			pos.x -= DT * speed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			pos.x += DT * speed;
		}


		if (KEY_PRESSED(KEY::_1))
		{
			float scale = GetCamera()->GetScale();
			scale += DT * 1.f;
			GetCamera()->SetScale(scale);
		}

		if (KEY_PRESSED(KEY::_2))
		{
			float scale = GetCamera()->GetScale();
			scale -= DT * 1.f;
			GetCamera()->SetScale(scale);
		}

		GetTransform()->SetRelativePos(pos);
	}

	void CameraMoveScript::Camera3DMove()
	{
		if (RenderMgr::GetInst()->GetEditorCam() != GetCamera())
		{
			return;
		}

		if (!IsMainFocused() || IsImGuizmoUsed())
			return;
		if (IsImGuizmoUsed())
		{
			int a = 0;
		}
		//if (!GetFocus() && IsImGuizmoUsed())
		//{
		//	return;
		//}

		vec3 pos = GetTransform()->GetRelativePos();
		vec3 rot = GetTransform()->GetRelativeRot();

		vec3 front = GetTransform()->GetRelativeDir(DIR_TYPE::FRONT);
		vec3 up = GetTransform()->GetRelativeDir(DIR_TYPE::UP);
		vec3 right = GetTransform()->GetRelativeDir(DIR_TYPE::RIGHT);

		float speed = camera_speed;

		if (KEY_PRESSED(KEY::LSHIFT))
			speed *= 7.f;

		int wheel_value = KeyMgr::GetInst()->GetMouseWheel();
		if (wheel_value)
		{
			pos += front * wheel_value * 0.01f * speed;
		}

		if (KEY_PRESSED(KEY::LBTN))
		{
			vec2 mouse_dir = KeyMgr::GetInst()->GetMouseDir();
			pos -= (mouse_dir.x * right + mouse_dir.y * up) * 0.02f * speed;
		}
		else if (KEY_PRESSED(KEY::RBTN))
		{
			vec2 mouse_dir = KeyMgr::GetInst()->GetMouseDir();
			rot.y += REAL_DT * mouse_dir.x;
			rot.x -= REAL_DT * mouse_dir.y;
		}

		GetTransform()->SetRelativePos(pos);
		GetTransform()->SetRelativeRot(rot);

		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::_3))
		{
			GetTransform()->SetRelativePos(0.0f, 0.0f, 0.0f);
			GetTransform()->SetRelativeRot(0.0f, 0.0f, 0.0f);
		}
	}
}