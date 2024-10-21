#include "pch.h"

#include "Engine.h"
#include "ResourceMgr.h"

#include "BlurShader.h"
namespace ff7r
{
	BlurShader::BlurShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group)
		: count(0)
		, tex_resolution(vec2(0.0f, 0.0f))
	{
		thread_x_per_group = _thread_x_per_group;
		thread_y_per_group = _thread_y_per_group;
		thread_z_per_group = _thread_z_per_group;

		bloom_vertical_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided8");
		bloom_horizon_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"BloomResolutionDivided8_2");
	}

	BlurShader::~BlurShader()
	{
	}

	void BlurShader::UpdateData()
	{
		if (count % 2 == 0)
		{
			bloom_vertical_tex->UpdateDataCS(1, true);
			bloom_horizon_tex->UpdateDataCS(0, false);
		}
		else
		{
			bloom_vertical_tex->UpdateDataCS(0, false);
			bloom_horizon_tex->UpdateDataCS(1, true);
		}

		mtrl_const_data.arr_int[0] = count;

		mtrl_const_data.arr_float[0] = tex_resolution.x;
		mtrl_const_data.arr_float[1] = tex_resolution.y;


		group_x_cnt = (UINT)(tex_resolution.x / thread_x_per_group) + 1;
		group_y_cnt = (UINT)(tex_resolution.y / thread_y_per_group) + 1;
	}	

	void BlurShader::Clear()
	{
		if (count % 2 == 0)
		{
			bloom_vertical_tex->ClearCS(true);
			bloom_horizon_tex->ClearCS(false);
		}
		else
		{
			bloom_vertical_tex->ClearCS(false);
			bloom_horizon_tex->ClearCS(true);
		}
	}
}