#pragma once
#include "Entity.h"


#include "Texture.h"
#include "ptr.h"

namespace ff7r
{
    // Multi Render Target
    class MRT : public Entity
    {
    public:
        MRT();
        ~MRT();

        CLONE_DISABLE(MRT);

        void Create(Ptr<Texture>* _textures, UINT _tex_cnt, Ptr<Texture> _ds_tex);
        void SetClearColor(vec4 _clear_color, UINT _idx) { clear_color[_idx] = _clear_color; }
        void ClearTarget();
        void OMSet(bool _is_stay = false);

    private:
        Ptr<Texture>    render_targets[8];
        Ptr<Texture>    ds_tex; // depth_stencil_tex
        UINT            rt_cnt; // render_target_cnt

        vec4            clear_color[8];

        D3D11_VIEWPORT  viewport;
    };

}