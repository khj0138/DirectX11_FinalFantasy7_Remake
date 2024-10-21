#pragma once

#include "ComputeShader.h"

namespace ff7r
{
	class StructuredBuffer;
	class CopyBoneShader : public ComputeShader
	{
	public:
		CopyBoneShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group);
		virtual ~CopyBoneShader();

		virtual void UpdateData();
		virtual void Clear();	

		void SetBoneCount(UINT _bone_cnt) { mtrl_const_data.arr_int[0] = _bone_cnt; }
		void SetRowIndex(UINT _row_idx) { mtrl_const_data.arr_int[1] = _row_idx; }

		void SetSourceBuffer(StructuredBuffer* _buff) { src_buff = _buff; }
		void SetDestBuffer(StructuredBuffer* _buff) { dest_buff = _buff; }

	private:
		StructuredBuffer* src_buff;
		StructuredBuffer* dest_buff;
	};
}
