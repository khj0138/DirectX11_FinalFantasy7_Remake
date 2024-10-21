#include "pch.h"
#include "Resource.h"

namespace ff7r
{
	Resource::Resource(RES_TYPE _type, bool _bEngine)
		: type(_type)
		, ref_cnt(0)
		, is_engine_res(_bEngine)
	{
	}

	Resource::Resource(const Resource& _Other)
		: Entity(_Other)
		, type(_Other.type)
		, ref_cnt(0)
		, key(_Other.key)
		, relative_path(_Other.relative_path)
	{
	}

	Resource::~Resource()
	{
	}

	void Resource::Release()
	{
		--ref_cnt;

		if (0 == ref_cnt)
		{
			delete this;
		}
	}
}