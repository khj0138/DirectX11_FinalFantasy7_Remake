#pragma once
namespace ff7r
{
	enum class KEY
	{
		UP,	DOWN, LEFT, RIGHT, 
		SPACE, ENTER, ESC, LALT, LCTRL, LSHIFT, TAB,
		LBTN, RBTN,

		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,

		_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,

		END,
	};

	enum class KEY_STATE
	{
		TAP,
		PRESSED,
		RELEASE,
		NONE,
	};

	struct KeyInfo
	{
		KEY			key;
		KEY_STATE	state;
		bool		prev;
	};

	extern int Mouse_Wheel_Value;

	class KeyMgr : public Singleton<KeyMgr>
	{
	public:
		void Init();
		void Tick();

		void TickMouse();

		KEY_STATE GetKeyState(KEY _key) { return key_info[(UINT)_key].state; }
		vec2 GetMousePos() { return mouse_pos; }
		vec2 GetMouseDir() { return mouse_dir; }

		int	GetMouseWheel() { return mouse_wheel; }

		// test code 
		void ChangeGameMode();

	private:
		SINGLE(KeyMgr);

		vector<KeyInfo>		key_info;
		vec2				mouse_pos;
		vec2				prev_mouse_pos;
		vec2				mouse_dir;

		int					mouse_wheel;

		bool				is_play_mode;
	};
}
