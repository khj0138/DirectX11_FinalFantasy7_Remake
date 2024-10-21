#include "pch.h"
#include "SetColorShader.h"

#include "Texture.h"

namespace ff7r
{
	SetColorShader::SetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	{
		thread_x_per_group = _iGroupPerThreadX;
		thread_y_per_group = _iGroupPerThreadY;
		thread_z_per_group = _iGroupPerThreadZ;
	}

	SetColorShader::~SetColorShader()
	{
	}

	void SetColorShader::SetTargetTexture(Ptr<Texture> _Tex)
	{
		target_tex = _Tex;

		if (nullptr != target_tex)
		{
			vec2 vResolution = vec2(target_tex->Width(), target_tex->Height());
			mtrl_const_data.arr_vec2[0] = vResolution;
		}
	}

	void SetColorShader::UpdateData()
	{
		target_tex->UpdateDataCS(0, false);

		// 그룹 개수 계산
		group_x_cnt = (target_tex->Width() / thread_x_per_group) + 1;
		group_y_cnt = (target_tex->Height() / thread_y_per_group) + 1;
		group_z_cnt = 1;
	}

	void SetColorShader::Clear()
	{
		target_tex->ClearCS(false);
	}
}