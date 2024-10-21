#pragma once


namespace ff7r
{
	class UIScript;
	class GameObject;
	class UIMgr : public Singleton<UIMgr>
	{
	public:
		void Init();
		void Progress();
		void AddUIObject(wstring _name, UIScript* _UI);

		void CallUI(wstring _UI_name);
		void ReturnUI();
		void ResetStack();

		void PrintWorldTextUI(string _text, bool _is_text, bool _is_damage_text, vec3 _world_pos, vec3 _world_scale = vec3(0.6f, 0.6f, 0.6f));

		GameObject* GetFrameObject() { return frame_object; }

	private:
		SINGLE(UIMgr);

		static GameObject* canvas;

		map<wstring, UIScript*> ui_objects;
		vector<UIScript*>		ui_stack;

		GameObject* focused_object;

		GameObject* frame_object;
		float time_check;

	};
}

