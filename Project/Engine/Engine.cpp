#include "pch.h"
#include "Engine.h"

#include "Level.h"
#include "Device.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResourceMgr.h"
#include "LevelMgr.h"
#include "CollisionMgr.h"
#include "RenderMgr.h"
#include "EventMgr.h"
#include "FontMgr.h"
#include "PhysXMgr.h"
#include "SoundMgr.h"
#include "InstancingBuffer.h"

namespace ff7r
{
	Engine::Engine()
		: hwnd(nullptr)
	{
	}

	Engine::~Engine()
	{
	}

	int Engine::Init(HWND _hWnd, UINT _width, UINT _height)
	{

		// 메인 윈도우 핸들
		hwnd = _hWnd;
		resolution = vec2((float)_width, (float)_height);

		// 해상도에 맞는 작업영역 크기 조정
		RECT rt = { 0, 0, (int)_width, (int)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
		SetWindowPos(hwnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(hwnd, true);

		// Device 초기화
		if (FAILED(Device::GetInst()->Init(hwnd, _width, _height)))
		{
			MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
			return E_FAIL;
		}


		// Manager 초기화
		PathMgr::GetInst()->Init();
		KeyMgr::GetInst()->Init();
		TimeMgr::GetInst()->Init();
		ResourceMgr::GetInst()->Init();
		RenderMgr::GetInst()->Init();
		FontMgr::GetInst()->Init();
		LevelMgr::GetInst()->Init();
		SoundMgr::GetInst()->Init();
		InstancingBuffer::GetInst()->Init();


		return S_OK;
	}


	void Engine::Progress()
	{
		Tick();

		// 카메라를 지정, 카메라가 바라보는 시점으로 화면을 윈도우에 그림
		Render();

		// Event 처리, tick 에서 바로 처리가 불가능한것들을 모아서 지연처리
		EventMgr::GetInst()->Tick();
	}

	void Engine::Tick()
	{
		// Manager Tick
		ResourceMgr::GetInst()->Tick();
		TimeMgr::GetInst()->Tick(); // DT(DeltaTime), FPS 구하기
		KeyMgr::GetInst()->Tick();

		// FMOD Update
		SoundMgr::GetInst()->Tick();
		Sound::g_fmod_system->update();

		// Level Update
		// Level 안에 존재하는 모든 GameObject 들이 Tick 을 호출받음
		LevelMgr::GetInst()->Tick();
		PhysXMgr::GetInst()->progress();


		// Level 내에 GameObject 들의 변경점에 의해서 발생한 충돌을 체크한다.
		//CollisionMgr::GetInst()->Tick();

	}

	void Engine::Render()
	{
		RenderMgr::GetInst()->Render();

		// FPS, DT 출력
		TimeMgr::GetInst()->Render();
		 //Device::GetInst()->ExcuteCommadList();
	}
}