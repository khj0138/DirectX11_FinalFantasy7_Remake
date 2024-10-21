#include "pch.h"
#include "LevelMgr.h"

#include "Level.h"
#include "Layer.h"
// == test
#include "KeyMgr.h"

namespace ff7r
{
	LevelMgr::LevelMgr()
		: cur_level(nullptr)
		, is_destroy(false)
	{
	}

	LevelMgr::~LevelMgr()
	{
		cur_level = nullptr;
		LevelMgr::GetInst()->SetDestroy();
		for (int i = 0; i < levels.size(); i++)
		{
			levels[i]->SetDestroy();
			delete levels[i];
			levels[i] = nullptr;
		}
		//if (nullptr != cur_level)
		//	delete cur_level;
	}

	void LevelMgr::Init()
	{
		cur_level = new Level;
		cur_level->SetName(L"Default Level");
		cur_level->ChangeState(LEVEL_STATE::STOP);
		levels.push_back(cur_level);

	}

	void LevelMgr::Tick()
	{
		cur_level->Clear();

		if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::P))
		{
			KeyMgr::GetInst()->ChangeGameMode();
		}


		if (LEVEL_STATE::PLAY == cur_level->GetState())
		{
			//cur_level->Tick();
		}
		cur_level->Tick();
		cur_level->FinalTick();
	}


	GameObject* LevelMgr::FindObjectByName(const wstring& _name)
	{
		return cur_level->FindObjectByName(_name);
	}

	/*void LevelMgr::EraseObjectByName(const wstring& _name)
	{
		cur_level->EraseObjectByName(_name);
	}*/

	void LevelMgr::FindObjectByName(const wstring& _name, vector<GameObject*>& _out)
	{
		cur_level->FindObjectByName(_name, _out);
	}

	GameObject* LevelMgr::FindParentObjectByName(const wstring& _name)
	{
		return cur_level->FindParentObjectByName(_name);
	}

	void LevelMgr::FindParentObjectByName(const wstring& _name, vector<GameObject*>& _out)
	{
		cur_level->FindParentObjectByName(_name, _out);
	}

	void LevelMgr::ChangeLevel(Level* _next_level)
	{
		if (nullptr != cur_level)
		{
			for (auto iter = levels.begin(); iter != levels.end();)
			{
				if (*iter == cur_level)
				{
					iter = levels.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			delete cur_level;
			cur_level = nullptr;
		}

		cur_level = _next_level;
		cur_level->ChangeState(LEVEL_STATE::STOP);
		for (int i = 0; i < save_objects.size(); i++)
		{
			cur_level->MoveGameObject(save_objects[i]);
		}
		save_objects.clear();
	}

	void LevelMgr::ChangeLevel(Level* _prev_level, Level* _next_level)
	{
		if (nullptr != _prev_level)
		{
			for (auto iter = levels.begin(); iter != levels.end();)
			{
				if (*iter == _prev_level)
				{
					iter = levels.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			delete _prev_level;
			_prev_level = nullptr;
		}

		//cur_level = _next_level;
		for (int i = 0; i < save_objects.size(); i++)
		{
			_next_level->MoveGameObject(save_objects[i]);
		}
		save_objects.clear();
	}

	void LevelMgr::ChangeLevel(int idx)
	{
		Level* _next = levels[idx];
		if (nullptr != cur_level)
		{
			for (auto iter = levels.begin(); iter != levels.end();)
			{
				if (*iter == cur_level)
				{
					iter = levels.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			delete cur_level;
			cur_level = nullptr;
		}

		cur_level = _next;
		for (int i = 0; i < save_objects.size(); i++)
		{
			cur_level->MoveGameObject(save_objects[i]);
		}
		save_objects.clear();
	}

	void LevelMgr::ChangeLevelNotDelete(Level* _level)
	{
		levels.push_back(_level);
		cur_level = _level;
	}


}