#include "pch.h"
#include "PlayerCameraScript.h"

#include <Engine\Transform.h>
#include <Engine\Camera.h>

#include <Engine\RenderMgr.h>
#include <Engine\SoundMgr.h>

#include "GameMgr.h"
#include "func.h"

namespace ff7r
{
	PlayerCameraScript::PlayerCameraScript()
		: Script((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
		, camera_speed(100.f)
		, player(nullptr)
		, offset_pos(vec3(0.0f, 1.5f, -4.5f))
		, is_cinematic(false)
		, cinematic_idx(0)
		, cinematic_time(0.f)
		, is_change_player(false)
		, change_time(0.f)
	{
	}

	PlayerCameraScript::~PlayerCameraScript()
	{
	}

	void PlayerCameraScript::Awake()
	{
		RenderMgr::GetInst()->RegisterEditorCamera(GetCamera());
		GetTransform()->SetRelativeRot(vec3(Degree2Rad(30.f), 0.0f, 0.0f));

		GetCamera()->SetFOV(Degree2Rad(45.f));
		CreateCenmatic();
		SoundMgr::GetInst()->SetListener(GetOwner());
	}

	void PlayerCameraScript::Tick()
	{
		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::_2))
		{
			RenderMgr::GetInst()->RegisterEditorCamera(GetCamera());
			GetTransform()->SetRelativeRot(vec3(Degree2Rad(30.f), 0.0f, 0.0f));
		}
		
		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::X))
		{
			is_change_player = true;
			GameMgr::GetInst()->ChangePlayer();
		}

		if (is_cinematic)
		{
			CameraCinematicMove();
		}
		else if (is_change_player)
		{
			CameraChangeMove();
		}
		else
		{
			Camera3DMove();
		}	
	}

	void PlayerCameraScript::TurnCinematicCamera(wstring _name, int _idx)
	{
		is_cinematic = true;

		cinematic_name = _name;
		cinematic_idx = _idx;

		cinematic_time = 0.f;

		if (cinematic_idx >= cinematic_move.find(cinematic_name)->second.size() - 1)
		{
			is_cinematic = false;
			cinematic_idx -= 1;
		}
	}

	void PlayerCameraScript::CreateCenmatic()
	{
		vector<CameraMove> limit0;

		CameraMove _cm1;

		_cm1.pos = vec3(3.063f, 2.314f, -1.836f);
		_cm1.rot = Degree2Rad(vec3(24.f, -63.f, 0.f));
		_cm1.time = 0.0f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);
		
		_cm1.pos = vec3(3.162f, 2.9f, -1.7f);
		_cm1.rot = Degree2Rad(vec3(24.f, -61.f, 0.f));
		_cm1.time = 15.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(1.418f, 0.814f, -0.633f);
		_cm1.rot = Degree2Rad(vec3(-2.f, -49.f, 0.f));
		_cm1.time = 20.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(1.418f, 0.814f, -0.633f);
		_cm1.rot = Degree2Rad(vec3(-2.f, -49.f, 0.f));
		_cm1.time = 29.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;

		limit0.push_back(_cm1);
		_cm1.pos = vec3(1.416f, 1.121f, -1.f);
		_cm1.rot = Degree2Rad(vec3(-2.f, -56.f, 0.f));
		_cm1.time = 34.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-3.574f, 1.631f, -0.574f);
		_cm1.rot = Degree2Rad(vec3(10.f, 88.f, 0.f));
		_cm1.time = 42.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-3.f, 2.f, -0.574f);
		_cm1.rot = Degree2Rad(vec3(10.f, 88.f, 0.f));
		_cm1.time = 48.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-6.f, 2.3f, -0.574f);
		_cm1.rot = Degree2Rad(vec3(10.f, 88.f, 0.f));
		_cm1.time = 57.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-2.662f, 1.447f, 1.584f);
		_cm1.rot = Degree2Rad(vec3(10.f, 117.f, 0.f));
		_cm1.time = 66.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-3.975f, 1.914f, 0.f);
		_cm1.rot = Degree2Rad(vec3(10.f, 90.f, 0.f));
		_cm1.time = 93.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-5.975f, 1.914f, 0.f);
		_cm1.rot = Degree2Rad(vec3(10.f, 90.f, 0.f));
		_cm1.time = 113.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		_cm1.pos = vec3(-3.147114f, 3.549037f, 0.f);
		_cm1.rot = Degree2Rad(vec3(30.f, 90.f, 0.f));
		_cm1.time = 153.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		limit0.push_back(_cm1);

		cinematic_move.insert({ L"Cloud_Limit0", limit0 });

		vector<CameraMove> _barret_limit0;


		
		_cm1.pos = vec3(1.7f, 1.2f, -1.5f);
		_cm1.rot = Degree2Rad(vec3(0.f, -45.f, 0.f));
		_cm1.time = 0.0f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.7f, 1.5f, 0.0f);
		_cm1.rot = Degree2Rad(vec3(10.f, -90.f, 0.f));
		_cm1.time = 20.0f *0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.5f, 1.9f, 1.0f);
		_cm1.rot = Degree2Rad(vec3(5, -130.f, 0.f));
		_cm1.time = 30.0f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.5f, 1.0f, 1.0f);
		_cm1.rot = Degree2Rad(vec3(-10.f, -145.f, 0.f));
		_cm1.time = 32.0f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.6f, 1.0f, 1.0f);
		_cm1.rot = Degree2Rad(vec3(-10.f, -145.f, 0.f));
		_cm1.time = 35.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.7f, 1.0f, 1.0f);
		_cm1.rot = Degree2Rad(vec3(-10.f, -145.f, 0.f));
		_cm1.time = 47.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.0f, 1.1f, 2.0f);
		_cm1.rot = Degree2Rad(vec3(-10.f, -180.f, 0.f));
		_cm1.time = 50.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(1.0f, 1.1f, 2.0f);
		_cm1.rot = Degree2Rad(vec3(-10.f, -180.f, 0.f));
		_cm1.time = 80.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		/*_cm1.pos = vec3(0.f, 1.2f, 2.0f);
		_cm1.rot = Degree2Rad(vec3(-7.5f, -195.f, 0.f));
		_cm1.time = 75.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(-1.0f, 1.3f, 2.0f);
		_cm1.rot = Degree2Rad(vec3(-5.0f, -210.f, 0.f));
		_cm1.time = 80.0f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(-2.0f, 1.4f, 2.0f);
		_cm1.rot = Degree2Rad(vec3(-2.5f, -225.f, 0.f));
		_cm1.time = 85.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);*/

		_cm1.pos = vec3(-3.0f, 1.5f, 2.0f);
		_cm1.rot = Degree2Rad(vec3(0.0f, -240.f, 0.f));
		_cm1.time = 90.0f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);

		_cm1.pos = vec3(-4.0f, 1.5f, 1.0f);
		_cm1.rot = Degree2Rad(vec3(0.0f, -260.f, 0.f));
		_cm1.time = 95.f * 0.03333334f;
		_cm1.type = CameraMove::InterpolationType::SMOOTHSTEP;
		_barret_limit0.push_back(_cm1);
		cinematic_move.insert({ L"Barret_Limit0", _barret_limit0 });

	}

	void PlayerCameraScript::Camera3DMove()
	{
		if (player)
		{
			vec3 rot = GetTransform()->GetRelativeRot();
			vec2 mouse_dir = KeyMgr::GetInst()->GetMouseDir();
			rot.y += REAL_DT * mouse_dir.x;	
			rot.x -= REAL_DT * mouse_dir.y;
			
			rot.x = MIN(rot.x, Degree2Rad(45.f));
			rot.x = MAX(rot.x, Degree2Rad(-25.f));

			vec3 _player_pos = player->GetTransform()->GetRelativePos();

			XMMATRIX rotX = XMMatrixRotationX(rot.x);
			XMMATRIX rotY = XMMatrixRotationY(rot.y);
			XMMATRIX rotZ = XMMatrixRotationZ(rot.z);

			XMMATRIX rotationMatrix = rotX * rotY * rotZ;
			vec3 _new_pos = XMVector3Transform(offset_pos, rotationMatrix);
			//vec3 _new_pos;
			//_new_pos.x = cos()
			GetTransform()->SetRelativePos(_player_pos + _new_pos);
			GetTransform()->SetRelativeRot(rot);
			
			return;
		}
	}

	void PlayerCameraScript::CameraCinematicMove()
	{
		vector<CameraMove>& cinematic = cinematic_move.find(cinematic_name)->second;

		cinematic_time += DT;

		float _ratio = cinematic_time / (cinematic[cinematic_idx + 1].time - cinematic[cinematic_idx].time);
		_ratio = _ratio > 1.0f ? 1.f : _ratio;

		vec3 _pos;
		vec3 _rot;

		switch (cinematic[cinematic_idx + 1].type)
		{
		case CameraMove::InterpolationType::LINEAR:
		{
			_pos = vec3::Lerp(cinematic[cinematic_idx].pos, cinematic[cinematic_idx + 1].pos, _ratio);
			_rot = vec3::Lerp(cinematic[cinematic_idx].rot, cinematic[cinematic_idx + 1].rot, _ratio);
		}
		break;

		case CameraMove::InterpolationType::SPHERICALLY:
		{
			_pos = Slerp(cinematic[cinematic_idx].pos, cinematic[cinematic_idx + 1].pos, _ratio);
			_rot = Slerp(cinematic[cinematic_idx].rot, cinematic[cinematic_idx + 1].rot, _ratio);
		}
		break;

		case CameraMove::InterpolationType::SMOOTHSTEP:
		{
			_pos = vec3::SmoothStep(cinematic[cinematic_idx].pos, cinematic[cinematic_idx + 1].pos, _ratio);
			_rot = vec3::SmoothStep(cinematic[cinematic_idx].rot, cinematic[cinematic_idx + 1].rot, _ratio);
		}
		break;

		default: break;
		}

		vec3 _player_pos = player->GetTransform()->GetRelativePos();
		float _player_rot_y = player->GetTransform()->GetRelativeRot().y;

		_pos = RotateVec3YAxis(_pos, _player_rot_y);
		_rot.y += _player_rot_y;

		GetTransform()->SetRelativePos(_player_pos + _pos);
		GetTransform()->SetRelativeRot(_rot);
	}
	void PlayerCameraScript::CameraChangeMove()
	{
		


		//GetTransform()->SetRelativeRot(vec3(Degree2Rad(30.f), 0.0f, 0.0f));

		GameObject* _player = GameMgr::GetInst()->GetCurPlayer();
		if (_player != player)
		{
			move_start_pos = GetTransform()->GetRelativePos();
			move_start_rot = GetTransform()->GetRelativeRot();

			move_start_rot.y = fmodf(move_start_rot.y, XM_PI * 2.f);
			player = _player;
		}

	
		//vec3 rot = vec3(Degree2Rad(30.f), 0.0f, 0.0f);
		vec3 rot = vec3(Degree2Rad(30.f), Degree2Rad(90.f), 0.0f);

		vec3 _player_pos = player->GetTransform()->GetRelativePos();

		XMMATRIX rotX = XMMatrixRotationX(rot.x);
		XMMATRIX rotY = XMMatrixRotationY(rot.y);
		XMMATRIX rotZ = XMMatrixRotationZ(rot.z);

		XMMATRIX rotationMatrix = rotX * rotY * rotZ;
		vec3 _new_pos = XMVector3Transform(offset_pos, rotationMatrix);
		
		//vec3 _player_pos = _player->GetTransform()->GetRelativePos();
		//vec3 _player_rot = _player->GetTransform()->GetRelativeRot();
		change_time += DT;

		float _ratio = change_time / 0.3f;
		_ratio = _ratio > 1.0f ? 1.f : _ratio;

		vec3 _pos;
		vec3 _rot;


		_pos = vec3::SmoothStep(move_start_pos, _player_pos + _new_pos, _ratio);
		_rot = vec3::SmoothStep(move_start_rot, rot, _ratio);


		GetTransform()->SetRelativePos(_pos);
		GetTransform()->SetRelativeRot(_rot);

		if (_ratio == 1.0f)
		{
			change_time = 0.0f;
			is_change_player = false;
		}
		
	}
}