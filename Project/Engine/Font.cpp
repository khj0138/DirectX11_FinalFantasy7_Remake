#include "pch.h"
#include "Font.h"
#include <fstream>

namespace ff7r
{
    Font::Font()
    {
    }
    Font::~Font()
    {
        ReleaseFontData();
    }
    bool Font::Initialize(char* _font_file_name, int _font_cnt, vec2 _size)
    {
        if (!LoadFontData(_font_file_name, _font_cnt, _size))
        {
            return false;
        }

        return true;
    }
    void Font::BuildVertexArray(vector<Vtx>& _vertices, char* _sentence, vec2 _pos)
    {

        int _len_sentence = (int)strlen(_sentence);


        int _text_idx = 0;
        int _text_num = 0;

        float _sum = 0.0f;
        for (int i = 0; i < _len_sentence; i++)
        {
            _text_num = (int)_sentence[i];
            _sum += fonts[_text_num].size.x;
        }
        float _sum2 = 0.0f;
        for (int i = 0; i < _len_sentence; i++)
        {
            _text_num = (int)_sentence[i];

            Vtx _v;



            _v.pos = vec3(-0.5f + _sum2 / _sum, -0.5f, 0.0f);
            _v.uv = vec2(fonts[_text_num].left, 1.0f); // 3
            _text_idx++;
            _vertices.push_back(_v);

            _v.pos = vec3(-0.5f + (_sum2 + fonts[_text_num].size.x) / _sum, 0.5f, 0.0f);
            _v.uv = vec2(fonts[_text_num].right, 0.0f); //1
            _text_idx++;
            _vertices.push_back(_v);
            _v.pos = vec3(-0.5f + _sum2 / _sum, 0.5f, 0.0f);
            _v.uv = vec2(fonts[_text_num].left, 0.0f); // 0
            _text_idx++;
            _vertices.push_back(_v);

            _v.pos = vec3(-0.5f + (_sum2 + fonts[_text_num].size.x) / _sum, 0.5f, 0.0f);
            _v.uv = vec2(fonts[_text_num].right, 0.0f); // 1
            _text_idx++;
            _vertices.push_back(_v);
            _v.pos = vec3(-0.5f + _sum2 / _sum, -0.5f, 0.0f);
            _v.uv = vec2(fonts[_text_num].left, 1.0f); // 3
            _text_idx++;
            _vertices.push_back(_v);
            _v.pos = vec3(-0.5f + (_sum2 + fonts[_text_num].size.x) / _sum, -0.5f, 0.0f);
            _v.uv = vec2(fonts[_text_num].right, 1.0f); // 2
            _text_idx++;
            _vertices.push_back(_v);
            _sum2 += fonts[_text_num].size.x;
        }

    }
    bool Font::LoadFontData(char* _str_file_path, int _font_cnt, vec2 _size)
    {
        std::ifstream _fin;
        char _temp;

        _fin.open(_str_file_path);
        if (_fin.fail())
        {
            return false;
        }

        for (int i = 0; i < _font_cnt; i++)
        {
            int _idx = -1;
            _fin.get(_temp);
            _fin >> _idx;
            while (_temp != ' ')
            {
                _fin.get(_temp);
            }
            _fin.get(_temp);
            while (_temp != ' ')
            {
                _fin.get(_temp);
            }

            FontType _font = FontType();
            //_font.padding = -1.0f;
            _fin >> _font.left;
            _fin >> _font.right;
            _fin >> _font.height;
            //_font.left += 1.0f;
            _font.size.x = _font.right - _font.left;// +1.0f;// -1.0f;
            _font.size.y = _font.height;
            _font.left /= _size.x;
            //_font.right -= 1.0f;
            _font.right /= _size.x;
            _font.height /= _size.y;
            if (_idx != -1)
            {

                /*if (_idx >= 65 && _idx <= 90)
                {
                    _idx -= 65;
                }
                else if (_idx >= 48 && _idx <= 57)
                {

                }
                else if (_idx >= 15 && _idx <= 24)
                {

                }*/
                //fonts[_idx] = _font;
                fonts.push_back(_font);
            }
        }

        return false;
    }

    void Font::ReleaseFontData()
    {

    }
}
