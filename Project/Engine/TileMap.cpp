#include "pch.h"
#include "TileMap.h"

#include "ResourceMgr.h"
#include "Transform.h"

#include "StructuredBuffer.h"

namespace ff7r
{
	TileMap::TileMap()
		: RenderComponent(COMPONENT_TYPE::TILEMAP)
		, tile_cnt_x(1)
		, tile_cnt_y(1)
	{
		SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"TileMapMtrl"));

		buffer = new StructuredBuffer;
		buffer->Create(sizeof(Tile), tile_cnt_x * tile_cnt_y, SB_TYPE::READ_ONLY, true);
	}

	TileMap::~TileMap()
	{
		if (nullptr != buffer)
			delete buffer;
	}

	void TileMap::FinalTick()
	{
	}

	void TileMap::Render()
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial())
			return;

		// Transform 에 UpdateData 요청
		GetTransform()->UpdateData();

		// 재질 업데이트
		GetMaterial()->SetScalarParam(INT_0, &tile_cnt_x);
		GetMaterial()->SetScalarParam(INT_1, &tile_cnt_y);
		GetMaterial()->UpdateData();

		// 구조화버퍼 업데이트
		UpdateData();

		// 렌더
		GetMesh()->Render();
	}

	void TileMap::UpdateData()
	{
		buffer->SetData(tiles.data(), sizeof(Tile) * tiles.size());
		buffer->UpdateData(20, PIPELINE_STAGE::PS_PIXEL);
	}

	void TileMap::SetTileCount(UINT _iXCount, UINT _iYCount)
	{
		tile_cnt_x = _iXCount;
		tile_cnt_y = _iYCount;

		tiles.clear();
		tiles.resize(tile_cnt_x * tile_cnt_y);

		if (buffer->GetElementCount() < tiles.size())
		{
			buffer->Create(sizeof(Tile), (UINT)tiles.size(), SB_TYPE::READ_ONLY, true);
		}

		// 타일 세팅 테스트
		for (size_t i = 0; i < tile_cnt_y; ++i)
		{
			for (size_t j = 0; j < tile_cnt_x; ++j)
			{
				tiles[i * tile_cnt_x + j].left_top.x = slice_size.x * j;
				tiles[i * tile_cnt_x + j].left_top.y = 0.f;
				tiles[i * tile_cnt_x + j].slice = slice_size;
			}
		}

		tiles[0].left_top = vec2(slice_size.x * 7.f, slice_size.y * 5.f);
	}

	void TileMap::SaveToLevelFile(FILE* _File)
	{
		RenderComponent::SaveToLevelFile(_File);

		fwrite(&tile_cnt_x, sizeof(UINT), 1, _File);
		fwrite(&tile_cnt_y, sizeof(UINT), 1, _File);
		fwrite(&slice_size, sizeof(vec2), 1, _File);
		fwrite(tiles.data(), sizeof(Tile), tiles.size(), _File);
	}

	void TileMap::LoadFromLevelFile(FILE* _File)
	{
		RenderComponent::LoadFromLevelFile(_File);

		fread(&tile_cnt_x, sizeof(UINT), 1, _File);
		fread(&tile_cnt_y, sizeof(UINT), 1, _File);
		fread(&slice_size, sizeof(vec2), 1, _File);

		SetTileCount(tile_cnt_x, tile_cnt_y);

		fread(tiles.data(), sizeof(Tile), tile_cnt_x * tile_cnt_y, _File);
	}
}