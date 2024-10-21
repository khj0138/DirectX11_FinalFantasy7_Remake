#pragma once
#include "BaseState.h"
#include "SoldierWeaponScript.h"

namespace ff7r
{
	// ============================= Base State =============================
	class SoldierWeaponState : public BaseState
	{
	public:
		SoldierWeaponState(SoldierWeaponScript* _weapon) { weapon = _weapon; }
		virtual ~SoldierWeaponState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		SoldierWeaponScript* weapon;
	};
	class SoldierSleepState : public SoldierWeaponState
	{
	public:
		SoldierSleepState(SoldierWeaponScript* _weapon) : SoldierWeaponState(_weapon) {}
		virtual ~SoldierSleepState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:

	};
	class SoldierNormalAtkState : public SoldierWeaponState
	{
	public:
		SoldierNormalAtkState(SoldierWeaponScript* _weapon) : SoldierWeaponState(_weapon) {}
		virtual ~SoldierNormalAtkState() {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;



		void CreateObjects();

	private:
		int shoot_idx;

	};


}