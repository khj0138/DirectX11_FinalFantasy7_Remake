#include "pch.h"

#include "PlayerDataScript.h"

namespace ff7r
{
	PlayerDataScript::PlayerDataScript()
		: CharacterDataScript(SCRIPT_TYPE::PLAYERDATASCRIPT)
		, max_mp(100)
		, cur_mp(max_mp)
		, atb_gauge(2.f)
		, limit_gauge(1.f)
		, command(nullptr)
		, is_focus(false)
		, target(nullptr)
	{
	}
	PlayerDataScript::PlayerDataScript(UINT _type)
		: CharacterDataScript(_type)
		, max_mp(100)
		, cur_mp(max_mp)
		, atb_gauge(2.f)
		, limit_gauge(1.f)
		, command(nullptr)
		, is_focus(false)
		, target(nullptr)
	{
	}
	PlayerDataScript::~PlayerDataScript()
	{
	}
	void PlayerDataScript::ClearCommand()
	{
		if (command)
		{
			delete command;
		}
		command = nullptr;	
	}

	void PlayerDataScript::ReciveCommandData(CommandData* _data)
	{
		ClearCommand();
		SetCommand(_data);

		GetAnimator3D()->SetBool(L"Move_Param", false);
	}

	void PlayerDataScript::IncreaseAtbGauge(float _amount)
	{
		atb_gauge = (atb_gauge + _amount) > 2.0f ? 2.0f : atb_gauge + _amount;
	}
	void PlayerDataScript::IncreaseLimitGauge(float _amount)
	{
		limit_gauge = (limit_gauge + _amount) > 1.0f ? 1.0f : limit_gauge + _amount;
	}
}