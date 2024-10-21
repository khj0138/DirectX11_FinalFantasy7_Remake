#pragma once
#include "Resource.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

namespace ff7r
{
	class Sound : public Resource
	{
	public:
		static FMOD::System* g_fmod_system;

	private:
		FMOD::Sound* sound;
		list<FMOD::Channel*>	channel_list;

	public:
		// 0 (무한반복) 0 ~ 1(Volume)
		int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
		void Stop();
		void SetPos(vec3 _pos, int _iChannelIdx);
		void SetMinMax(vec2 _min_max, int _iChannelIdx);
		float GetVolume(int _iChannelIdx);
		vec2 GetMinMax(int _iChannelIdx);

		// 0 ~ 1
		void SetVolume(float _f, int _iChannelIdx);

	private:
		void RemoveChannel(FMOD::Channel* _pTargetChannel);
		friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
			, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
			, void* commanddata1, void* commanddata2);

		vec2 min_max_distance;

	public:
		virtual void UpdateData() {}

		// 파일로 저장
		virtual int Save(const wstring&) override { return S_OK; }

		// 파일로부터 로딩
		virtual int Load(const wstring& _strFilePath) override;

	public:
		Sound();
		virtual ~Sound();
	};

}