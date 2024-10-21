#include "pch.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "StructuredBuffer.h"

#include "CopyBoneShader.h"
namespace ff7r
{
	CopyBoneShader::CopyBoneShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group)
		: src_buff(nullptr)
		, dest_buff(nullptr)
	{
		thread_x_per_group = _thread_x_per_group;
		thread_y_per_group = _thread_y_per_group;
		thread_z_per_group = _thread_z_per_group;
	}

	CopyBoneShader::~CopyBoneShader()
	{		
	}

	void CopyBoneShader::UpdateData()
	{
		src_buff->UpdateDataCS(16, true);  // t16
		dest_buff->UpdateDataCS(0, false);	// u0

		// 그룹 수 계산
		int iBoneCount = mtrl_const_data.arr_int[0];

		group_x_cnt = iBoneCount / thread_x_per_group + 1;
		group_y_cnt = 1;
		group_z_cnt = 1;
	}

	void CopyBoneShader::Clear()
	{
		src_buff->ClearCS(true);
		dest_buff->ClearCS(false);
	}
}