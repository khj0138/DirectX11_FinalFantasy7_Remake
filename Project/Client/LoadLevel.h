#pragma once

#include <Engine\Level.h>

namespace ff7r
{
	class LoadLevel : public Level
	{
	public:
		LoadLevel();
		~LoadLevel();


		
		virtual void Tick() override;
		virtual void FinalTick() override;

		void Refresh();
	private:
		void Init();

		void FindFileName(const wstring& _FolderPath, wstring exclude);
		RES_TYPE GetResTypeByExt(const wstring& _relativepath);
		void Load();
		
		vector<wstring> path;

		vector<GameObject*> load;

		Level* next_level;
		Level* temp;

		bool is_first_tick;
		bool complete_load;
		bool register_level;
		bool b;
		bool load_character;
	};

	void RegisterLoadLevelFunc();
	void CreateLoadLevel();
}
