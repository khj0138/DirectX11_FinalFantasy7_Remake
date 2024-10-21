#include "pch.h"
#include "SoundMgr.h"
#include "ResourceMgr.h"
#include "Sound.h"

#include "Transform.h"

namespace ff7r
{
	SoundMgr::SoundMgr()
		: listener(nullptr)
	{
	}

	SoundMgr::~SoundMgr()
	{
		for (int i = 0; i < 32; i++)
		{
			sound_objects[i].sound = nullptr;
		}
	}
	void SoundMgr::Init()
	{
		for (int i = 0; i < 32; i++)
		{
			SoundObject _object;
			sound_objects[i] = _object;
		}
	}

	void SoundMgr::Tick()
	{
		if (listener)
		{
			vec3 listener_pos = listener->GetTransform()->GetWorldPos();
			FMOD_VECTOR listenerPosition = { listener_pos.x, listener_pos.y, listener_pos.z };
			Sound::g_fmod_system->set3DListenerAttributes(0, &listenerPosition, nullptr, nullptr, nullptr);

			for (int i = 0; i < 32; i++)
			{
				SoundObject _object = sound_objects[i];
				if (_object.min_max.y == 0.0f)
				{
					continue;
				}
				if (_object.object)
				{
					_object.pos = _object.object->GetTransform()->GetWorldPos();


				}
				_object.sound->SetPos(_object.pos, i);
				float _volume = _object.sound->GetVolume(i);
				vec2 _min_max = _object.sound->GetMinMax(i);
			}
		}
	}

	int SoundMgr::Play(std::wstring _name, SOUND_OBJECT_TYPE _type, GameObject* _object, float _fVolume, vec2 _min_max, int _iRoopCount, bool _bOverlap)
	{
		Ptr<Sound> _sound = ResourceMgr::GetInst()->FindRes<Sound>(_name);
		SoundObject _sound_object;
		_sound_object.object = _object;
		if (_object)
		{
			_sound_object.pos = _object->GetTransform()->GetWorldPos();
		}
		_sound_object.min_max = _min_max;
		_sound_object.sound_type = _type;
		_sound_object.sound = _sound.Get();
		_sound_object.volume = _fVolume;

		int _channel = _sound->Play(_iRoopCount, _fVolume, _bOverlap);
		if (_channel != -1)
		{
			sound_objects[_channel] = _sound_object;

			_sound->SetMinMax(_min_max, _channel);
			return _channel;
		}
		return -1;
	}

	void SoundMgr::Stop(int _channel, GameObject* _object)
	{
		SoundObject object;
		if ((_channel != -1) && (sound_objects[_channel].object == _object))
		{
			sound_objects[_channel].sound->Stop();
			sound_objects[_channel] = object;
		}
	}


	void SoundMgr::Stop(int _channel)
	{
		//sound_objects[_channel].sound->Stop();

		SoundObject _object;
		sound_objects[_channel] = _object;
	}

	/*void SoundMgr::Stop(int _channel)
	{
		SoundObject _object;
		sound_objects[_channel] = _object;
	}*/

}