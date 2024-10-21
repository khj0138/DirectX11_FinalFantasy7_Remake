#include "pch.h"

#include "Animator3D.h"
#include "AnimatorController.h"
#include "ResourceMgr.h"

#include "AnimationState.h"
namespace ff7r
{
	AnimationState::AnimationState()
		: animator(nullptr)
		, is_wait(false)
		, wait_time(0.0f)
	{
	}

	AnimationState::~AnimationState()
	{
		for (int i = 0; i < transitions.size(); i++)
		{
			if (transitions[i])
			{
				delete transitions[i];
				transitions[i] = nullptr;
			}
		}
	}

	void AnimationState::FinalTick()
	{
		if (animation == nullptr)
		{
			for (auto* transition : transitions)
			{
				animator->ChangeState(transition->next_state, 0.0f, transition->start_frame_idx);
				break;
			}

			return;
		}

		if (is_wait)
		{
			is_wait = animation->PlayPrevFrame(wait_time);
		}
		else
		{
			animation->FinalTick();
		}

		bool _change = false;

		for (auto* transition : transitions)
		{
			if (!transition->always_chek)
			{
				if (transition->check_frame_idx == -1)
				{
					if (!animation->IsFinish())
					{
						continue;
					}
				}
				else if (transition->check_frame_idx != animation->GetFrameIdx())
				{
					continue;
				}
			}

			_change = true;

			for (auto& condition : transition->conditions)
			{
				if (CheckCondition(condition))
				{
					_change = true;
				}
				else
				{
					_change = false;
					break;
				}
			}

			if (_change)
			{
				animator->ChangeState(transition->next_state, animation->GetDuration(), transition->start_frame_idx);
				break;
			}
		}

		if (animation->IsFinish() && !_change && animation->IsLoop())
		{
			animation->Reset();
			//animator->ResetRootMat();
			animator->ResetStartClip();
		}
	}

	AnimationState* AnimationState::CheckTransition()
	{
		bool _change = false;

		for (auto* transition : transitions)
		{
			_change = true;

			for (auto& condition : transition->conditions)
			{
				if (CheckCondition(condition))
				{
					_change = true;
				}
				else
				{
					_change = false;
					break;
				}
			}

			if (_change)
			{
				return transition->next_state;
			}
		}

		return nullptr;
	}

	void AnimationState::UpdateData()
	{
		if (animation != nullptr)
		{
			animation->UpdateData();
		}
	}

	void AnimationState::EnterState(float _wait_time, int _start_idx)
	{
		if (_start_idx + 1 >= animation->GetFrameCount())
		{
			_start_idx = 0;
		}

		animation->SetStartFrame(_start_idx);
		wait_time = _wait_time;
		is_wait = true;
	}

	void AnimationState::AddCondition(Transition* _transition, wstring _param_key, Data _compare, COMPARISON _comparison)
	{
		Parameter* _param = animator->GetParameter(_param_key);

		if (_param)
		{
			Condition _condition;
			_condition.comparsion = _comparison;
			_condition.compare_data = _compare;
			_condition.input_data = _param;
			_transition->conditions.push_back(_condition);
		}
	}

	void AnimationState::Save(FILE* _file)
	{
		SaveWString(GetName(), _file);

		int _cnt = transitions.size();
		fwrite(&_cnt, sizeof(int), 1, _file);

		for (int i = 0; i < _cnt; i++)
		{
			Transition* _transition = transitions[i];
			
			SaveWString(_transition->next_state->GetName(), _file);

			int _cond_cnt = _transition->conditions.size();
			fwrite(&_cond_cnt, sizeof(int), 1, _file);

			for (int j = 0; j < _cond_cnt; j++)
			{
				auto& condi = _transition->conditions[j];

				SaveWString(condi.input_data->GetKey(), _file);
				fwrite(&condi.compare_data, sizeof(Data), 1, _file);
				fwrite(&condi.comparsion, sizeof(COMPARISON), 1, _file);
			}
			fwrite(&_transition->check_frame_idx, sizeof(int), 1, _file);
			fwrite(&_transition->start_frame_idx, sizeof(int), 1, _file);
			fwrite(&_transition->always_chek, sizeof(bool), 1, _file);
		}

		if (animation != nullptr)
		{
			int _check = 1;
			fwrite(&_check, sizeof(int), 1, _file);

			SaveWString(animation->GetKey(), _file);
		}
		else
		{
			int _check = -1;
			fwrite(&_check, sizeof(int), 1, _file);
		}

		fwrite(&node_pos, sizeof(vec2), 1, _file);
		fwrite(&wait_time, sizeof(float), 1, _file);
		fwrite(&is_wait, sizeof(bool), 1, _file);
	}

