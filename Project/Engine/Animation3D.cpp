#include "pch.h"

#include "Animator3D.h"
#include "Animation3DShader.h"
#include "GLTFLoader.h"
#include "StructuredBuffer.h"
#include "TimeMgr.h"
#include "PathMgr.h"

#include "Animation3D.h"
#include "KeyMgr.h"
namespace ff7r
{
	Vector4 QuaternionToVec4(const DirectX::SimpleMath::Quaternion& _q)
	{
		Vector4 vec4;
		vec4.x = _q.x;
		vec4.y = _q.y;
		vec4.z = _q.z;
		vec4.w = _q.w;

		return vec4;
	}

	void DecomposeMatrix(Matrix& matrix, vec4& position, vec4& scale, vec4& rotation)
	{
		Quaternion	_rotation;
		vec3		_position;
		vec3		_scale;

		matrix.Decompose(_scale, _rotation, _position);

		rotation = QuaternionToVec4(_rotation);
		position = _position;
		scale = _scale;
	}

	Animation3D::Animation3D()
		: Resource(RES_TYPE::ANIMATION3D)
		, joint_cnt(0)
		, frame_cnt(0)
		, cur_time(0.f)
		, frame_idx(0)
		, next_frame_idx(1)
		, ratio(0.f)
		, is_finish(false)
		, animator(nullptr)
		, duration(0.f)
		, is_loop(true)
		, is_start(true)
		, joint_data(nullptr)
	{
	}


	Animation3D::~Animation3D()
	{
		if (joint_data)
		{
			delete joint_data;
			joint_data = nullptr;
		}
	}

	void Animation3D::FinalTick()
	{
		if (is_start)
		{
			animator->CalcWorldDir();
			is_start = false;
		}

		cur_time += DT;
		
		// 나중에 DT가 커져서 한번에 2프레임 이상 넘어가는 기능 구현해야 될듯? 그러면 이벤트 호출 쪽도 좀 수정하고
		
		if (cur_time > frame_times[next_frame_idx])
		{
			frame_idx++;
			next_frame_idx++;
		
			for (int i = 0; i < anim_events.size(); i++)
			{
				if (anim_events[i].first == frame_idx)
				{
					animator->GetAnimationEvent(anim_events[i].second)();
				}
			}
		
			if (next_frame_idx >= frame_cnt)
			{
				is_finish = true;
				next_frame_idx = frame_cnt - 1;
				frame_idx = frame_cnt - 1;
				ratio = 1.0f;
				//animator->ResetRootMat();
				return;
				//Reset();
			}
		}
		
		ratio = (float)((cur_time - frame_times[frame_idx]) / (frame_times[next_frame_idx] - frame_times[frame_idx]));

		//if (KEY_TAP(KEY::R))
		//{
		//	frame_idx++;
		//	next_frame_idx++;
		//}
		//if (KEY_TAP(KEY::T))
		//{
		//	frame_idx--;
		//	next_frame_idx--;
		//}
		//for (int i = 0; i < anim_events.size(); i++)
		//{
		//	if (anim_events[i].first == frame_idx)
		//	{
		//		animator->GetAnimationEvent(anim_events[i].second)();
		//		break;
		//	}
		//}

		//if (next_frame_idx >= frame_cnt)
		//{
		//	is_finish = true;
		//	next_frame_idx = frame_cnt - 1;
		//	frame_idx = frame_cnt - 1;
		//	ratio = 1.0f;
		//	//animator->ResetRootMat();
		//	return;
		//	//Reset();
		//}

		//ratio = 0.0f;
	}

	void Animation3D::Reset()
	{
		cur_time = 0.0f;
		frame_idx = 0;
		next_frame_idx = 1;
		ratio = 0.0f;
		is_finish = false;
		is_start = true;
	}

	void Animation3D::Clear()
	{
		joint_data->Clear();
	}
		
	bool Animation3D::PlayPrevFrame(float _wait_time)
	{
		cur_time += DT;
		//_wait_time = 3.0f;
		if (cur_time >= _wait_time)
		{
			ratio = 0.0f;
			cur_time = 0.0f;
			//animator->ResetRootMat();
			animator->ResetStartClip();
			return false;
		}

		ratio = cur_time / _wait_time;

		return true;
	}

	void Animation3D::SetClip(AnimClip* _clip, UINT _joint_cnt)
	{
		frame_times = _clip->frame_times;
		joint_cnt = _joint_cnt;

		frame_cnt = _clip->key_frames[0].size();

		frame_trans.resize(frame_cnt * joint_cnt);

		for (UINT i = 0; i < joint_cnt; i++)
		{
			for (UINT j = 0; j < frame_cnt; j++)
			{
				FrameTrans _trans;
				DecomposeMatrix(_clip->key_frames[i][j].mat_joint, _trans.translate, _trans.scale, _trans.rotation);
				frame_trans[i + j * joint_cnt] = _trans;
			}
		}

		duration = frame_times[frame_cnt - 1] - frame_times[frame_cnt - 2];

		if (joint_data == nullptr)
		{
			joint_data = new StructuredBuffer;
			joint_data->Create(sizeof(FrameTrans), joint_cnt * frame_cnt, SB_TYPE::READ_ONLY, false, frame_trans.data());
		}
	}

