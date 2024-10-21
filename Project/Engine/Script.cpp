#include "pch.h"
#include "Script.h"

namespace ff7r
{
	Script::Script(UINT _ScriptType)
		: Component(COMPONENT_TYPE::SCRIPT)
		, script_type(_ScriptType)
	{
	}

	Script::~Script()
	{
	}

	void Script::AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc)
	{
		parameters.push_back(ScriptParam{ eParam , _pData , _Desc });
	}
}