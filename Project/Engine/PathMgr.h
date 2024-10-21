#pragma once

namespace ff7r
{
	class PathMgr : public Singleton<PathMgr>
	{
	public:
		void Init();

		const wchar_t* GetContentPath() { return content_path; }

	private:
		SINGLE(PathMgr);

		wchar_t		content_path[256];
	};
}