	void Animation3D::SetAnimationEvent(wstring _event_name, int _frame)
	{
		if (_frame == -1)
		{
			_frame = frame_cnt - 1;
		}

		auto _event = animator->GetAnimationEvent(_event_name);

		if (_event)
		{
			//anim_events.insert({ _frame, _event_name });
			anim_events.push_back(make_pair(_frame, _event_name));
		}
	}

	bool Animation3D::UpdateAnimationEvent(int _idx, wstring _event_name, int _frame)
	{
		for (int i = 0; i < anim_events.size(); i++)
		{
			if (i == _idx)
				continue;
			if (anim_events[i].first == _frame)
			{
				if (anim_events[i].second == _event_name)
				{
					return false;
				}
			}
		}
		
		anim_events[_idx].first = _frame;
		anim_events[_idx].second = _event_name;

		return true;
	}

	void Animation3D::UnsetAnimationEvent(int _idx)
	{
		anim_events.erase(anim_events.begin() + _idx);
	}

	void Animation3D::UpdateData()
	{
		Animation3DShader* _shader = animator->GetAnimationShader();
		_shader->SetFrameDataBuffer(joint_data);

		_shader->SetFrameIndex(frame_idx);
		_shader->SetNextFrameIdx(next_frame_idx);
		_shader->SetFrameRatio(ratio);
	}

	int Animation3D::Save(const wstring& _strRelativePath)
	{
		wstring strFilePath = PathMgr::GetInst()->GetContentPath() + _strRelativePath;

		// 파일 쓰기모드로 열기
		FILE* _file = nullptr;
		errno_t err = _wfopen_s(&_file, strFilePath.c_str(), L"wb");
		assert(_file);

		// 키값, 상대 경로	
		SaveWString(GetName(), _file);
		SaveWString(GetKey(), _file);

		// Animation3D 정보 
		UINT _cnt = (UINT)frame_trans.size();
		fwrite(&_cnt, sizeof(int), 1, _file);
		for (UINT i = 0; i < _cnt; ++i)
		{
			//SaveWString(m_vecAnimClip[i].strAnimName, pFile);
			fwrite(&frame_trans[i].scale, sizeof(vec4), 1, _file);
			fwrite(&frame_trans[i].translate, sizeof(vec4), 1, _file);
			fwrite(&frame_trans[i].rotation, sizeof(vec4), 1, _file);
		}

		fwrite(&joint_cnt, sizeof(UINT), 1, _file);
		fwrite(&frame_cnt, sizeof(UINT), 1, _file);
		fwrite(&duration, sizeof(float), 1, _file);

		_cnt = (UINT)frame_times.size();
		fwrite(&_cnt, sizeof(int), 1, _file);

		for (UINT i = 0; i < _cnt; ++i)
		{
			fwrite(&frame_times[i], sizeof(float), 1, _file);
		}

		_cnt = (UINT)anim_events.size();
		fwrite(&_cnt, sizeof(int), 1, _file);

		for (auto iter = anim_events.begin(); iter != anim_events.end(); iter++)
		{
			fwrite(&iter->first, sizeof(int), 1, _file);
			SaveWString(iter->second, _file);
		}

		fclose(_file);


		return S_OK;
	}

	int Animation3D::Load(const wstring& _strFilePath)
	{
		FILE* _file = nullptr;
		errno_t err = _wfopen_s(&_file, _strFilePath.c_str(), L"rb");
		assert(_file);

		// 키값, 상대경로
		wstring strName, strKey, strRelativePath;
		LoadWString(strName, _file);
		SetName(strName);

		LoadWString(strKey, _file);

		// SetKey(strKey);
		// LoadWString(strRelativePath, pFile);
		// SetName(strName);
		// Animation3D 정보 읽기

		int _cnt = 0;
		fread(&_cnt, sizeof(int), 1, _file);
		for (int i = 0; i < _cnt; ++i)
		{
			FrameTrans _trans = {};

			fread(&_trans.scale, sizeof(vec4), 1, _file);
			fread(&_trans.translate, sizeof(vec4), 1, _file);
			fread(&_trans.rotation, sizeof(vec4), 1, _file);

			frame_trans.push_back(_trans);
		}

		fread(&joint_cnt, sizeof(UINT), 1, _file);
		fread(&frame_cnt, sizeof(UINT), 1, _file);
		fread(&duration, sizeof(float), 1, _file);

		_cnt = 0;
		fread(&_cnt, sizeof(int), 1, _file);
		for (int i = 0; i < _cnt; ++i)
		{
			float time;
			fread(&time, sizeof(float), 1, _file);

			frame_times.push_back(time);
		}

		_cnt = 0;
		fread(&_cnt, sizeof(int), 1, _file);
		for (int i = 0; i < _cnt; ++i)
		{
			int index;
			fread(&index, sizeof(int), 1, _file);

			wstring name;
			LoadWString(name, _file);

			//anim_events.insert({ index, name });
			anim_events.push_back(make_pair( index, name ));
		}

		joint_data = new StructuredBuffer;
		joint_data->Create(sizeof(FrameTrans), joint_cnt * frame_cnt, SB_TYPE::READ_ONLY, false, frame_trans.data());


		fclose(_file);

		return S_OK;
	}
}