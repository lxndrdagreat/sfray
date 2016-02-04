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
    
    void Map::SetDataFromIntArray(const std::vector<std::vector<int> > data){
        
        mWidth = data.size();
        mHeight = data[0].size();

        // clear data, jic
        mData.clear();
        
        for (int x = 0; x < mWidth; ++x){
            mData.push_back(std::vector<MapTile>());
            for (int y = 0; y < mHeight; ++y){
                MapTile tile;
                tile.TextureIndex = data[x][y];
                if (data[x][y] == 0){
                    // floor
                    tile.IsFloor = true;
                    tile.IsWall = false;
                    int chance = rand() % 10;
                    if (chance <= 2){
                        tile.TextureIndex = -1;
                    }
                }
                else{
                    tile.IsWall = true;
                    tile.IsFloor = false;
                }
                
                mData[x].push_back(tile);
            }
        }
    }
    
    MapTile& Map::GetTile(const unsigned int x, const unsigned int y){
		mData[x][y].TextureWidth = mTextures[mData[x][y].TextureIndex].getSize().x;
		mData[x][y].TextureHeight = mTextures[mData[x][y].TextureIndex].getSize().y;
        return mData[x][y];
    }
    
    void Map::LoadTexture(int numeric_index, const std::string &path){
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
    
    sf::Texture& Map::GetTexture(int index){
        return mTextures[index];
    }
	
	sf::Color Map::GetPixelFromTexture(int index, int x, int y){
		int pixel_index = y * mTextures[index].getSize().x + x;
//		sf::Color c(mTexturePixelData[index][pixel_index], 
//					mTexturePixelData[index][pixel_index+1],
//					mTexturePixelData[index][pixel_index+2],
//					mTexturePixelData[index][pixel_index+3]);
		
		return mTexturePixelData[index][pixel_index];
	}
	
	void Map::addEntity(sfray::Entity* entity){
		mEntities.push_back(entity);
	}
	
	std::vector<sfray::Entity*> Map::getEntities(){
		return mEntities;
	}
    
}