#pragma once

#include "HomingLaserScript.h"

#include "BaseState.h"
namespace ff7r
{
	class HomingLaserState: public BaseState
	{
	public:
		HomingLaserState(HomingLaserScript* _laser) { homing_laser = _laser; }
		virtual ~HomingLaserState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		HomingLaserScript* homing_laser;
	};

	class HomingLaserSleepState : public HomingLaserState
	{
	public:
		HomingLaserSleepState(HomingLaserScript* _laser) : HomingLaserState(_laser) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class HomingLaserLaunchState : public HomingLaserState
	{
	public:
		HomingLaserLaunchState(HomingLaserScript* _laser) : HomingLaserState(_laser), time(0.0f) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		float time;
	};

	class HomingLaserShootState : public HomingLaserState
	{
	public:
		HomingLaserShootState(HomingLaserScript* _laser) : HomingLaserState(_laser) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		vec3	orgin;
		float	time;
	};
}

