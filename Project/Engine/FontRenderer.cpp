#include "pch.h"
#include "FontRenderer.h"

#include "ResourceMgr.h"
#include "Transform.h"

#include "StructuredBuffer.h"
#include "FontMgr.h"

namespace ff7r
{
	FontRenderer::FontRenderer()
		: RenderComponent(COMPONENT_TYPE::FONTRENDERER)
	{
		buffer = new StructuredBuffer;
		buffer->Create(sizeof(Texts), 1, SB_TYPE::READ_WRITE, true);
		SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"PointMesh"));
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"TextMtrl"));
	}

	FontRenderer::~FontRenderer()
	{
		if (nullptr != buffer)
			delete buffer;
	}

	void FontRenderer::FinalTick()
	{
	}

	void FontRenderer::Render()
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial())
			return;

		// Transform 에 UpdateData 요청
		GetTransform()->UpdateData();

		// 구조화버퍼 업데이트
		UpdateData();

		// 렌더

		Ptr<Mesh> _mesh = GetMesh();

		if (nullptr == _mesh || nullptr == GetMaterial())
			return;



		for (size_t i = 0; i < _mesh->GetSubsetCount(); ++i)
		{
			UINT _mtrl_id = _mesh->GetMaterialID(i);

			GetMaterial(_mtrl_id)->UpdateData();
			_mesh->Render(i);
		}

		buffer->Clear();
		FontMgr::GetInst()->FontClear();
	}

	void FontRenderer::UpdateData()
	{
 		if (texts.first == 0)
			return;
		FontMgr::GetInst()->SetFont();
		texts.font_size.x = GetTransform()->GetRelativeScale().x;
		texts.font_size.y = GetTransform()->GetRelativeScale().y;
		//texts[14] = (int)GetTransform()->GetRelativeScale().x;
		//texts[15] = (int)GetTransform()->GetRelativeScale().y;
		buffer->SetData(&texts, 0);
		buffer->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);
	}
	void FontRenderer::SetTexts(string _texts, bool _is_text)
	{
		texts = {};
		texts.first = 1;

		texts.is_text = _is_text;
		
		if(texts.is_text)
		{
			for (int i = 0; i < 17; i++)
			{
				if (i >= _texts.size())
				{
					texts.sentence[i] = -1.0f;
					continue;
				}
				char _text = _texts[i];
				if (_text == ' ')
				{
					texts.sentence[i] = ((int)(_text - ' '));
				}
				else if (_text >= 'A' && _text <= 'Z')
				{
					texts.sentence[i] = ((int)(_text - 'A') + 1);
				}
				else if (_text >= 'a' && _text <= 'z')
				{
					texts.sentence[i] = ((int)(_text - 'a') + 27);
				}
				else if (_text >= '0' && _text <= '9')
				{
					texts.sentence[i] = ((int)(_text - '0') + 53);
				}
			}
		}
		else
		{
			for (int i = 0; i < 17; i++)
			{
				if (i >= _texts.size())
				{
					texts.sentence[i] = -1.0f;
					continue;
				}
				char _text = _texts[i];

				if (_text >= '0' && _text <= '9')
				{
					texts.sentence[i] = ((int)(_text - '0'));
				}
			}
		}
	}
}