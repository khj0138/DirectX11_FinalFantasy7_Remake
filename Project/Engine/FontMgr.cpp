#include "pch.h"
#include "FontMgr.h"
#include "Font.h"
#include "RenderMgr.h"
#include "ResourceMgr.h"
#include "Mesh.h"
#include "StructuredBuffer.h"

//#include "Device.h"
//
//namespace ff7r
//{
//	FontMgr::FontMgr()
//		: fw1_factory(nullptr)
//		, font_wrapper(nullptr)
//	{
//	}
//
//	FontMgr::~FontMgr()
//	{
//		if (nullptr != fw1_factory)
//			fw1_factory->Release();
//
//		if (nullptr != font_wrapper)
//			font_wrapper->Release();
//	}
//
//	void FontMgr::Init()
//	{
//		if (FAILED(FW1CreateFactory(FW1_VERSION, &fw1_factory)))
//		{
//			assert(NULL);
//		}
//
//		if (FAILED(fw1_factory->CreateFontWrapper(DEVICE, L"Arial", &font_wrapper)))
//		{
//			assert(NULL);
//		}
//	}
//
//	void FontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
//	{
//		font_wrapper->DrawString(
//			CONTEXT,
//			_pStr, // String
//			_fFontSize,// Font size
//			_fPosX,// X position
//			_fPosY,// Y position
//			_Color,// Text color, 0xAaBbGgRr
//			FW1_RESTORESTATE      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
//		);
//	}
//}


namespace ff7r
{
	FontMgr::FontMgr()
		: font(nullptr)
		, font_buffer(nullptr)
	{
	}

	FontMgr::~FontMgr()
	{
		if (font)
		{
			delete font;
			font = nullptr;
		}

		if (font_buffer)
		{
			delete font_buffer;
			font_buffer = nullptr;
		}

		if (damage_font)
		{
			delete damage_font;
			damage_font = nullptr;
		}

		if (damage_font_buffer)
		{
			delete damage_font_buffer;
			damage_font_buffer = nullptr;
		}
	}

	void FontMgr::Init()
	{
		font = new Font();
		wstring _wtsr_file_path = PathMgr::GetInst()->GetContentPath();
		wstring _wstr_font_path = wstring(L"texture\\English_Font_Data.txt");
		_wtsr_file_path += _wstr_font_path;
		string _str_font_path = string(_wtsr_file_path.begin(), _wtsr_file_path.end());

		char* _font_path = new char[_str_font_path.size() + 1];
		strcpy_s(_font_path, _str_font_path.size() + 1, _str_font_path.c_str());

		font->Initialize(_font_path, 63, vec2(1646.f, 46.0f));

		delete _font_path;
		font_buffer = new StructuredBuffer();
		font_buffer->Create(sizeof(GlobalFont), 1, SB_TYPE::READ_WRITE, true);

		{
			damage_font = new Font();
			wstring _wtsr_file_path = PathMgr::GetInst()->GetContentPath();
			wstring _wstr_font_path = wstring(L"texture\\Damage_Font_Data.txt");
			_wtsr_file_path += _wstr_font_path;
			string _str_font_path = string(_wtsr_file_path.begin(), _wtsr_file_path.end());
		
			char* _font_path = new char[_str_font_path.size() + 1];
			strcpy_s(_font_path, _str_font_path.size() + 1, _str_font_path.c_str());
		
		
			damage_font->Initialize(_font_path, 10, vec2(410.f, 66.0f));
		
			if (_font_path)
			{
				delete _font_path;
			}
		
			damage_font_buffer = new StructuredBuffer();
			damage_font_buffer->Create(sizeof(DamageFont), 1, SB_TYPE::READ_WRITE, true);
		}
	}

	void FontMgr::MakeSentence(string _str_sentence, vec2 _pos)
	{
		vector<Vtx> _vertex;
		vector<UINT> _index;
		Vtx _v;

		Ptr<Mesh> _mesh = nullptr;

		// ==============
		// PointMesh »ý¼º
		// ==============

		int _vertex_count = _str_sentence.size() * 6;

		char* _sentence = new char[_str_sentence.size() + 1];
		strcpy_s(_sentence, _str_sentence.size() + 1, _str_sentence.c_str());

		font->BuildVertexArray(_vertex, _sentence, _pos);

		for (int i = 0; i < _vertex_count; i++)
		{
			_index.push_back(i);
		}

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size(), true);
		wstring _wstr_sentence = wstring(_str_sentence.begin(), _str_sentence.end());
		ResourceMgr::GetInst()->AddRes(_wstr_sentence.c_str(), _mesh);

		delete _sentence;
	}

	void FontMgr::MakeDamage(string _str_sentence, vec2 _pos)
	{
		vector<Vtx> _vertex;
		vector<UINT> _index;
		Vtx _v;

		Ptr<Mesh> _mesh = nullptr;

		int _vertex_count = _str_sentence.size() * 6;

		char* _sentence = new char[_str_sentence.size() + 1];
		strcpy_s(_sentence, _str_sentence.size() + 1, _str_sentence.c_str());

		damage_font->BuildVertexArray(_vertex, _sentence, _pos);

		for (int i = 0; i < _vertex_count; i++)
		{
			_index.push_back(i);
		}

		_mesh = new Mesh(true);
		_mesh->Create(_vertex.data(), (UINT)_vertex.size(), _index.data(), (UINT)_index.size(), true);
		wstring _wstr_sentence = wstring(_str_sentence.begin(), _str_sentence.end());
		ResourceMgr::GetInst()->AddRes(_wstr_sentence.c_str(), _mesh);

		delete _sentence;
	}

	/*void FontMgr::RegisterTexts(string _texts)
	{

	}*/

	void FontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
	{

	}
	void FontMgr::SetFont()
	{
		vector<Font::FontType>& _fonts = font->GetFonts();

		font_buffer->SetData(_fonts.data(), sizeof(GlobalFont));
		font_buffer->UpdateData(20, PIPELINE_STAGE::PS_GEOMETRY);

		vector<Font::FontType>& _fonts_d = damage_font->GetFonts();

		damage_font_buffer->SetData(_fonts_d.data(), sizeof(DamageFont));
		damage_font_buffer->UpdateData(22, PIPELINE_STAGE::PS_GEOMETRY);
	}
	void FontMgr::FontClear()
	{
		damage_font_buffer->Clear();
		font_buffer->Clear();
	}
}