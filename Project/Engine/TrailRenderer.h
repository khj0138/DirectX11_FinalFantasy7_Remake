#pragma once
#include "RenderComponent.h"

#include <deque>
namespace ff7r
{
    class TrailRenderer : public RenderComponent
    {
    public:
        TrailRenderer();
        ~TrailRenderer();

        CLONE(TrailRenderer);

        virtual void FinalTick() override;
        virtual void Render() override;
        virtual void Render(UINT _subset) override {}

        void SetTrailColor(vec4 _color);
        void SetTrailSocket(GameObject* _a, GameObject* _b) { trail_socket_a = _a; trail_socket_b = _b;}
        void SetStartTrailSocket(GameObject* _start) { trail_socket_a = _start; }
        void SetEndTrailSocket(GameObject* _end) { trail_socket_b = _end; }

    private:
        GameObject*         trail_socket_a;
        GameObject*         trail_socket_b;

        std::deque<vec3>    socket_pos;
        vector<Vtx>         vertex;
        UINT                vtx_cnt;
    };

}