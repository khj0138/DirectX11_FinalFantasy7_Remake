#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	enum class ATK_TYPE
	{
		DEFAULT,		// �ǰ� ��� ����(������ ī�޶� ��鸲 ����?)
		PUSH_NONE,		// ��¦ ��鸮�� �ǰ� ���
		PUSH_WEAK,		// ��¦ �з����� �ǰ� ���
		PUSH_STRONG,	// ���� �з����� �ǰ� ���
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
