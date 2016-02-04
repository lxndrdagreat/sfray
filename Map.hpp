//
//  Map.h
//  SFML Raycast Engine
//
//  Created by Daniel Alexander on 12/9/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//

#ifndef __SFML_Raycast_Engine__Map__
#define __SFML_Raycast_Engine__Map__

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "MapUtils.h"

namespace sfray {
    
    struct MapTile{
        int TextureIndex = 0;
		int TextureWidth = 0;
		int TextureHeight = 0;
        bool IsFloor = true;
        bool IsWall = false;
    };
    
    class Map {
    private:
        std::vector<std::vector<MapTile> > mData;
        std::map<int,sf::Texture> mTextures;
		std::map<int, std::vector<sf::Color>> mTexturePixelData;
        int mWidth;
        int mHeight;
		
		std::vector<sfray::Entity*> mEntities;
        
    public:
        Map();
		~Map();
        
        std::vector<RoomDim> Rooms;
        
        // getters
        int Width() { return mWidth; };
        int Height() { return mHeight; };
        
        MapTile& GetTile(const unsigned int x, const unsigned int y);
        
        void SetDataFromIntArray(const std::vector<std::vector<int> > data);
        
        void LoadTexture(int numeric_index, const std::string& path);
        
        sf::Texture& GetTexture(int index);
		sf::Color GetPixelFromTexture(int index, int x, int y);
		
		std::vector<sfray::Entity*> getEntities();
		void addEntity(sfray::Entity* entity);
    };
}

#endif /* defined(__SFML_Raycast_Engine__Map__) */
