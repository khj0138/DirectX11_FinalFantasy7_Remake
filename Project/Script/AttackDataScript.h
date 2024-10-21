#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	enum class ATK_TYPE
	{
		DEFAULT,		// 피격 모션 없음(간단한 카메라 흔들림 정도?)
		PUSH_NONE,		// 살짝 흔들리는 피격 모션
		PUSH_WEAK,		// 살짝 밀려나는 피격 모션
		PUSH_STRONG,	// 많이 밀려나는 피격 모션
		UP,
		DOWN,
		BURST,
		LIMIT,
	};

	class AttackDataScript : public Script
	{
	public:
		AttackDataScript();
		AttackDataScript(UINT _type);
		~AttackDataScript();

		CLONE(AttackDataScript);

		void SetAttackData(ATK_TYPE _type, int _dmg) { type = _type; damage = _dmg; }

		ATK_TYPE GetAttackType() { return type; }
		int GetAttackDamage() { return damage; }

	private:
		ATK_TYPE	type;

		int			damage;
	};
}
