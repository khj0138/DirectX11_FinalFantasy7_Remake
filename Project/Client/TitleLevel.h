#pragma once

#include <Engine\Level.h>

namespace ff7r
{
    class Layer;
    class GameObject;
    class TitleLevel : public Level
    {
    public:
        TitleLevel();
        ~TitleLevel();

        CLONE(TitleLevel);

        virtual void Tick() override;
        virtual void FinalTick() override;


        

    private:
        void Init();
        void Load();

        void Create();

        void FindFileName(const wstring& _FolderPath, wstring exclude);
        RES_TYPE GetResTypeByExt(const wstring& _relativepath);

        vector<wstring> path;

        Level* next_level;

        GameObject* button;

        bool is_first_tick;
        bool complete_load;
        bool register_level;
        bool b;
        bool load_character;
    };


    void CreateTitleLevel();
}
