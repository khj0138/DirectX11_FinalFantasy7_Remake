#pragma once

#include "RenderComponent.h"


namespace ff7r
{
    class StructuredBuffer;
    class TileMap : public RenderComponent
    {
    private:
        UINT                tile_cnt_x;  // 타일 가로
        UINT                tile_cnt_y;  // 타일 세로
        vec2                slice_size;   // 타일 하나의 크기(UV 단위)
        vector<Tile>        tiles;
        StructuredBuffer*   buffer;

    public:
        virtual void FinalTick() override;
        virtual void Render() override;
        virtual void Render(UINT _i) override {}

        void UpdateData();

        void SetTileCount(UINT _iXCount, UINT _iYCount);
        void SetSliceSize(vec2 _vSliceSize) { slice_size = _vSliceSize; }


        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;


        CLONE(TileMap);
    public:
        TileMap();
        ~TileMap();
    };

}