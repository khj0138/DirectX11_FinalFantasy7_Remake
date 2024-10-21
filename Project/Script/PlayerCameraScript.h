#pragma once
#include <Engine\Script.h>

namespace ff7r
{
	struct CameraMove
	{
		enum class InterpolationType
		{
			LINEAR,
			SPHERICALLY,
			SMOOTHSTEP,
			NONE
		};

		InterpolationType type;

		vec3	pos;
		vec3	rot;
		float	time;
	};

	class PlayerCameraScript :public Script
	{
	public:
		PlayerCameraScript();
		~PlayerCameraScript();

		virtual void Awake() override;
		virtual void Tick() override;

		void SetPlayer(GameObject* _player) { player = _player; }

		void TurnCinematicCamera(wstring _name, int _idx);

		void ChangePlayer(GameObject* _player);

	private:
		CLONE(PlayerCameraScript);

		void CreateCenmatic();
		void Camera3DMove();
		void CameraCinematicMove();
		void CameraChangeMove();

		GameObject* player;
		wstring		cinematic_name;
		vec3		offset_pos;
		vec3		move_start_pos;
		vec3		move_start_rot;

		float		camera_speed;
		float		cinematic_time;
		float		change_time;

		int			cinematic_idx;

		bool		is_cinematic;
		bool		is_change_player;

		map<wstring, vector<CameraMove>> cinematic_move;
	};

}