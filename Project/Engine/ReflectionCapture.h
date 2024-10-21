#pragma once

#include "Component.h"
#include "Material.h"

namespace ff7r
{
	class Texture;
	class ReflectionCapture : public Component
	{
	public:
		ReflectionCapture();
		~ReflectionCapture();

		CLONE(ReflectionCapture);

		virtual void Awake() override;
		virtual void Begin() override;
		virtual void FinalTick() override;

		void SetLevel(UINT _level) { capture_info.level = _level; }
		void SetRadius(float _radius) { capture_info.radius = _radius; }

		void StartSetting();
		void EndSetting();

		void UpdateCapture();
		void Capture(int _idx);

		int GetCaptureId() { return capture_id; }
		CaptureInfo& GetCaptureInfo() { return capture_info; }

		virtual void SaveToLevelFile(FILE* _File);
		virtual void LoadFromLevelFile(FILE* _FILE);

	private:
		Ptr<Material> light_mtrl;
		Ptr<Material> merge_light_mtrl;
		Ptr<Material> pre_filter_mtrl;

		Ptr<Texture> filter_environment_map;
		Ptr<Texture> environment_map;
		Ptr<Texture> light_capture;

		CaptureInfo capture_info;

		UINT	width;
		UINT	height;
		UINT	mip_level;
		UINT	cube_array_slot;

		bool	is_capture;
		int		capture_id;

		static int g_capture_id;

		static GameObject g_capture_camera;
		static vector<Ptr<Texture>> g_capture_array;
		static Ptr<Texture> g_capture_cube_array;
		static Ptr<Texture> g_irradiance_array;
		
	};
}

