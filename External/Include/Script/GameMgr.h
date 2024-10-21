#pragma once

#include <Engine\Singleton.h>

namespace ff7r
{
	class GameObject;
	class GameMgr : public Singleton<GameMgr>
	{
	public:
		enum class LEVEL_TYPE
		{
			TITLE,
			STAGE1,
			LOADING,
			STAGE2
		};
		void Init();

		GameObject* GetCurPlayer() { return cur_player; }
		GameObject* GetOtherPlayer(GameObject* _player = GameMgr::GetInst()->cur_player );

		void SetDestroyPillar();
		void DestroyPillar();

		void ChangePlayer();
		bool IsControl(GameObject* _player) { return (cur_player == _player); }

		void ChangeLevel(LEVEL_TYPE _type);

		void RegisterPlayer(GameObject* _player);

	private:
		SINGLE(GameMgr);

		vector<GameObject*> destory_pillar;
		vector<GameObject*> players;
		GameObject* cur_player;

		int bgm_index;
	};
}
