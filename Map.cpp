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
        _width = 0;
        _height = 0;
        _data.clear();
    }
	
	Map::~Map(){
		_data.clear();
		_textures.clear();
		mTexturePixelData.clear();
		Rooms.clear();
		
//		for (auto entity : mEntities){
//			delete entity;
//		}
		mEntities.clear();
	}
    
    void Map::SetDataFromIntArray(const std::vector<std::vector<int> > data, const std::vector<std::vector<int> > tiletype){
        
        _width = data.size();
        _height = data[0].size();
    
        // clear data, jic
        _data.clear();
        
        for (int x = 0; x < _width; ++x){
            _data.push_back(std::vector<MapTile>());
            
            for (int y = 0; y < _height; ++y){
                MapTile tile;
                tile.TextureIndex = data[x][y];
                if (tiletype[x][y] == 0){
                    // floor
                    tile.IsFloor = true;
                    tile.IsWall = false;
                }
                else{
                    tile.IsWall = true;
                    tile.IsFloor = false;
                }
                
                _data[x].push_back(tile);
            }
        }
        
    }
    
    void Map::CreateFromBSP(const int width, const int height){
        _width = width;
        _height = height;
        
        _data.clear();
        
        GenerationResults builtBSP = makeBSP(_width, _height);
        Rooms = builtBSP.rooms;
        
        for (int x = 0; x < _width; ++x){
            for (int y = 0; y < _height;++y){
                _data.push_back(std::vector<MapTile>());
                
                for (int y = 0; y < _height; ++y){
                    MapTile tile;
                    tile.TextureIndex = builtBSP.mapArray[x][y];
                    if (builtBSP.mapArray[x][y] == 0){
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
                    
                    _data[x].push_back(tile);
                }

            }
        }
    }
    
    MapTile& Map::GetTile(const unsigned int x, const unsigned int y){
		_data[x][y].TextureWidth = _textures[_data[x][y].TextureIndex].getSize().x;
		_data[x][y].TextureHeight = _textures[_data[x][y].TextureIndex].getSize().y;
        return _data[x][y];
    }
    
    void Map::LoadTexture(int numeric_index, const std::string &path){
        _textures[numeric_index] = sf::Texture();
        if (!_textures[numeric_index].loadFromFile(path)){
            throw "Could not load texture needed by map: '" + path + "'";
        }
		
		// store pixel data
		const sf::Uint8* data = _textures[numeric_index].copyToImage().getPixelsPtr();
		for (unsigned int i = 0; i < _textures[numeric_index].getSize().x * _textures[numeric_index].getSize().y * 4; i += 4){
			sf::Color c(data[i], data[i+1], data[i+2], data[i+3]);
			mTexturePixelData[numeric_index].push_back(c);
		}
		//mTexturePixelData[numeric_index] = _textures[numeric_index].copyToImage().getPixelsPtr();
    }
    
    sf::Texture& Map::GetTexture(int index){
        return _textures[index];
    }
	
	sf::Color Map::GetPixelFromTexture(int index, int x, int y){
		int pixel_index = y * _textures[index].getSize().x + x;
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