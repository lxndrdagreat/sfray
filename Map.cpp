//
//  Map.cpp
//  SFML Raycast Engine
//
//  Created by Daniel Alexander on 12/9/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//

#include "Map.hpp"

namespace sfray {
    
    Map::Map(){
        mWidth = 0;
        mHeight = 0;
        mTileStoreOrder = TileStoreOrder::ColumnFirstOrder;
    }
	
	Map::~Map(){
		mData.clear();
		mTextures.clear();
		mTexturePixelData.clear();
		Rooms.clear();
		
//		for (auto entity : mEntities){
//			delete entity;
//		}
		mEntities.clear();
	}
    
    MapTile& Map::getTile(const unsigned int x, const unsigned int y){
        unsigned int index;
        if (mTileStoreOrder == TileStoreOrder::RowFirstOrder){
            index = y * mWidth + x;
        }
        else if (mTileStoreOrder == TileStoreOrder::ColumnFirstOrder){
            index = x * mWidth + y;
        }
        return this->getTile(index);
    }

    MapTile &Map::getTile(const unsigned int index) {
        if (index >= mData.size()){
            throw "Map::getTile ERROR: index out of bounds.";
        }
        return mData[index];
    }
    
    void Map::loadTexture(int numeric_index, const std::string &path){
        mTextures[numeric_index] = sf::Texture();
        if (!mTextures[numeric_index].loadFromFile(path)){
            throw "Could not load texture needed by map: '" + path + "'";
        }
		
		// store pixel data
        sf::Image imageCopy = mTextures[numeric_index].copyToImage();
		const sf::Uint8* data = imageCopy.getPixelsPtr();
        if (data == nullptr){
            throw "Could not get pixel data from image.";
        }
        unsigned int textureWidth = mTextures[numeric_index].getSize().x;
        unsigned int textureHeight = mTextures[numeric_index].getSize().y;
		for (unsigned int i = 0; i < textureWidth*textureHeight*4; i += 4) {
            sf::Color c(data[i], data[i + 1], data[i + 2], data[i + 3]);
            mTexturePixelData[numeric_index].push_back(c);
        }
    }
    
    sf::Texture& Map::getTexture(int index){
        return mTextures[index];
    }
	
	sf::Color Map::getPixelFromTexture(int index, int x, int y){
		int pixel_index = y * mTextures[index].getSize().x + x;
		return mTexturePixelData[index][pixel_index];
	}
	
	void Map::addEntity(sfray::Entity* entity){
		mEntities.push_back(entity);
	}
	
	std::vector<sfray::Entity*> Map::getEntities(){
		return mEntities;
	}

    void Map::setDataFromIntArray(const std::vector<int> data, unsigned int width, unsigned int height) {
        mWidth = width;
        mHeight = height;

        // clear any existing data
        mData.clear();

        for (auto& intTile : data){
            MapTile tile;
            tile.setTextureIndex(intTile);
            if (intTile == 0){
                // floor
                tile.setTileType(sfray::MapTileType::Floor);
            }
            else{
                tile.setTileType(sfray::MapTileType::Wall);
            }

            tile.setTextureWidth(mTextures[tile.getTextureIndex()].getSize().x);
            tile.setTextureHeight(mTextures[tile.getTextureIndex()].getSize().y);

            mData.push_back(tile);
        }
    }

    void Map::setDataFromMapTileArray(std::vector<MapTile> data, unsigned int width, unsigned int height) {
        mWidth = width;
        mHeight = height;

        // clear any existing data
        mData.clear();

        for (auto& tile : data){
            tile.setTextureWidth(mTextures[tile.getTextureIndex()].getSize().x);
            tile.setTextureHeight(mTextures[tile.getTextureIndex()].getSize().y);

            mData.push_back(tile);
        }
    }

    TileStoreOrder Map::getTileStoreOrder() {
        return mTileStoreOrder;
    }

    void Map::setTileStoreOrder(TileStoreOrder order) {
        mTileStoreOrder = order;
    }
}