#pragma once
#include "Entity.h"


namespace ff7r
{
#define GET_COMPONENT(Type, TYPE) Type* Get##Type() const { return (Type*)components[(UINT)COMPONENT_TYPE::TYPE]; }

    class Component;
    class Transform;
    class MeshRender;
    class ParticleSystem;
    class Collider2D;
    class Collider3D;
    class Animator2D;
    class Animator3D;
    class Light2D;
    class Light3D;
    class Camera;
    class RenderComponent;
    class SkyBox;
    class TileMap;
    class Decal;
    class Script;
    class ReflectionCapture;
    class Socket;
    class TrailRenderer;
    class FontRenderer;

    class GameObject : public Entity
    {
    public:
        GameObject();
        GameObject(const GameObject& _Other);
        ~GameObject();

        CLONE(GameObject);

        void Awake();
        void Start();
        void Begin();
        void Tick();
        virtual void FinalTick();
        void FinalTickModule();
        void Render();
        void RenderShadowMap();
        void OnEnable();
        void OnDisable();

        void AddComponent(Component* _Component);
        void AddChild(GameObject* _Object);

        void SetLayerIndex(int _index) { layer_idx = _index; }
        void SetLayerIndexByName(const wstring& name);

        Component* GetComponent(COMPONENT_TYPE _ComType) { return components[(UINT)_ComType]; }
        const vector<GameObject*>& GetChild() { return child; }
        GameObject* FindChildObject(wstring _name);
        GameObject* GetParent() const { return parent; }

        void SetActive(bool _active);
        bool GetActive() { return is_active; }
        void SetDestroy(bool _destroy) { is_destroy = _destroy; }
        bool GetDestroy() { return is_destroy; }

        RenderComponent* GetRenderComponent() const { return render_component; }
        UINT GetLayerIndex() { return layer_idx; }

        const vector<Script*>& GetScripts() { return scripts; }

        void SetLifeSpan(float _fTime);

        bool IsDead() { return is_dead; }
        bool IsAncestor(GameObject* _Target);

        GET_COMPONENT(Transform, TRANSFORM);
        GET_COMPONENT(MeshRender, MESHRENDER);
        GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
        GET_COMPONENT(Camera, CAMERA);
        GET_COMPONENT(Collider2D, COLLIDER2D);
        GET_COMPONENT(Collider3D, COLLIDER3D);
        GET_COMPONENT(Light2D, LIGHT2D);
        GET_COMPONENT(TileMap, TILEMAP);
        GET_COMPONENT(Animator2D, ANIMATOR2D);
        GET_COMPONENT(Animator3D, ANIMATOR3D);
        GET_COMPONENT(Light3D, LIGHT3D);
        GET_COMPONENT(SkyBox, SKYBOX);
        GET_COMPONENT(Decal, DECAL);
        GET_COMPONENT(ReflectionCapture, REFLECTIONCAPTURE);
        GET_COMPONENT(TrailRenderer, TRAILRENDERER);
        GET_COMPONENT(FontRenderer, FONTRENDERER);
        GET_COMPONENT(Socket, SOCKET);

        template<typename T> T* GetScript();
        template<typename T> T* GetScriptInChildren(); // 자식들 중에 해당 스크립트를 가진 첫번째 오브젝트의 스크립트 반환

    private:
        void DisconnectFromParent();
        void ChangeToChildType();
        void AddParentList();

        GameObject*             parent;
        vector<GameObject*>     child;

        Component*              components[(UINT)COMPONENT_TYPE::END];
        RenderComponent*        render_component;
        vector<Script*>         scripts;

        float                   life_time;
        float                   cur_life_time;

        int                     layer_idx; // 소속된 레이어 인덱스값

        bool                    is_active;
        bool                    is_dead;
        bool                    is_life_span;
        bool                    is_first_tick;
        bool                    is_destroy;

        friend class Layer;
        friend class EventMgr;
    };

    template<typename T>
    inline T* GameObject::GetScript()
    {
        for (size_t i = 0; i < scripts.size(); ++i)
        {
            T* _script = dynamic_cast<T*> (scripts[i]);
            if (_script)
            {
                return _script;
            }
        }

        return nullptr;
    }

    template<typename T>
    inline T* GameObject::GetScriptInChildren()
    {
        T* _script = GetScript<T>();

        if (_script)
        {
            return _script;
        }

        for (size_t i = 0; i < child.size(); ++i)
        {
            _script = child[i]->GetScriptInChildren<T>();

            if (_script)
            {
                return _script;
            }
        }

        return nullptr;
    }
}