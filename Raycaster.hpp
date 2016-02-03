//
//  Raycaster.h
//  SFML Raycast Engine
//
//  Created by Daniel Alexander on 12/9/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//

#ifndef __SFML_Raycast_Engine__Raycaster__
#define __SFML_Raycast_Engine__Raycaster__

#include "Camera.hpp"
#include "Map.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

namespace sfray {
	
	enum WallRenderMethod {
		Wall_None = 0,
		Wall_Color,
		Wall_Texture
	};
	
	enum FloorRenderMethod {
		Floor_None = 0,
		Floor_Color,
		Floor_Texture
	};
	
	enum CeilingRenderMethod {
		Ceiling_None = 0,
		Ceiling_Color,
		Ceiling_Texture
	};
	
	enum EntityRenderMethod {
		Entity_None = 0,
		Entity_Texture
	};
    
    class Raycaster{
    private:
        unsigned int mGfxWidth;
        unsigned int mGfxHeight;
		
		// render options
		WallRenderMethod	mWallRenderMethod;
		FloorRenderMethod	mFloorRenderMethod;
		CeilingRenderMethod	mCeilingRenderMethod;
		float				mMaxObjectRenderDistance;
		EntityRenderMethod	mEntityRenderMethod;
		
		sf::Color			mWallRenderColor;
		sf::Color			mCeilingRenderColor;
		sf::Color			mFloorRenderColor;

		// Z Buffer and Height Map used to speed up calculations in the loop.
        std::vector<int> 	mZBuffer;
        std::vector<float> 	mHeightMap;

        // Floorcasting pieces
		sf::Uint8*			mFloorcastingPixels;
		sf::Texture			mFloorcastingTexture;
		sf::Sprite			mFloorcastingSprite;

        // The world map
        Map 				mMap;
        
		void updateForSize();
		void combSort(std::vector<int>& order, std::vector<float>& dist, int amount);
        
    public:
        Raycaster(int width_=320, int height_=240);
        
        // size accessors and setters
        int getWidth() { return mGfxWidth; };
        int getHeight() { return mGfxHeight; };
        void setWidth(unsigned int amount);
        void setHeight(unsigned int amount);
        void setSize(unsigned int width_, unsigned int height_);

        // Set our map
        void setMap(sfray::Map &map);

        // Draw
        void drawForCamera(sf::RenderWindow& window, Camera& camera);
		void drawForCamera(sf::RenderWindow& window, Camera& camera, sf::View& view);

        // Rendering option getters/setters
		WallRenderMethod getWallRenderMethod();
		void setWallRenderMethod(WallRenderMethod method);
		FloorRenderMethod getFloorRenderMethod();
		void setFloorRenderMethod(FloorRenderMethod method);
		CeilingRenderMethod getCeilingRenderMethod();
		void setCeilingRenderMethod(CeilingRenderMethod method);
		EntityRenderMethod getEntityRenderMethod();
		void setEntityRenderMethod(EntityRenderMethod method);
		
		float getMaxObjectRenderDistance();
		void setMaxObjectRenderDistance(float distance);
		
		void setCeilingRenderColor(const sf::Color& color);
		void setFloorRenderColor(const sf::Color& color);
		void setWallRenderColor(const sf::Color& color);
    };
    
};
#endif /* defined(__SFML_Raycast_Engine__Raycaster__) */
