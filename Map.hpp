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
#include "MapTile.hpp"

namespace sfray {

    /* TileStoreOrder relates to how the
     * 2D map is stored in a 1D array.
     */
    enum TileStoreOrder {
        ColumnFirstOrder = 0, // x and then y
        RowFirstOrder // y and then x
    };

    class Map {
    protected:
        TileStoreOrder mTileStoreOrder;
        std::vector<MapTile> mData;
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
        int getWidth() { return mWidth; };
        int getHeight() { return mHeight; };
        
        MapTile& getTile(const unsigned int x, const unsigned int y);
		MapTile& getTile(const unsigned int index);

        TileStoreOrder getTileStoreOrder();
        void            setTileStoreOrder(TileStoreOrder order);

        // Set map data
        void setDataFromIntArray(const std::vector<int> data, unsigned int width, unsigned int height);
        void setDataFromMapTileArray(std::vector<MapTile> data, unsigned int width, unsigned int height);

        void loadTexture(int numeric_index, const std::string &path);
        
        sf::Texture&getTexture(int index);
		sf::Color getPixelFromTexture(int index, int x, int y);
		
		std::vector<sfray::Entity*> getEntities();
		void addEntity(sfray::Entity* entity);
    };
}

#endif /* defined(__SFML_Raycast_Engine__Map__) */
