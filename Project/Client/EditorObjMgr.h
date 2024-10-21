#pragma once


namespace ff7r
{
	class GameObjectEx;
	class EditorObjMgr : public Singleton<EditorObjMgr>
	{
		SINGLE(EditorObjMgr);
	private:
		vector<GameObjectEx*>	editor_obj;

		GameObjectEx* debug_shape[(UINT)SHAPE_TYPE::END];
		vector<DebugShapeInfo> debug_shape_info;

	public:
		void Init();
		void Progress();

	private:
		void Tick();
		void Render();
	};
}

