#pragma once

#include "Resource.h"

namespace ff7r
{
	class StructuredBuffer;
	class Animator3D;
	class Animation3D : public Resource
	{
	public:
		Animation3D();
		~Animation3D();

		CLONE(Animation3D);

		void FinalTick();
		void Reset();
		void Clear();

		bool PlayPrevFrame(float _wait_time);

		virtual void UpdateData();
		virtual int Save(const wstring& _strRelativePath) override;

		StructuredBuffer* GetJointFrameData() { return joint_data; }

		float GetDuration() { return duration; }
		float GetRatio() { return ratio; }

		bool IsFinish() { return is_finish; }
		bool IsLoop() { return is_loop; }

		void SetLoop(bool _loop) { is_loop = _loop; }
		void SetClip(struct AnimClip* _clip, UINT _joint_cnt);
		void SetAnimator(Animator3D* _animator) { animator = _animator; }
		void SetAnimationEvent(wstring _event_name, int _frame = -1);
		bool UpdateAnimationEvent(int _idx, wstring _event_name, int _frame = -1);
		void UnsetAnimationEvent(int _idx);

		void SetStartFrame(int _frame_idx) { frame_idx = _frame_idx;  next_frame_idx = frame_idx + 1; }

		int GetFrameIdx() { return frame_idx; }
		int GetNextFrameIdx() { return next_frame_idx; }
		vector<std::pair<int, wstring>>& GetAnimationEvents() { return anim_events; }


		FrameTrans* FindCurFrameTrans(int _frame_index, int _joint_index) { return &frame_trans[_frame_index * joint_cnt + _joint_index]; }

		int GetFrameCount() { return (int)frame_cnt; }
		vector<float>& GetFrames() { return frame_times; }

	private:
		virtual int Load(const wstring& _strFilePath) override;

		StructuredBuffer*	joint_data;
		vector<FrameTrans>	frame_trans;
		Animator3D*			animator;
		vector<float>		frame_times;
		
		UINT				joint_cnt;
		UINT				frame_cnt;

		double				cur_time;

		float				duration;
		float				ratio;	

		int					frame_idx;
		int					next_frame_idx;

		bool				is_finish;
		bool				is_loop;
		bool				is_start;

		vector<std::pair<int, wstring>> anim_events;
	};
}

