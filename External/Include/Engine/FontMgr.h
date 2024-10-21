//#pragma once
//#include "Singleton.h"
//
//#include <FontEngine/FW1FontWrapper.h>
//#include <FontEngine/FW1CompileSettings.h>
//
//#ifdef _DEBUG
//#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
//#else
//#pragma comment(lib, "FontEngine/FW1FontWrapper"))
//#endif
//
//namespace ff7r
//{
//#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)
//
//	class FontMgr : public Singleton<FontMgr>
//	{
//	public:
//		FontMgr();
//		~FontMgr();
//
//		void Init();
//		void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
//
//	private:
//		IFW1Factory*		fw1_factory;
//		IFW1FontWrapper*	font_wrapper;
//	};
//}

#pragma once
#include "Singleton.h"

namespace ff7r
{
	class Font;
	class StructuredBuffer;
	class FontMgr : public Singleton<FontMgr>
	{
	public:

		void Init();
		void MakeSentence(string _sentence, vec2 _pos);
		void MakeDamage(string _sentence, vec2 _pos);

		//void RegisterTexts(string _texts);
		void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
		void SetFont();

		void FontClear();
		//void UpdateData

	private:
		SINGLE(FontMgr);

		Font* font;
		Font* damage_font;

		StructuredBuffer* font_buffer;
		StructuredBuffer* damage_font_buffer;

	};
}