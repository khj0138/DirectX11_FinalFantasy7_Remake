#include "pch.h"
#include "TimeMgr.h"

#include "Engine.h"
#include "FontMgr.h"

namespace ff7r
{
	TimeMgr::TimeMgr()
		: prev_cnt{}
		, cur_cnt{}
		, frequency{}
		, call_cnt(0)
		, delta_time(0.f)
		, acc_time(0.f)
		, speed(1.0f)
	{

	}

	TimeMgr::~TimeMgr()
	{

	}



	void TimeMgr::Init()
	{
		// 1초당 카운팅 증가량
		QueryPerformanceFrequency(&frequency);

		QueryPerformanceCounter(&cur_cnt);
		QueryPerformanceCounter(&prev_cnt);
	}

	void TimeMgr::Tick()
	{
		QueryPerformanceCounter(&cur_cnt);

		// tick 간격 시간
		delta_time = (float)(cur_cnt.QuadPart - prev_cnt.QuadPart) / (float)frequency.QuadPart;

		// 누적 시간
		acc_time += delta_time;

		// 함수 호출 횟수
		++call_cnt;

		// 이전 카운트 값을 현재 카운트로 갱신
		prev_cnt = cur_cnt;

		// GlobalData 갱신
		g_global_data.acc_time += delta_time;

#ifdef _DEBUG
		if (delta_time > (1.f / 20.f))
			delta_time = (1.f / 20.f);
#endif

		if (duration > 0.f)
		{
			duration -= delta_time;

			if (duration <= 0.f)
			{
				duration = 0.f;
				speed = 1.f;
			}
		}
		g_global_data.delta_time = speed * delta_time;
	}

	void TimeMgr::Render()
	{
		// 1초에 한번
		static wchar_t _buff[256] = {};

		if (1.f <= acc_time)
		{
			swprintf_s(_buff, L"FPS : %d, DT : %f", call_cnt, delta_time);
			//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);	
			SetWindowText(Engine::GetInst()->GetMainWnd(), _buff);

			acc_time = 0.f;
			call_cnt = 0;
		}

		//FontMgr::GetInst()->DrawFont(_buff, 10, 20, 16, FONT_RGBA(255, 0, 0, 255));
	}
}