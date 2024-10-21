#include "pch.h"
#include <Engine\GameObject.h>

#include "PlayerDataScript.h"
#include <Engine\GameObject.h>

#include "GameMgr.h"
#include "ObjectPoolMgr.h"
#include <Engine\LevelMgr.h>
#include <Engine\EventMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\SoundMgr.h>


namespace ff7r
{
	GameMgr::GameMgr()
		: cur_player(nullptr)
	{
	}

	GameMgr::~GameMgr()
	{
	}

	void GameMgr::Init()
	{
	}

	GameObject* GameMgr::GetOtherPlayer(GameObject* _player)
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (_player != players[i])
			{
				return players[i];
			}
		}
		return nullptr;
	}

	void GameMgr::SetDestroyPillar()
	{
		vector<GameObject*> t;
		LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"F59MainPillarC_01_0ShinBuilding", t);

		vector<GameObject*> pillar_t;
		for (GameObject* pillar : t)
		{
			if (pillar->GetChild().size() == 1 && pillar->GetChild()[0]->GetCollider3D())
			{
				pillar_t.push_back(pillar);
			}
		}
		destory_pillar = pillar_t;
	}

	void GameMgr::DestroyPillar()
	{
		if (destory_pillar.size() > 1)
		{
			SoundMgr::GetInst()->Play(L"sound\\HundredGunner\\Battle_Resident [103].wav", SOUND_OBJECT_TYPE::_3D);
		}

		for (int i = 0; i < destory_pillar.size(); i++)
		{
			vec3 pos = destory_pillar[i]->GetTransform()->GetWorldPos();
			pos.y += 2.0f;
			for (int j = 0; j < 15; j++)
			{

				
				GameObject* t = ObjectPoolMgr::GetInst()->GetGameObject(L"Rock 1");
				t->GetTransform()->SetRelativePos(pos);

				t = ObjectPoolMgr::GetInst()->GetGameObject(L"Rock 2");
				t->GetTransform()->SetRelativePos(pos);

				t = ObjectPoolMgr::GetInst()->GetGameObject(L"Rock 3");
				t->GetTransform()->SetRelativePos(pos);
			}



			DestroyObject(destory_pillar[i]);

		}
		destory_pillar.clear();
	}

	void GameMgr::ChangePlayer()
	{
		UINT _idx = 0;
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i] == cur_player)
			{
				_idx = i;
				break;
			}
		}
		_idx = (_idx + 1) % players.size();

		cur_player->GetScript<PlayerDataScript>()->SetFocus(false);
		cur_player = players[_idx];
		cur_player->GetScript<PlayerDataScript>()->SetFocus(true);
	}


	void GameMgr::ChangeLevel(LEVEL_TYPE _type)
	{
		switch(_type)
		{
		case LEVEL_TYPE::TITLE:
		{
			bgm_index = SoundMgr::GetInst()->Play(L"sound\\BGM\\01. Prelude.mp3", SOUND_OBJECT_TYPE::_2D, nullptr, 0.3f, vec2(1.0f, 20.f), 0);
		}
		break;
		case LEVEL_TYPE::STAGE1:
		{
			float i = 0.f;
			SoundMgr::GetInst()->Stop(bgm_index, nullptr);
			bgm_index = SoundMgr::GetInst()->Play(L"sound\\BGM\\02. Opening~Bombing Mission.mp3", SOUND_OBJECT_TYPE::_2D, nullptr, 0.3f, vec2(1.0f, 20.f), 0);
			for (auto player : players)
			{
				player->GetTransform()->SetRelativePos(106.f,3.355f, 3.f + i);
				i += 3.f;
			}
		}
		break;
		case LEVEL_TYPE::STAGE2:
		{
			SoundMgr::GetInst()->Stop(bgm_index, nullptr);
			bgm_index = SoundMgr::GetInst()->Play(L"sound\\BGM\\75. Hurry Faster!.mp3", SOUND_OBJECT_TYPE::_2D, nullptr, 0.3f, vec2(1.0f, 20.f), 0);

			float i = 0.f;
			for (auto player : players)
			{
				player->GetTransform()->SetRelativePos(10.f + i, -22.15f, 70.f);
				player->GetTransform()->FinalTick();
				i += 10.f;
			}
			GameObject* hundred = ResourceMgr::GetInst()->FindRes<Prefab>(L"Prefab\\Hundred_Gunner.pref")->Instantiate();
			hundred->GetTransform()->SetRelativePos(0.f, -22.3f, 78.369f);
			hundred->GetTransform()->SetRelativeRot(0.f, Degree2Rad( -90.f), 0.f);

		}
		break;
		default: break;
		}
	}

	void GameMgr::RegisterPlayer(GameObject* _player)
	{
		if (players.empty())
		{
			cur_player = _player;
			cur_player->GetScript<PlayerDataScript>()->SetFocus(true);
		}

		players.push_back(_player);
	}
}