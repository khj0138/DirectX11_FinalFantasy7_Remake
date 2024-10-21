#pragma once
#include "Entity.h"

#include "GameObject.h"

namespace ff7r
{
    class Transform;
    class MeshRender;
    class Collider3D;

#define GET_OTHER_COMPONENT(Type) Type* Get##Type() {return owner->Get##Type();}


    class Component : public Entity
    {
    public:
        Component(COMPONENT_TYPE _type);
        Component(const Component& _other);
        virtual ~Component();

        virtual Component* Clone() = 0;

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Begin() {}
        virtual void Tick() {}
        virtual void FinalTick() = 0;
        virtual void OnEnable() {}
        virtual void OnDisable() {}

        COMPONENT_TYPE GetType() { return type; }
        GameObject* GetOwner() { return owner; }

        void SetEnabled(bool _enable);

        virtual void SaveToLevelFile(FILE* _file) = 0;
        virtual void LoadFromLevelFile(FILE* _fILE) = 0;

        GET_OTHER_COMPONENT(Transform);
        GET_OTHER_COMPONENT(MeshRender);
        GET_OTHER_COMPONENT(Camera);    
        GET_OTHER_COMPONENT(Collider2D);
        GET_OTHER_COMPONENT(Collider3D);
        GET_OTHER_COMPONENT(Light2D);
        GET_OTHER_COMPONENT(TileMap);
        GET_OTHER_COMPONENT(Animator2D);
        GET_OTHER_COMPONENT(Animator3D);
        GET_OTHER_COMPONENT(ParticleSystem);
        GET_OTHER_COMPONENT(FontRenderer);

    private:
        GameObject* owner;
        const COMPONENT_TYPE type;

        bool enabled;

        friend class GameObject;
    };

}