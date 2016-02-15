//
// Created by Dan.Alexander on 2/15/2016.
//

#include "MapTile.hpp"

namespace sfray{

    MapTile::MapTile()
    :mTileType(Floor)
    ,mIsSolid(false)
    ,mTextureIndex(0)
    { }

    bool MapTile::isWall() {
        return mTileType == MapTileType::Wall;
    }

    bool MapTile::isFloor() {
        return mTileType == MapTileType::Floor;
    }

    bool MapTile::isSolid() {
        return mIsSolid;
    }

    void MapTile::setIsSolid(bool is_solid) {
        mIsSolid = is_solid;
    }

    void MapTile::setTileType(MapTileType t) {
        mTileType = t;
        if (mTileType == MapTileType::Wall){
            mIsSolid = true;
        }
        else{
            mIsSolid = false;
        }
    }

    MapTileType MapTile::getTileType() {
        return mTileType;
    }

    int MapTile::getTextureIndex() {
        return mTextureIndex;
    }

    void MapTile::setTextureIndex(int index) {
        mTextureIndex = index;
    }


    unsigned int MapTile::getTextureWidth() {
        return mTextureWidth;
    }

    unsigned int MapTile::getTextureHeight() {
        return mTextureHeight;
    }

    void MapTile::setTextureWidth(unsigned int w) {
        mTextureWidth = w;
    }

    void MapTile::setTextureHeight(unsigned int h) {
        mTextureHeight = h;
    }

    void MapTile::setTextureSize(unsigned int width, unsigned int height) {
        mTextureWidth = width;
        mTextureHeight = height;
    }
}