#pragma once
namespace ff7r
{
	class Font
	{
	private:

	public:
		struct FontType
		{
			float left, right, height;
			vec2 size;
		};
		Font();
		~Font();

		bool Initialize(char*, int _font_cnt, vec2 _siz);
		void BuildVertexArray(vector<Vtx>&, char* _sentence, vec2 _pos);
		vector<FontType>& GetFonts() { return fonts; }

	private:
		//struct FontType
		//{
		//	float left, right;
		//	int size;
		//};

		bool LoadFontData(char* _str_file_path, int _font_cnt, vec2 _size);
		void ReleaseFontData();

		//map<int, FontType> fonts;
		vector<FontType> fonts;

	};

}
