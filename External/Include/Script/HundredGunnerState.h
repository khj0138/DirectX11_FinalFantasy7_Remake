#pragma once

#include "HundredGunnerScript.h"

#include "BaseState.h"
namespace ff7r
{
	class HundredGunnerState : public BaseState
	{
	public:
		HundredGunnerState(HundredGunnerScript* _hunredgunner) { hunredgunner = _hunredgunner; }
		virtual ~HundredGunnerState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		HundredGunnerScript* hunredgunner;
	};

	class HundredGunnerSleepState : public HundredGunnerState
	{
	public:
		HundredGunnerSleepState(HundredGunnerScript* _hunredgunner) : HundredGunnerState(_hunredgunner) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class HundredGunnerTurnState : public HundredGunnerState
	{
	public:
		HundredGunnerTurnState(HundredGunnerScript* _hunredgunner) : HundredGunnerState(_hunredgunner) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		enum class TURN_TYPE
		{
			L90,
			L180,
			R90,
			R180,
			END
		};
	};

	class HundredGunnerAttackLazerState : public HundredGunnerState
	{
	public:
		HundredGunnerAttackLazerState(HundredGunnerScript* _hunredgunner)
			: HundredGunnerState(_hunredgunner)
			, lazer_time(0.0f)
			, lazer_index(0) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		float lazer_time;
		int lazer_index;
	};

	class HundredGunnerAttackFireState : public HundredGunnerState
	{
	public:
		HundredGunnerAttackFireState(HundredGunnerScript* _hunredgunner)
			: HundredGunnerState(_hunredgunner) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
	};

	class HundredGunnerAttackShootState : public HundredGunnerState
	{
	public:
		HundredGunnerAttackShootState(HundredGunnerScript* _hunredgunner)
			: HundredGunnerState(_hunredgunner) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		bool bfirst = true;
	};

	class HundredGunnerAttackBodyState : public HundredGunnerState
	{
	public:
		HundredGunnerAttackBodyState(HundredGunnerScript* _hunredgunner)
			: HundredGunnerState(_hunredgunner) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class HundredGunnerAttackBeamState : public HundredGunnerState
	{
	public:
		HundredGunnerAttackBeamState(HundredGunnerScript* _hunredgunner)
			: HundredGunnerState(_hunredgunner) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
}

