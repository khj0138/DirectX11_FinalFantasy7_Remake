#include "pch.h"
#include "PathMgr.h"

#include "Engine.h"

namespace ff7r
{
	PathMgr::PathMgr()
		: content_path{}
	{
	}

	PathMgr::~PathMgr()
	{
	}


	void PathMgr::Init()
	{
		GetCurrentDirectory(256, content_path);

		// 상위폴더로 감
		int _len = (int)wcslen(content_path);

		for (int i = _len - 1; i >= 0; --i)
		{
			if (L'\\' == content_path[i])
			{
				content_path[i] = 0;
				break;
			}
		}

		// + bin\\content
		wcscat_s(content_path, L"\\bin\\content\\");
	}
}