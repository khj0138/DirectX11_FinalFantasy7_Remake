#pragma once

#include "RenderComponent.h"


namespace ff7r
{
    class StructuredBuffer;
    class TileMap : public RenderComponent
    {
    private:
        UINT                tile_cnt_x;  // Ÿ�� ����
        UINT                tile_cnt_y;  // Ÿ�� ����
        vec2                slice_size;   // Ÿ�� �ϳ��� ũ��(UV ����)
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