#pragma once

#include "HundredGunnerRightArmScript.h"

#include "BaseState.h"
namespace ff7r
{
	class HundredGunnerRightArmState : public BaseState
	{
	public:
		HundredGunnerRightArmState(HundredGunnerRightArmScript* _right_arm) { right_arm = _right_arm; }
		virtual ~HundredGunnerRightArmState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		HundredGunnerRightArmScript* right_arm;
	};

	class HundredGunnerRightArmSleepState : public HundredGunnerRightArmState
	{
	public:
		HundredGunnerRightArmSleepState(HundredGunnerRightArmScript* _right_arm) : HundredGunnerRightArmState(_right_arm) {}

		virtual void Enter() override;
	 	virtual void Excute() override;
		virtual void Exit() override;

	};

	class HundredGunnerRightArmReadyState : public HundredGunnerRightArmState
	{
	public:
		HundredGunnerRightArmReadyState(HundredGunnerRightArmScript* _right_arm)
			: HundredGunnerRightArmState(_right_arm)
			, time(0.0f) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		float time;
	};

	class HundredGunnerRightArmFireState : public HundredGunnerRightArmState
	{
	public:
		HundredGunnerRightArmFireState(HundredGunnerRightArmScript* _right_arm)
			: HundredGunnerRightArmState(_right_arm)
			, time(0.0f) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		float time;
	};
}

