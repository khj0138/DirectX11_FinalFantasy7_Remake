#include "pch.h"
#include "KeyMgr.h"

#include "Engine.h"

namespace ff7r
{
	int Mouse_Wheel_Value = 0;
	int g_arr_key[(UINT)KEY::END] =
	{
		 VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT,
		 VK_SPACE, VK_RETURN, VK_ESCAPE, VK_LMENU, VK_LCONTROL, VK_LSHIFT, VK_TAB,
		 VK_LBUTTON,VK_RBUTTON,

		 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		 'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	KeyMgr::KeyMgr()
		: is_play_mode(false)
	{

	}

	KeyMgr::~KeyMgr()
	{

	}


	void KeyMgr::Init()
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			key_info.push_back(KeyInfo{ (KEY)i  , KEY_STATE::NONE });
		}
	}

	void KeyMgr::Tick()
	{
		if (GetFocus())
		{
			mouse_wheel = Mouse_Wheel_Value;
			Mouse_Wheel_Value = 0;

			for (size_t i = 0; i < key_info.size(); ++i)
			{
				if (GetAsyncKeyState(g_arr_key[(UINT)key_info[i].key]) & 0x8000)
				{
					// 이전에는 눌리지 않았다.
					if (false == key_info[i].prev)
					{
						key_info[i].state = KEY_STATE::TAP;
						key_info[i].prev = true;
					}
					else
					{
						// 지금도 눌려있고, 이전 프레임에서도 눌려있었다.
						key_info[i].state = KEY_STATE::PRESSED;
					}
				}
				else
				{
					// 눌려있지 않다.
					if (false == key_info[i].prev)
					{
						key_info[i].state = KEY_STATE::NONE;
					}
					else
					{
						key_info[i].state = KEY_STATE::RELEASE;
						key_info[i].prev = false;
					}
				}
			}

			TickMouse();
			// Mouse 위치 갱신
		}

		// Window 가 focus 상태가 아니다
		else
		{
			for (size_t i = 0; i < key_info.size(); ++i)
			{
				if (KEY_STATE::TAP == key_info[i].state || KEY_STATE::PRESSED == key_info[i].state)
				{
					key_info[i].state = KEY_STATE::RELEASE;
				}

				else if (KEY_STATE::RELEASE == key_info[i].state)
				{
					key_info[i].state = KEY_STATE::NONE;
				}
			}
		}
	}

	void KeyMgr::TickMouse()
	{
		if (is_play_mode)
		{
			vec2 _center = Engine::GetInst()->GetWindowResolution();
			POINT _center_pos = { _center.x / 2.0f, _center.y / 2.0f };
			POINT _mouse_pos = {};
			GetCursorPos(&_mouse_pos);
			ScreenToClient(Engine::GetInst()->GetMainWnd(), &_mouse_pos);
			mouse_pos = vec2((float)_mouse_pos.x, (float)_mouse_pos.y);

			mouse_dir = mouse_pos - vec2(_center / 2.f);
			mouse_dir.y *= -1;

			ClientToScreen(Engine::GetInst()->GetMainWnd(), &_center_pos);
			SetCursorPos((int)(_center_pos.x), (int)(_center_pos.y));
		}
		else
		{
			prev_mouse_pos = mouse_pos;

			POINT _mouse_pos = {};
			GetCursorPos(&_mouse_pos);
			ScreenToClient(Engine::GetInst()->GetMainWnd(), &_mouse_pos);
			mouse_pos = vec2((float)_mouse_pos.x, (float)_mouse_pos.y);

			mouse_dir = mouse_pos - prev_mouse_pos;
			mouse_dir.y *= -1;
		}
	}

	void KeyMgr::ChangeGameMode()
	{
		is_play_mode = !is_play_mode;

		if (is_play_mode)
		{
			ShowCursor(FALSE);
		}
		else
		{
			ShowCursor(TRUE);
		}
	}
}