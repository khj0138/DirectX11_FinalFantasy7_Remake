#include "pch.h"
#include "Sound.h"

#include "ResourceMgr.h"
#include "SoundMgr.h"

namespace ff7r
{
	FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

	FMOD::System* Sound::g_fmod_system = nullptr;

	Sound::Sound()
		: Resource(RES_TYPE::SOUND)
		, sound(nullptr)
	{
	}

	Sound::~Sound()
	{
		if (nullptr != sound)
		{
			sound->release();
			sound = nullptr;
		}
	}

	int Sound::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
	{
		if (_iRoopCount <= -1)
		{
			assert(nullptr);
		}

		// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
		if (!_bOverlap && !channel_list.empty())
		{
			return -1;
		}

		_iRoopCount -= 1;

		FMOD::Channel* pChannel = nullptr;
		g_fmod_system->playSound(sound, nullptr, false, &pChannel);

		pChannel->setVolume(_fVolume);

		pChannel->setCallback(CHANNEL_CALLBACK);
		pChannel->setUserData(this);

		pChannel->setMode(FMOD_LOOP_NORMAL);
		pChannel->setLoopCount(_iRoopCount);

		channel_list.push_back(pChannel);

		int iIdx = -1;
		pChannel->getIndex(&iIdx);

		return iIdx;
	}

	void Sound::Stop()
	{
		list<FMOD::Channel*>::iterator iter;

		while (!channel_list.empty())
		{
			iter = channel_list.begin();
			(*iter)->stop();
		}
	}

	void Sound::SetPos(vec3 _pos, int _iChannelIdx)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();

		int iIdx = -1;

		FMOD_VECTOR soundPosition = { _pos.x, _pos.y, _pos.z };

		for (; iter != channel_list.end(); ++iter)
		{
			(*iter)->getIndex(&iIdx);
			if (_iChannelIdx == iIdx)
			{
				(*iter)->set3DAttributes(&soundPosition, nullptr);
				return;
			}
		}
	}

	void Sound::SetMinMax(vec2 _min_max, int _iChannelIdx)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();

		int iIdx = -1;
		for (; iter != channel_list.end(); ++iter)
		{
			(*iter)->getIndex(&iIdx);
			if (_iChannelIdx == iIdx)
			{
				(*iter)->set3DMinMaxDistance(_min_max.x, _min_max.y);
				return;
			}
		}
	}

	float Sound::GetVolume(int _iChannelIdx)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();

		int iIdx = -1;
		float _volume = -1.0f;
		for (; iter != channel_list.end(); ++iter)
		{
			(*iter)->getIndex(&iIdx);
			if (_iChannelIdx == iIdx)
			{
				(*iter)->getVolume(&_volume);
				break;
			}
		}
		return _volume;
	}

	vec2 Sound::GetMinMax(int _iChannelIdx)
	{
		vec2 _min_max = vec2(-1.0f, -1.0f);

		list<FMOD::Channel*>::iterator iter = channel_list.begin();

		int iIdx = -1;
		for (; iter != channel_list.end(); ++iter)
		{
			(*iter)->getIndex(&iIdx);
			if (_iChannelIdx == iIdx)
			{
				float _min;
				float _max;
				(*iter)->get3DMinMaxDistance(&_min, &_max);
				_min_max = vec2(_min, _max);
				break;
			}
		}
		return _min_max;
	}

	void Sound::SetVolume(float _f, int _iChannelIdx)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();

		int iIdx = -1;
		for (; iter != channel_list.end(); ++iter)
		{
			(*iter)->getIndex(&iIdx);
			if (_iChannelIdx == iIdx)
			{
				(*iter)->setVolume(_f);
				return;
			}
		}
	}

	void Sound::RemoveChannel(FMOD::Channel* _pTargetChannel)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();
		for (; iter != channel_list.end(); ++iter)
		{
			if (*iter == _pTargetChannel)
			{
				channel_list.erase(iter);
				return;
			}
		}
	}

	//int Sound::Load(const wstring& _strFilePath)
	//{
	//	string path(_strFilePath.begin(), _strFilePath.end());

	//	//if (FMOD_OK != g_fmod_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound))
	//	if (FMOD_OK != g_fmod_system->createSound(path.c_str(), FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, nullptr, &sound))
	//	{
	//		assert(nullptr);
	//	}

	//	return S_OK;
	//}

	int Sound::Load(const wstring& _strFilePath)
	{
		string _path(_strFilePath.begin(), _strFilePath.end());

		path a = _path;

		ResourceMgr::GetInst()->GetSoundMutex()->lock();
		if (a.parent_path().filename().string() == "BGM")
		{
			if (FMOD_OK != g_fmod_system->createSound(_path.c_str(), FMOD_DEFAULT, nullptr, &sound))
			{
				assert(nullptr);
			}
		}
		
		//if(_strFilePath)
		//if (FMOD_OK != g_fmod_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound))
		else
		{
			if (FMOD_OK != g_fmod_system->createSound(_path.c_str(), FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, nullptr, &sound))
			{
				assert(nullptr);
			}
		}
		ResourceMgr::GetInst()->GetSoundMutex()->unlock();

		return S_OK;
	}

	// =========
	// Call Back
	// =========
	FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2)
	{
		FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
		Sound* pSound = nullptr;

		switch (controltype)
		{
		case FMOD_CHANNELCONTROL_CALLBACK_END:
		{
			cppchannel->getUserData((void**)&pSound);
			int iIdx = -1;
			cppchannel->getIndex(&iIdx);
			if (iIdx != -1)
				SoundMgr::GetInst()->Stop(iIdx);
			pSound->RemoveChannel(cppchannel);
		}
		break;
		}

		return FMOD_OK;
	}
}