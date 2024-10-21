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

		// ���� ������ �ڵ�
		hwnd = _hWnd;
		resolution = vec2((float)_width, (float)_height);

		// �ػ󵵿� �´� �۾����� ũ�� ����
		RECT rt = { 0, 0, (int)_width, (int)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
		SetWindowPos(hwnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(hwnd, true);

		// Device �ʱ�ȭ
		if (FAILED(Device::GetInst()->Init(hwnd, _width, _height)))
		{
			MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"����", MB_OK);
			return E_FAIL;
		}


		// Manager �ʱ�ȭ
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

		// ī�޶� ����, ī�޶� �ٶ󺸴� �������� ȭ���� �����쿡 �׸�
		Render();

		// Event ó��, tick ���� �ٷ� ó���� �Ұ����Ѱ͵��� ��Ƽ� ����ó��
		EventMgr::GetInst()->Tick();
	}

	void Engine::Tick()
	{
		// Manager Tick
		ResourceMgr::GetInst()->Tick();
		TimeMgr::GetInst()->Tick(); // DT(DeltaTime), FPS ���ϱ�
		KeyMgr::GetInst()->Tick();

		// FMOD Update
		SoundMgr::GetInst()->Tick();
		Sound::g_fmod_system->update();

		// Level Update
		// Level �ȿ� �����ϴ� ��� GameObject ���� Tick �� ȣ�����
		LevelMgr::GetInst()->Tick();
		PhysXMgr::GetInst()->progress();


		// Level ���� GameObject ���� �������� ���ؼ� �߻��� �浹�� üũ�Ѵ�.
		//CollisionMgr::GetInst()->Tick();

	}

	void Engine::Render()
	{
		RenderMgr::GetInst()->Render();

		// FPS, DT ���
		TimeMgr::GetInst()->Render();
		 //Device::GetInst()->ExcuteCommadList();
	}
}