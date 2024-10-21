#pragma once
#include "BaseState.h"
#include "BarretWeaponScript.h"

namespace ff7r
{
	// ============================= Base State =============================
	class BarretWeaponState : public BaseState
	{
	public:
		BarretWeaponState(BarretWeaponScript* _weapon) { weapon = _weapon; }
		virtual ~BarretWeaponState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		BarretWeaponScript* weapon;
	};
	class BarretSleepState : public BarretWeaponState
	{
	public:
		BarretSleepState(BarretWeaponScript* _weapon) : BarretWeaponState(_weapon) {}
		virtual ~BarretSleepState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:

	};
	class BarretNormalAtkState : public BarretWeaponState
	{
	public:
		BarretNormalAtkState(BarretWeaponScript* _weapon) : BarretWeaponState(_weapon) {}
		virtual ~BarretNormalAtkState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;



		void CreateObjects();

	private:
		int shoot_idx;

	};

	class BarretUpperCutState : public BarretWeaponState
	{
	public:
		BarretUpperCutState(BarretWeaponScript* _weapon) : BarretWeaponState(_weapon) {}
		virtual ~BarretUpperCutState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

		void CreateObjects();

	private:
		bool charging;
		GameObject* _upper_cut;

	};
	class BarretSmackDownState : public BarretWeaponState
	{
	public:
		BarretSmackDownState(BarretWeaponScript* _weapon) : BarretWeaponState(_weapon) {}
		virtual ~BarretSmackDownState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

		void CreateObjects();

	private:
		int shoot_idx;

	};
	class BarretMaximumFuryState : public BarretWeaponState
	{
	public:
		BarretMaximumFuryState(BarretWeaponScript* _weapon) : BarretWeaponState(_weapon) {}
		virtual ~BarretMaximumFuryState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

		void CreateObjects();

	private:
		int shoot_idx;
		int sound_idx;
		GameObject* flame_particle;

	};
	class BarretLimitAtkState : public BarretWeaponState
	{
	public:
		BarretLimitAtkState(BarretWeaponScript* _weapon) : BarretWeaponState(_weapon) {}
		virtual ~BarretLimitAtkState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

		void CreateObjects();

	private:
		int shoot_idx;
		GameObject* limit_atk_obj;

	};


}