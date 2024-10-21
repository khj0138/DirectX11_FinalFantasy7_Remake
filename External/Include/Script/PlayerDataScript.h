#pragma once

#include "CharacterDataScript.h"

namespace ff7r
{
	struct CommandData
	{
		enum class COMMAND_TYPE
		{
			ABILITY,
			MAGIC,
			ITEM,
			LIMIT
		};

		COMMAND_TYPE type;

		GameObject* target;
		wstring		name;

		int			cost;
	};

	struct SkillData
	{
		wstring name;
		wstring info_text;

		int cost;
	};

	class PlayerDataScript : public CharacterDataScript
	{
	public:
		PlayerDataScript();
		PlayerDataScript(UINT _type);
		~PlayerDataScript();

		CLONE(PlayerDataScript);

		void SetAbilityData(SkillData _data) { abilities.push_back(_data); }
		void SetMagicData(SkillData _data) { magics.push_back(_data); }
		void SetItemData(SkillData _data) { items.push_back(_data); }
		void SetLimitData(SkillData _data) { limits.push_back(_data); }

		void SetCommand(CommandData* _command) { command = _command; }
		void ClearCommand();
		void ReciveCommandData(CommandData* _data);
		void SetFocus(bool _is_focus) { is_focus = _is_focus; }
		void SetTarget(GameObject* _target) { target = _target; }
		void IncreaseAtbGauge(float _amount);
		void IncreaseLimitGauge(float _amount);

		const vector<SkillData>& GetAbilityData() { return abilities; }
		const vector<SkillData>& GetMagicData() { return magics; }
		const vector<SkillData>& GetItemData() { return items; }
		const vector<SkillData>& GetLimitData() { return limits; }

		float GetMaxMP() { return max_mp; }
		float GetCurMP() { return cur_mp; }
		float GetAtbGauge() { return atb_gauge; }
		float GetLimitGauge() { return limit_gauge; }

		CommandData* GetCommand() { return command; }

	protected:
		vector<SkillData>		abilities;
		vector<SkillData>		magics;
		vector<SkillData>		items;
		vector<SkillData>		limits;

		CommandData* command;
		GameObject* target;

		float	atb_gauge;
		float	limit_gauge;

		int		max_mp;
		int		cur_mp;

		bool	is_focus;
	};
}
