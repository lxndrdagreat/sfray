//
// Created by Dan.Alexander on 2/15/2016.
//

#ifndef SFRAY_MAPTILE_HPP
#define SFRAY_MAPTILE_HPP

namespace sfray{

    enum MapTileType {
        Floor = 0,
        Wall = 1
    };

    class MapTile {
    protected:
        MapTileType     mTileType;
        bool            mIsSolid;

        int             mTextureIndex;
        unsigned int    mTextureWidth;
        unsigned int    mTextureHeight;

    public:
        MapTile();

        bool            isWall();
        bool            isFloor();

        void            setTileType(MapTileType t);
        MapTileType     getTileType();

        bool            isSolid();
        void            setIsSolid(bool is_solid);

        int             getTextureIndex();
        void            setTextureIndex(int index);

        unsigned int    getTextureWidth();
        unsigned int    getTextureHeight();
        void            setTextureWidth(unsigned int w);
        void            setTextureHeight(unsigned int h);
        void            setTextureSize(unsigned int width, unsigned int height);
    };
}


#endif //SFRAY_MAPTILE_HPP
