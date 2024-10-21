#include "Value.hlsli"
#include "Struct.hlsli"

StructuredBuffer<GlobalFont> FontBuffer : register(t20);
StructuredBuffer<Texts> TextsData : register(t21);
StructuredBuffer<DamageFont> DamageFontBuffer : register(t22);

//#define Font        FontBuffer[0]
#define TextData    TextsData[0]


struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

struct GSOut
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// 문장 개수 달라지면 바꿔야함
[maxvertexcount(102)]
void main(point VSOut _in[1], inout TriangleStream<GSOut> outstream)
{
    if (TextData.sentence[0] == -1)
        return;
    
    
    float3 view_pos = mul(_in[0].pos, Mat_View).xyz;
    float2 _box_size = TextData.box_size;
    
    
    GSOut output[4] = { (GSOut) 0.f, (GSOut) 0.f, (GSOut) 0.f, (GSOut) 0.f };
    //output[0].pos = (float4) 0.f;
    //output[0].color = (float4) 0.f;
    //output[0].uv = (float2) 0.f;
    float2 _sum = (float2) 0.0f;
    int i;

    
    
    for (i = 0; i < 17; i++)
    {
        int _text_idx = TextData.sentence[i];
        if (_text_idx == -1)
            break;
        
        Font _font;
        if (TextData.is_text)
        {
            _font = FontBuffer[0].Fonts[_text_idx];
        }
        else
        {
            _font = DamageFontBuffer[0].Fonts[_text_idx];
        }
        
        _sum += _font.text_size;
    }
    _sum *= _box_size;
    
    float _cur_pos = 0.0f;
    for (i = 0; i < 17; i++)
    {
        int _text_idx = TextData.sentence[i];
        if (_text_idx == -1)
            break;
        
        
        Font _font;
        if (TextData.is_text)
        { 
            _font = FontBuffer[0].Fonts[_text_idx];
        }
        else
        {
            _font = DamageFontBuffer[0].Fonts[_text_idx];
        }
        
        float3 _center_pos = view_pos;
        _center_pos.x += _cur_pos;
        _center_pos.x += _font.text_size.x * 0.5f * _box_size.x;
       // _center_pos.x -= _sum.x * 0.5f;
        
        _cur_pos += _font.text_size.x * _box_size.x;
        
        
        output[0].pos = mul(float4(_center_pos + float3(_font.text_size.x * -0.5f * _box_size.x, _font.text_size.y * _box_size.y * 0.5f, 0.0f), 1.f), Mat_Proj);
        output[0].uv = float2(_font.text_uv.x, 0.f);
        output[0].color = _in[0].color;

        output[1].pos = mul(float4(_center_pos + float3(_font.text_size.x * 0.5f * _box_size.x, _font.text_size.y * _box_size.y * 0.5f, 0.0f), 1.f), Mat_Proj);
        output[1].uv = float2(_font.text_uv.y, 0.f);
        output[1].color = _in[0].color;

    
        output[2].pos = mul(float4(_center_pos + float3(_font.text_size.x * 0.5f * _box_size.x, _font.text_size.y * _box_size.y * -0.5f, 0.0f), 1.f), Mat_Proj);
        output[2].uv = float2(_font.text_uv.y, _font.text_uv.z);
        output[2].color = _in[0].color;

    
        output[3].pos = mul(float4(_center_pos + float3(_font.text_size.x * -0.5f * _box_size.x, _font.text_size.y * _box_size.y * -0.5f, 0.0f), 1.f), Mat_Proj);
        output[3].uv = float2(_font.text_uv.x, _font.text_uv.z);
        output[3].color = _in[0].color;

        
         // 정점 생성
        outstream.Append(output[0]);
        outstream.Append(output[1]);
        outstream.Append(output[2]);
        outstream.RestartStrip();
        outstream.Append(output[0]);
        outstream.Append(output[2]);
        outstream.Append(output[3]);
        outstream.RestartStrip();
    }
    
    
}