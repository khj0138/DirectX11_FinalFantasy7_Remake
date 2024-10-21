#pragma once
#include "Singleton.h"

#include "ptr.h"
#include "Texture.h"

namespace ff7r
{
    class Camera;
    class Light2D;
    class Light3D;
    class ReflectionCapture;
    class StructuredBuffer;
    class MRT;

    class RenderMgr : public Singleton<RenderMgr>
    {
    public:
        void Init();
        void Render();

        int  RegisterCamera(Camera* _camera, int _idx);
        void RegisterEditorCamera(Camera* _camera) { editor_camera = _camera; }
        void RegisterUpdateCapture(ReflectionCapture* _capture) { update_capture.push_back(_capture); }
        void RegisterCapture(ReflectionCapture* _capture, const CaptureInfo& _info);

        void SetRenderFunc(bool _is_play);

        UINT RegisterLight2D(Light2D* _light2d, const LightInfo& _info);
        UINT RegisterLight3D(Light3D* _light3d, const LightInfo& _info);

        void ClearCamera() { cameras.clear(); }

        void AddDebugShapeInfo(const DebugShapeInfo& _info) { shape_info.push_back(_info); }
        vector<DebugShapeInfo>& GetDebugShapeInfo() { return shape_info; }

        const vector<Light2D*>& GetLight2D() { return light2d; }
        const vector<Light3D*>& GetLight3D() { return light3d; }

        void SetAmbientColor(vec3 _color) { ambient_color = _color; }
        void SetAmbientIntensity(float _intensity) { ambient_intensity = _intensity; }

        Camera* GetMainCam();
        Camera* GetCam(UINT _idx);
        Camera* GetEditorCam() { return editor_camera; }

        void CopyRenderTarget();
        void BlurRenderTarget();

        Ptr<Texture> GetCopyRenderTarget() { return copy_render_target; }
        Ptr<Texture> GetBlurRenderTarget() { return blur_render_target; }

        MRT* GetMRT(MRT_TYPE _type) { return mrt[(UINT)_type]; }

        void ClearMRT();
        void ClearMRT(MRT_TYPE _type);

    private:
        SINGLE(RenderMgr);

        void UpdateData();
        void UpdateCapture();
        void RenderShadowMap();
        void RenderPlay();
        void RenderEditor();
        void Clear();

        void CreateMRT();

        void (RenderMgr::* RENDER_FUNC)(void);

        vector<Camera*>             cameras;
        Camera*                     editor_camera;

        vector<Light2D*>            light2d;
        vector<LightInfo>           light2d_info;
        vector<Light3D*>            light3d;
        vector<LightInfo>           light3d_info;
        StructuredBuffer*           light2d_buff;
        StructuredBuffer*           light3d_buff;

        vector<ReflectionCapture*>  capture;
        vector<ReflectionCapture*>  update_capture;
        vector<CaptureInfo>         capture_info;
        StructuredBuffer*           capture_buff;

        vec3                        ambient_color;
        float                       ambient_intensity;

        Ptr<Texture>                copy_render_target;
        Ptr<Texture>                blur_render_target;
        vector<Ptr<Texture>>        pre_filter_map;

        vector<DebugShapeInfo>      shape_info;

        MRT*                        mrt[(UINT)MRT_TYPE::END];

    };
}
