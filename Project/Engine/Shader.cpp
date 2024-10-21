#include "pch.h"
#include "Shader.h"

namespace ff7r
{
	Shader::Shader(RES_TYPE _eType)
		: Resource(_eType, true)
	{
	}

	Shader::~Shader()
	{
	}
}