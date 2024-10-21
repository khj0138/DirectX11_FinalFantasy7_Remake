#include "pch.h"
#include "Animation3DShader.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "StructuredBuffer.h"

namespace ff7r
{
	Animation3DShader::Animation3DShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group)
		: frame_data_buff(nullptr)
		, offset_mat_buff(nullptr)
		, output_mat_buff(nullptr)
		, output_trans_buff(nullptr)
		, cur_joint_mat_buff(nullptr)
	{
		thread_x_per_group = _thread_x_per_group;
		thread_y_per_group = _thread_y_per_group;
		thread_z_per_group = _thread_z_per_group;
	}

	Animation3DShader::~Animation3DShader()
	{
	}
		
	void Animation3DShader::UpdateData()
	{
		// 구조화버퍼 전달
		if (frame_data_buff)
		{
			frame_data_buff->UpdateDataCS(16, true); // t16
		}
		offset_mat_buff->UpdateDataCS(17, true); // t17
		output_mat_buff->UpdateDataCS(0, false);   // u0
		output_trans_buff->UpdateDataCS(1, false);
		cur_joint_mat_buff->UpdateDataCS(2, false);

		group_x_cnt = (mtrl_const_data.arr_int[0] / thread_x_per_group) + 1;
		group_y_cnt = 1;
		group_z_cnt = 1;
	}


	void Animation3DShader::Clear()
	{
		// 전달한 구조화버퍼 클리어	
		if (nullptr != frame_data_buff)
		{
			frame_data_buff->ClearCS(true);
			frame_data_buff = nullptr;
		}

		if (nullptr != offset_mat_buff)
		{
			offset_mat_buff->ClearCS(true);
			offset_mat_buff = nullptr;
		}

		if (nullptr != output_mat_buff)
		{
			output_mat_buff->ClearCS(false);
			output_mat_buff = nullptr;
		}

		if (nullptr != output_trans_buff)
		{
			output_trans_buff->ClearCS(false);
			output_trans_buff = nullptr;
		}

		if (nullptr != cur_joint_mat_buff)
		{
			cur_joint_mat_buff->ClearCS(false);
			cur_joint_mat_buff = nullptr;
		}
	}
}