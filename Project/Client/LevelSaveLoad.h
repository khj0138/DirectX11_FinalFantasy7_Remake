#pragma once

namespace ff7r
{
	class Level;
	class Layer;
	class GameObject;


	class LevelSaveLoad
	{
	public:
		static int SaveLevel(const wstring& _LevelPath, Level* _Level, bool _is_overwrite);
		static int SaveGameObject(GameObject* _Object, FILE* _File);

		static Level* LoadLevel(const wstring& _LevelPath, Level* _lv = nullptr);
		static GameObject* LoadGameObject(FILE* _File);

		static void LoadLevelKeep(const wstring& _LevelPath, Level* _lv = nullptr);
		static Level* GetKeepLevel();

		static Level* keep_level;
	};
}

