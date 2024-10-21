#pragma once
#include "Singleton.h"

namespace ff7r
{
    class Level;
    class GameObject;

    class LevelMgr : public Singleton<LevelMgr>
    {
    public:
        void Init();
        void Tick();


        GameObject* FindParentObjectByName(const wstring& _name);
        void FindParentObjectByName(const wstring& _name, vector<GameObject*>& _out);
        Level* GetCurLevel() { return cur_level; }
        Level* GetLevel(int _idx) { return levels[_idx]; }
        GameObject* FindObjectByName(const wstring& _name);
        
        void SaveObject(GameObject* _object) { save_objects.push_back(_object); }
        void ClearSavedObjects() { save_objects.clear(); }
        vector<GameObject*>& GetSavedObjects() { return save_objects; }

        void FindObjectByName(const wstring& _name, vector<GameObject*>& _out);

        void ChangeLevel(Level* _prev_level, Level* _next_level);
        void ChangeLevel(Level* _next_level);
        void ChangeLevel(int idx);
        void CreateLevel(Level* _level) { levels.push_back(_level); }
        void ChangeLevelNotDelete(Level* _level);

        void SetDestroy() { is_destroy = true; };
        bool GetDestroy() { return is_destroy; }

    private:
        SINGLE(LevelMgr);

        Level* cur_level;
        vector<Level*> levels;
        vector<GameObject*> save_objects;
        bool is_destroy;
    };
}