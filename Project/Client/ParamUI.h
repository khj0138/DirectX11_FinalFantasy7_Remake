#pragma once

#include <Engine\ptr.h>
#include <Engine\Texture.h>


#include "ImGuiMgr.h"
namespace ff7r
{
	class UI;
	class ParamUI
	{
	private:
		static UINT		g_NextId;	// �Ķ���� UI ���� ID

	public:
		static int Param_Int(const string& _strDesc, int* _pData, bool _bDrag = false);
		static int Param_Float(const string& _strDesc, float* _pData, bool _bDrag = false);
		static int Param_Vec2(const string& _strDesc, vec2* _pData, bool _bDrag = false);
		static int Param_Vec3(const string& _strDesc, vec3* _pData, bool _bDrag = false);
		static int Param_Vec4(const string& _strDesc, vec4* _pData, bool _bDrag = false);

		static int Param_Tex(const string& _strDesc, Ptr<Texture>& _CurTex, UI* _UI, UI_DELEGATE_1 _Func);


	private:
		static string GetNextName(const string& _ShowName);

		friend class ImGuiMgr;
	};

}