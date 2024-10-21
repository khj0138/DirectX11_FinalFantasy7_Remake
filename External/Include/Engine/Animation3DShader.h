#pragma once
#include "ComputeShader.h"

namespace ff7r
{
	class StructuredBuffer;
	class Animation3DShader : public ComputeShader
	{
	public:
		Animation3DShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group);
		virtual ~Animation3DShader();

		virtual void UpdateData();
		virtual void Clear();

		// g_int_0 : BonCount, g_int_1 : Frame Index
		void SetBoneCount(int _iBoneCount) { mtrl_const_data.arr_int[0] = _iBoneCount; }
		void SetFrameIndex(int _iFrameIdx) { mtrl_const_data.arr_int[1] = _iFrameIdx; }
		void SetNextFrameIdx(int _iFrameIdx) { mtrl_const_data.arr_int[2] = _iFrameIdx; }
		void SetChangeAnim(bool _is_change) { mtrl_const_data.arr_bool[0] = _is_change; }
		void SetStartChangeAnim(bool _is_start_change) { mtrl_const_data.arr_bool[1] = _is_start_change; }
		void SetFrameRatio(float _fFrameRatio) { mtrl_const_data.arr_float[0] = _fFrameRatio; }


		void SetFrameDataBuffer(StructuredBuffer* _buffer) { frame_data_buff = _buffer; }
		void SetOffsetMatBuffer(StructuredBuffer* _buffer) { offset_mat_buff = _buffer; }
		void SetOutputBuffer(StructuredBuffer* _buffer) { output_mat_buff = _buffer; }
		void SetCurJointMatBuffer(StructuredBuffer* _buffer) { cur_joint_mat_buff = _buffer; }
		void SetFrameTransBuffer(StructuredBuffer* _buffer) { output_trans_buff = _buffer; }

		//StructuredBuffer* GetOutput

	private:
		StructuredBuffer* frame_data_buff;			// t16
		StructuredBuffer* offset_mat_buff;			// t17 
		StructuredBuffer* output_mat_buff;			// u0
		StructuredBuffer* output_trans_buff;		// u1
		StructuredBuffer* cur_joint_mat_buff;		// u2
	};
}