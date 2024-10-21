#pragma once
#include "Singleton.h"

namespace ff7r
{
	class Sound;
	class SoundMgr : public Singleton<SoundMgr>
	{
	public:

		void Init();
		void Tick();
		void SetListener(GameObject* _listener) { listener = _listener; }
		int Play(std::wstring _name, SOUND_OBJECT_TYPE type, GameObject* _object = nullptr, float _fVolume = 0.3f, vec2 _min_max = vec2(1.0f, 20.0f), int _iRoopCount = 1, bool _bOverlap = false);
		void Stop(int _channel, GameObject* _object);
		void Stop(int _channel);
	private:
		SINGLE(SoundMgr);

		struct SoundObject
		{
			Sound* sound = nullptr;
			GameObject* object = nullptr;
			vec3 pos = vec3(0.0f, 0.0f, 0.0f);
			vec2 min_max = vec2(0.0f, 0.0f);
			SOUND_OBJECT_TYPE sound_type = SOUND_OBJECT_TYPE::_3D;
			float volume = 0.0f;
		};

		SoundObject sound_objects[32];
		GameObject* listener;


	};
}