	void AnimationState::Load(FILE* _file, AnimatorController* _controller)
	{
		wstring _name;
		LoadWString(_name, _file);
		if (_name != GetName())
		{
			// 세이브, 로드 순서 잘못됨, 코드 수정
			assert(nullptr);
		}

		int _cnt;
		fread(&_cnt, sizeof(int), 1, _file);

		for (int i = 0; i < _cnt; i++)
		{
			wstring _key;
			LoadWString(_key, _file);

			Transition* _transition = new Transition();
			_transition->next_state = _controller->GetAnimState(_key);

			int _cond_cnt;
			fread(&_cond_cnt, sizeof(int), 1, _file);

			for (int j = 0; j < _cond_cnt; j++)
			{
				Condition _condition;

				wstring _param_key;
				LoadWString(_param_key, _file);

				_condition.input_data = _controller->GetParameter(_param_key);

				fread(&_condition.compare_data, sizeof(Data), 1, _file);
				fread(&_condition.comparsion, sizeof(COMPARISON), 1, _file);

				_transition->conditions.push_back(_condition);
			}
			fread(&_transition->check_frame_idx, sizeof(int), 1, _file);
			fread(&_transition->start_frame_idx, sizeof(int), 1, _file);
			fread(&_transition->always_chek, sizeof(bool), 1, _file);

			transitions.push_back(_transition);
		}

		int _check;
		fread(&_check, sizeof(int), 1, _file);

		if (_check == 1)
		{
			wstring _anim_key;
			LoadWString(_anim_key, _file);

			// animation = ResourceMgr::GetInst()->MultiFindRes<Animation3D>(_anim_key);
			animation = ResourceMgr::GetInst()->MultiLoad<Animation3D>(_anim_key, _anim_key);
		}


		fread(&node_pos, sizeof(vec2), 1, _file);
		fread(&wait_time, sizeof(float), 1, _file);
		fread(&is_wait, sizeof(bool), 1, _file);
	}

	bool AnimationState::CheckCondition(Condition& condition)
	{
		switch (condition.input_data->type)
		{
		case DATA_TYPE::INT:
		{
			if (condition.comparsion == COMPARISON::EQUALS)
			{
				if (condition.input_data->data.i == condition.compare_data.i)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::NOTEQUAL)
			{
				if (condition.input_data->data.i != condition.compare_data.i)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::LESS)
			{
				if (condition.input_data->data.i < condition.compare_data.i)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::GREATER)
			{
				if (condition.input_data->data.i > condition.compare_data.i)
				{
					return true;
				}
			}
		}
		break;

		case DATA_TYPE::FLOAT:
		{
			if (condition.comparsion == COMPARISON::EQUALS)
			{
				if (condition.input_data->data.f == condition.compare_data.f)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::NOTEQUAL)
			{
				if (condition.input_data->data.f != condition.compare_data.f)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::LESS)
			{
				if (condition.input_data->data.f < condition.compare_data.f)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::GREATER)
			{
				if (condition.input_data->data.f > condition.compare_data.f)
				{
					return true;
				}
			}
		}
		break;

		case DATA_TYPE::BOOL:
		{
			if (condition.comparsion == COMPARISON::C_TRUE)
			{
				if (condition.input_data->data.b)
				{
					return true;
				}
			}
			else if (condition.comparsion == COMPARISON::C_FALSE)
			{
				if (!condition.input_data->data.b)
				{
					return true;
				}
			}
		}
		break;

		case DATA_TYPE::TRIGGER:
		{
			if (condition.input_data->data.b)
			{
				return true;
			}
		}
		break;

		default: break;
		}

		return false;
	}
}