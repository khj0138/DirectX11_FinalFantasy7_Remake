#include "pch.h"
#include "Animation2D.h"

#include "TimeMgr.h"

namespace ff7r
{
	Animation2D::Animation2D()
		: owner(nullptr)
		, cur_frame(0)
		, is_finish(false)
		, time(0.f)
	{
	}

	Animation2D::~Animation2D()
	{
	}

	void Animation2D::FinalTick()
	{
		if (is_finish)
			return;

		time += DT;

		if (frames[cur_frame].duration < time)
		{
			time = time - frames[cur_frame].duration;
			++cur_frame;

			if (frames.size() <= cur_frame)
			{
				cur_frame = frames.size() - 1;
				is_finish = true;
			}
		}
	}

	void Animation2D::Create(const wstring& _strAnimName, Ptr<Texture> _AtlasTex
		, vec2 _vLeftTop, vec2 _vSlice, vec2 _vBackSize, int _FrameCount, int _FPS)
	{
		assert(_AtlasTex.Get());

		SetName(_strAnimName);

		atlas_tex = _AtlasTex;

		vec2 vResolution = vec2(atlas_tex->Width(), atlas_tex->Height());

		back_size = _vBackSize / vResolution;

		for (size_t i = 0; i < _FrameCount; ++i)
		{
			Anim2DFrame frm = {};

			frm.duration = 1.f / (float)_FPS;
			frm.left_top_uv = vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution;
			frm.slice_uv = _vSlice / vResolution;

			frames.push_back(frm);
		}
	}

	void Animation2D::Create(const wstring& _ani_name, Ptr<Texture> _atlas_tex, vec2 _left_top, vec2 _slice_cnt, float _fps)
	{
		assert(_atlas_tex.Get());

		SetName(_ani_name);

		atlas_tex = _atlas_tex;
		vec2 _resolution = vec2(atlas_tex->Width(), atlas_tex->Height());
		vec2 _slice_size = _resolution / _slice_cnt;
		back_size = _slice_size / _resolution;

		for (size_t i = 0; i < _slice_cnt.y; ++i)
		{
			for (size_t j = 0; j < _slice_cnt.x; ++j)
			{
				Anim2DFrame frm = {};
		
				frm.duration = 1.f / (float)_fps;
				frm.left_top_uv = vec2(_slice_size.x * j, _slice_size.y * i) / _resolution;
				frm.slice_uv = _slice_size / _resolution;
		
				frames.push_back(frm);
			}
		}

		// for (int i = _slice_cnt.y - 1; i >= 0; --i)
		// {
		// 	for (int j = _slice_cnt.x - 1; j >=0; --j)
		// 	{
		// 		Anim2DFrame frm = {};
		// 
		// 		frm.duration = 1.f / (float)_fps;
		// 		frm.left_top_uv = vec2(_slice_size.x * j, _slice_size.y * i) / _resolution;
		// 		frm.slice_uv = _slice_size / _resolution;
		// 
		// 		frames.push_back(frm);
		// 	}
		// }
	}

	void Animation2D::SaveToLevelFile(FILE* _File)
	{
		SaveWString(GetName(), _File);

		size_t FrameCount = frames.size();
		fwrite(&FrameCount, sizeof(size_t), 1, _File);
		fwrite(frames.data(), sizeof(Anim2DFrame), FrameCount, _File);
		fwrite(&back_size, sizeof(vec2), 1, _File);

		SaveResRef(atlas_tex.Get(), _File);
	}

	void Animation2D::LoadFromLevelFile(FILE* _File)
	{
		wstring name;
		LoadWString(name, _File);
		SetName(name);

		size_t FrameCount = 0;
		fread(&FrameCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < FrameCount; ++i)
		{
			Anim2DFrame frm = {};
			fread(&frm, sizeof(Anim2DFrame), 1, _File);
			frames.push_back(frm);
		}

		fread(&back_size, sizeof(vec2), 1, _File);

		LoadResRef(atlas_tex, _File);
	}
}