#pragma once

#include <Engine\Script.h>
#include "AttackDataScript.h"
namespace ff7r
{
	class CharacterDataScript : public Script
	{
	public:
		CharacterDataScript();
		CharacterDataScript(UINT _type);
		~CharacterDataScript();

		CLONE(CharacterDataScript);

		virtual void Awake() override;
		virtual void Tick() override;

		int GetMaxHP() { return max_hp; }
		int GetCurHP() { return cur_hp; }
		void SetCurHP(int _hp) { cur_hp = _hp; }
		void SetMaxHP(int _hp) { max_hp = cur_hp = _hp; }

		virtual void Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos = vec3(0.0f, 0.0f, 0.0f));
	
	private:
		int	max_hp;
		int	cur_hp;
	};
}

