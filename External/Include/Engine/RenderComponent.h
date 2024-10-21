#pragma once
#include "Component.h"

#include "Mesh.h"
#include "Material.h"
#include "ptr.h"

namespace ff7r
{
    struct MtrlSet
    {
        Ptr<Material>  shared_mtrl;    // 공유 메테리얼
        Ptr<Material>  dynamic_mtrl;   // 공유 메테리얼의 복사본    
        Ptr<Material>  current_mtrl;       // 현재 사용 할 메테리얼
    };

    class RenderComponent : public Component
    {
    public:
        RenderComponent(COMPONENT_TYPE _type);
        ~RenderComponent();

        virtual void Render() = 0;
        virtual void Render(UINT _subset) = 0;
        void RenderShadowmap();

        void SetFrustumCheck(bool _Check) { frustum_check = _Check; }

        void SetMesh(Ptr<Mesh> _mesh);
        void SetMaterial(Ptr<Material> _mtrl, UINT _idx = 0);
        void SetRender(bool _is_render) { is_render = _is_render; }

        Ptr<Mesh> GetMesh() { return mesh; }
        Ptr<Material> GetMaterial(UINT _idx = 0);
        Ptr<Material> GetSharedMaterial(UINT _idx = 0);
        Ptr<Material> GetDynamicMaterial(UINT _idx = 0);

        ULONG64 GetInstID(UINT _mtrl_idx);

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

        UINT GetMtrlCount() { return (UINT)materials.size(); }
        bool IsFrustumCheck() { return frustum_check; }
        bool IsRender() { return is_render; }

    private:
        Ptr<Mesh>               mesh;
        vector<MtrlSet>         materials;

        float                   bounding;
        bool                    frustum_check;
        bool                    dynamice_shadow;
        bool                    is_render;
    };
}
