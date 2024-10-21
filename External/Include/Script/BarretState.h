#pragma once
#include "BaseState.h"
#include "BarretMotionScript.h"

namespace ff7r
{
	// ============================= Base State =============================
	class BarretState : public BaseState
	{
	public:
		BarretState(BarretMotionScript* _barret) { barret = _barret; }
		virtual ~BarretState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		BarretMotionScript* barret;
	};

	class BarretIdleState : public BarretState
	{
	public:
		BarretIdleState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class BarretMoveEnterState : public BarretState
	{
	public:
		BarretMoveEnterState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:
		enum class TURN_TYPE
		{
			NONE,
			L90,
			L180,
			R90,
			R180,
			END
		};

		float rot_y;
	};

	class BarretMoveState : public BarretState
	{
	public:
		BarretMoveState(BarretMotionScript* _barret) : BarretState(_barret)
			, prev_dir(DIR_TYPE::FRONT)
			, is_turn(false)
		{
			rot_y[0] = 0.0f;
			rot_y[1] = Degree2Rad(45.0f);
			rot_y[2] = Degree2Rad(90.0f);
			rot_y[3] = Degree2Rad(135.0f);
			rot_y[4] = Degree2Rad(180.0f);
			rot_y[5] = Degree2Rad(-135.0f);
			rot_y[6] = Degree2Rad(-90.0f);
			rot_y[7] = Degree2Rad(-45.0f);
		}
		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

		void TurnEnd() { is_turn = false; }

	private:
		enum class DIR_TYPE
		{
			FRONT,
			FRONT_RIGHT,
			RIGHT,
			BACK_RIGHT,
			BACK,
			BACK_LEFT,
			LEFT,
			FRONT_LEFT,
			END,
		};

		DIR_TYPE prev_dir;

		float rot_y[(UINT)DIR_TYPE::END];

		bool is_turn;
	};
	class BarretAttackEnterState : public BarretState
	{
	public:
		BarretAttackEnterState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:
		enum class TURN_TYPE
		{
			NONE,
			L120,
			R120,
			END
		};

		float rot_y;
	};
	class BarretAttackState : public BarretState
	{
	public:
		BarretAttackState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:


		int bullet;
		DIR_TYPE prev_dir;
		float rot_y;
	};
	class BarretOverChargeState : public BarretState
	{
	public:
		BarretOverChargeState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
	class BarretAvoidState : public BarretState
	{
	public:
		BarretAvoidState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class BarretAbilityState : public BarretState
	{
	public:
		BarretAbilityState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
	class BarretMagicState : public BarretState
	{
	public:
		BarretMagicState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
	class BarretLimitState : public BarretState
	{
	public:
		BarretLimitState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class BarretItemState : public BarretState
	{
	public:
		BarretItemState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};
	class BarretAIAttackState : public BarretState
	{
	public:
		BarretAIAttackState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:


		int bullet;
		DIR_TYPE prev_dir;
		float rot_y;
	};
	class BarretAIAttackEnterState : public BarretState
	{
	public:
		BarretAIAttackEnterState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:
		enum class TURN_TYPE
		{
			NONE,
			L120,
			R120,
			END
		};

		float rot_y;
	};
	class BarretAIMoveEnterState : public BarretState
	{
	public:
		BarretAIMoveEnterState(BarretMotionScript* _barret) : BarretState(_barret) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:


		int bullet;
		DIR_TYPE prev_dir;
		float rot_y;
	};
	class BarretAIMoveState : public BarretState
	{
	public:
		BarretAIMoveState(BarretMotionScript* _barret) : BarretState(_barret)
			, prev_dir(DIR_TYPE::FRONT)
			, is_turn(false)
		{
		}
		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

		void TurnEnd() { is_turn = false; }
	private:


		DIR_TYPE prev_dir;

		bool is_turn;
	};
	class BarretHitState : public BarretState
	{
	public:
		BarretHitState(BarretMotionScript* _barret) : BarretState(_barret)
		{

		}
		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

}