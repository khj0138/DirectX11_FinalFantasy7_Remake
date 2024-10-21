#pragma once
#include "Entity.h"

namespace ff7r
{
    class Layer;
    class GameObject;
    class Level : public Entity
    {
    public:
        Level();
        ~Level();

        void Begin();
        virtual void Tick();
        virtual void FinalTick();
        void Test();

        Layer* FindLayerByName(const wstring& _name);
        Layer* GetLayer(int _layer_idx) { assert(!(_layer_idx < 0)); return layers[_layer_idx]; }
        int FindLayerIdxByName(const wstring& _name);
        wstring FindLayerNameByIdx(int _idx);

        GameObject* FindParentObjectByName(const wstring& _name);
        void FindParentObjectByName(const wstring& _name, vector<GameObject*>& _result);
        GameObject* FindObjectByName(const wstring& _name);
        void FindObjectByName(const wstring& _name, vector<GameObject*>& _result);
        void FindObjectByNameT(const wstring& _name, vector<GameObject*>& _result, GameObject* _t);
        //void EraseObjectByName(const wstring& _name);

        void AddGameObject(GameObject* _object, int _layer_idx, bool _follow_parent = false);
        void AddGameObject(GameObject* _object, const wstring& _layer_name, bool _follow_parent = false);
        void MoveGameObject(GameObject* _object);

        void ChangeState(LEVEL_STATE _State);
        void SetDestroy();
        LEVEL_STATE GetState() { return state; }

    private:
        CLONE(Level);

        void Clear();

        Layer* layers[MAX_LAYER];
        LEVEL_STATE     state;

        vector<std::tuple<int, GameObject*, bool>> spawn_objects;

        friend class LevelMgr;
    };

}