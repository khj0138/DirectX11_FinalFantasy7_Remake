#pragma once
#include "Component.h"

#include "Frustum.h"
namespace ff7r
{
    class Camera : public Component
    {
    public:
        Camera();
        Camera(const Camera& _Other);
        ~Camera();

        CLONE(Camera);

        virtual void Awake() override;
        virtual void Begin() override;
        virtual void FinalTick() override;

        void RegisterMatrix();
        void SortObjects();
        void SortObject(GameObject* _object);
        void SortShadowObject();
        void Render();
        void RenderShadowMap();
        void SetFlashIntensity(float _intensity) { intensity = _intensity; }

        UINT GetLayerMask() { return layer_mask; }

        void SetProjType(PROJ_TYPE _Type) { proj_type = _Type; }
        PROJ_TYPE GetProjType() { return proj_type; }

        void SetScale(float _fScale) { scale = _fScale; }
        float GetScale() { return scale; }

        void SetNear(float _near) { __near = _near; }
        void SetFar(float _far) { __far = _far; }
        float GetFar() { return __far; }

        void SetLayerMask(int _iLayer, bool _Visible);
        void SetLayerMaskAll(bool _Visible);

        void SetFOV(float _Radian) { fov = _Radian; }
        float GetFOV() { return fov; }

        void SetOrthoWidth(float _width) { ortho_width = _width; }
        void SetOrthoHeight(float _height) { ortho_height = _height; }

        float GetorthoWidth() { return ortho_width; }
        float GetOrthoHeight() { return ortho_height; }

        void SetCameraIndex(int _idx);

        const Matrix& GetViewMat() { return mat_view; }
        const Matrix& GetProjMat() { return mat_proj; }

        const Matrix& GetViewInvMat() { return mat_view_inv; }
        const Matrix& GetProjInvMat() { return mat_proj_inv; }

        enum class CameraPostProcess
        {
            BLOOM,
            TONEMAPPING,
            FXAA,
            GRAYSCALE,
            FLASH,
            END
        };

        void SetPostProcess(CameraPostProcess _type, bool _turn_on) { post_process_option[(UINT)_type] = _turn_on; }

    private:
        void Clear();
        void ClearShadow();

        void RenderDeferred();
        void RenderForward();
        void RenderOpaque();
        void RenderMask();
        void RenderDecal();
        void RenderTransparent();
        void RenderPostprocess();
        void RenderUI();

        void Bloom();
        void ToneMapping();
        void FXAA();
        void GrayScale();
        void Flash();

        void CalcViewMat();
        void CalcProjMat();

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;

        Frustum                 frustum;

        float                   aspect_ratio;
        float                   scale;           // Orthograpic 에서 사용하는 카메라 배율
        float                   __far;
        float                   __near;
        float                   fov;
        float                   intensity;

        float                   ortho_width;   
        float                   ortho_height;  

        bool                    post_process_option[(UINT)CameraPostProcess::END];

        PROJ_TYPE               proj_type;

        Matrix                  mat_view;
        Matrix                  mat_view_inv;

        Matrix                  mat_proj;
        Matrix                  mat_proj_inv;

        UINT                    layer_mask;

        int                     camera_idx;          // 카메라 우선순위


        map<ULONG64, vector<InstObj>>		inst_deferred;	 
        map<ULONG64, vector<InstObj>>		inst_forward;
        map<INT_PTR, vector<InstObj>>		inst_single;

        vector<GameObject*>     deferred_obj;
        vector<GameObject*>     deferred_dacal_obj;

        vector<GameObject*>     opaque_obj;
        vector<GameObject*>     mask_obj;
        vector<GameObject*>     decal_obj;
        vector<GameObject*>     transparent_obj;
        vector<GameObject*>     ui_obj;
        vector<GameObject*>     post_obj;

        vector<GameObject*>    shadow_pbj;
    };

}