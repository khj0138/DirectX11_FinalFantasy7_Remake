#pragma once

#include "BaseState.h"

namespace ff7r
{
	class StateMachine
	{
	private: BaseState* cur_state = nullptr;

	public:
		StateMachine(BaseState* _state)
		{
			cur_state = _state;
		}

		~StateMachine()
		{
		}

		void ChangeState(BaseState* _state)
		{
			if (_state == cur_state)
			{
				return;
			}

			if (cur_state)
			{
				cur_state->Exit();
				cur_state = nullptr;
			}

			cur_state = _state;
			cur_state->Enter();
		}

		void UpdateState()
		{
			if (cur_state)
			{
				cur_state->Excute();
			}
		}
	
	};
}

