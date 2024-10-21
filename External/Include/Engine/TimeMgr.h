#pragma once

namespace ff7r
{
	class TimeMgr : public Singleton<TimeMgr>
	{
	public:
		void Init();
		void Tick();
		void Render();

		UINT GetFramePerSecond() { return call_cnt; }
		float GetAccTime() { return acc_time; }
		float GetDeltaTime() { return speed * delta_time; }
		float GetRealDeltaTime() { return delta_time; }

		void SetTimeSpeed(float _speed, float _duration = 0.f) { duration = _duration; speed = _speed; }

	private:
		SINGLE(TimeMgr);

		LARGE_INTEGER	prev_cnt;
		LARGE_INTEGER	cur_cnt;
		LARGE_INTEGER	frequency;

		UINT			call_cnt;

		float			delta_time;
		float			acc_time; // accumulated time
		float			duration;
		float			speed;
	};
}
