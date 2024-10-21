#pragma once

#include "BaseState.h"
#include "CloudMoveScript.h"

namespace ff7r
{
	// ============================= Base State =============================
	class CloudState : public BaseState
	{
	public:
		CloudState(CloudMoveScript* _cloud) { cloud = _cloud; }
		virtual ~CloudState() {}

		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;

	protected:
		CloudMoveScript* cloud;
	};

	// ============================= Idle State =============================
	class CloudIdleState : public CloudState
	{
	public:
		CloudIdleState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	// ============================= Move State =============================
	class CloudMoveEnterState : public CloudState
	{
	public:
		CloudMoveEnterState(CloudMoveScript* _cloud) : CloudState(_cloud), rot_y(0.0f) {}

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
		
	class CloudMoveState : public CloudState
	{
	public:
		CloudMoveState(CloudMoveScript* _cloud) : CloudState(_cloud) 
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
		int sound_idx;
		bool is_turn;
	};
	// ============================= Attack State =============================
	class CloudAttackState : public CloudState
	{
	public:
		CloudAttackState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;

	private:
		wstring attack_name;
	};	

	// ============================= Hit State =============================
	class CloudHitState : public CloudState
	{
	public:
		CloudHitState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	// ============================= Avoid State =============================
	class CloudAvoidState : public CloudState
	{
	public:
		CloudAvoidState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	// ============================= Ability State =============================
	class CloudAbilityState : public CloudState
	{
	public:
		CloudAbilityState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	// ============================= Magic State =============================
	class CloudMagicState : public CloudState
	{
	public:
		CloudMagicState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	// ============================= Limit State =============================
	class CloudLimitState : public CloudState
	{
	public:
		CloudLimitState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

	class CloudAIAttackState : public CloudState
	{
	public:
		CloudAIAttackState(CloudMoveScript* _Cloud) : CloudState(_Cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:
		wstring attack_name;
	};
	class CloudAIMoveEnterState : public CloudState
	{
	public:
		CloudAIMoveEnterState(CloudMoveScript* _cloud) : CloudState(_cloud), rot_y(0.0f) {}

		enum class TURN_TYPE
		{
			NONE,
			L90,
			L180,
			R90,
			R180,
			END
		};

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	private:
		float rot_y;
	};
	class CloudAIMoveState : public CloudState
	{
	public:
		CloudAIMoveState(CloudMoveScript* _cloud) : CloudState(_cloud)
			, prev_dir(MOVE_DIR_TYPE::FRONT)
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
		enum class MOVE_DIR_TYPE
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

		MOVE_DIR_TYPE prev_dir;

		float rot_y[(UINT)MOVE_DIR_TYPE::END];

		bool is_turn;
	};


	class CloudItemState : public CloudState
	{
	public:
		CloudItemState(CloudMoveScript* _cloud) : CloudState(_cloud) {}

		virtual void Enter() override;
		virtual void Excute() override;
		virtual void Exit() override;
	};

}

