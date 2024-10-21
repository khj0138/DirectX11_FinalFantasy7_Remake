#pragma once

#include "ComputeShader.h"
#include "Texture.h"
#include "ptr.h"

namespace ff7r
{
	class StructuredBuffer;
	//class Texture;
	class BlurShader : public ComputeShader
	{
	public:
		BlurShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group);
		~BlurShader();

		virtual void UpdateData() override;
		virtual void Clear() override;

		void SetCount(UINT _cnt) { count = _cnt; }
		void SetTexSize(vec2 _size) { tex_resolution = _size; }
		void SetTexture(Ptr<Texture> _vertical, Ptr<Texture> _horizon) { bloom_vertical_tex = _vertical; bloom_horizon_tex = _horizon;}

	private:
		Ptr<Texture> bloom_vertical_tex;
		Ptr<Texture> bloom_horizon_tex;

		UINT count;

		vec2 tex_resolution;
	};
}

