#pragma once

#include "BaseState.h"
#include "SoldierScript.h"

namespace ff7r
{
	// ============================= Base State =============================
	class SoldierState : public BaseState
	{
	public:
		SoldierState(SoldierScript* _soldier) { soldier = _soldier; }
		virtual ~SoldierState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		SoldierScript* soldier;
	};

	// ============================= Idle State =============================
	class SoldierIdleState : public SoldierState
	{
	public:
		SoldierIdleState(SoldierScript* _soldier) : SoldierState(_soldier) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
	// ============================= Attack State =============================
	class SoldierAttackState : public SoldierState
	{
	public:
		SoldierAttackState(SoldierScript* _soldier) : SoldierState(_soldier) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
	// ============================= Hit State =============================
	class SoldierHitState : public SoldierState
	{
	public:
		SoldierHitState(SoldierScript* _soldier) : SoldierState(_soldier) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
}

