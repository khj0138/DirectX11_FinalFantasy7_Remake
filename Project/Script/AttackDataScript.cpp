#include "pch.h"

#include "AttackDataScript.h"
namespace ff7r
{
	AttackDataScript::AttackDataScript()
		: Script(SCRIPT_TYPE::ATTACKDATASCRIPT)
		, type(ATK_TYPE::DEFAULT)
		, damage(1)
	{
	}

	AttackDataScript::AttackDataScript(UINT _type)
		: Script(_type)
		, type(ATK_TYPE::DEFAULT)
		, damage(1)
	{
	}

	AttackDataScript::~AttackDataScript()
	{
	}

}