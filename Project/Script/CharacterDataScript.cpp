#include "pch.h"

#include "UIMgr.h"
#include "WorldTextUIScript.h"
#include "CharacterDataScript.h"
namespace ff7r
{
	CharacterDataScript::CharacterDataScript()
		: Script(SCRIPT_TYPE::CHARACTERDATASCRIPT)
		, max_hp(1000.f)
		, cur_hp(max_hp)

	{
	}

	CharacterDataScript::CharacterDataScript(UINT _type)
		: Script(_type)
		, max_hp(1000.f)
		, cur_hp(max_hp)
	{
	}

	CharacterDataScript::~CharacterDataScript()
	{
	}

	void CharacterDataScript::Awake()
	{
	}

	void CharacterDataScript::Tick()
	{
		//cur_hp += 100.f * DT;
		//if (cur_hp >= max_hp)
		//{
		//	cur_hp = cur_hp - max_hp;
		//}

		//cur_mp += 1;
		//if (cur_mp >= max_mp)
		//{
		//	cur_mp = cur_mp - max_mp;
		//}

		//atb_gauge += 0.2f * DT;
		//if (atb_gauge >= 2.f)
		//{
		//	atb_gauge = 0.0f;
		//}

		//limit_gauge += 0.05f * DT;
		//if (limit_gauge >= 1.0f)
		//{
		//	limit_gauge = limit_gauge - 1.0f;
		//}
	}
	void CharacterDataScript::Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos)
	{
		if (cur_hp > 0)
		{
			string _str_damage = std::to_string(_damage);
			UIMgr::GetInst()->PrintWorldTextUI(_str_damage, false, true, _contact_pos);

			SetCurHP((cur_hp - _damage) <= 0 ? 0 : cur_hp - _damage);
		}
	}
}