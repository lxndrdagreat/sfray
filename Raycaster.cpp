//
//  Raycaster.cpp
//  SFML Raycast Engine
//
//  Created by Daniel Alexander on 12/9/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//

#include "Raycaster.hpp"
#include <cmath>
#include "Entity.hpp"

namespace sfray {
    
    
    Raycaster::Raycaster(int width_, int height_){
        mGfxWidth = width_;
        mGfxHeight = height_;
		
		mWallRenderMethod = Wall_Texture;
		mFloorRenderMethod = Floor_Texture;
		mCeilingRenderMethod = Ceiling_Texture;
		
		// these distances will never change, so store them once to remove computation and division later.
		for (int i = 0; i < height_; ++i){
			mHeightMap.push_back(mGfxHeight / (2.0 * i - mGfxHeight));
		}
		
		mZBuffer.clear();
		for (unsigned int i = 0; i < mGfxWidth; ++i){
			mZBuffer.push_back(0);
		}
		
		mFloorcastingPixels = new sf::Uint8[mGfxWidth * mGfxHeight * 4];
		mFloorcastingTexture.create(mGfxWidth, mGfxHeight);
		mFloorcastingSprite.setTexture(mFloorcastingTexture);
		
		
		mMaxObjectRenderDistance = 1000.0f;
		
		mCeilingRenderColor = sf::Color::Black;
		mFloorRenderColor = sf::Color::Black;
		mWallRenderColor = sf::Color::Blue;
    }
    
    void Raycaster::setMap(sfray::Map &map){
        mMap = map;
    }
	
	void Raycaster::drawForCamera(sf::RenderWindow& window, sfray::Camera& camera){
		sf::View view = sf::View(sf::FloatRect(0,0,mGfxWidth,mGfxHeight));
        
		window.setView(view);
		
		drawForCamera(window, camera, view);
		
		window.setView(window.getDefaultView());
	}
    
    void Raycaster::drawForCamera(sf::RenderWindow &window, sfray::Camera &camera, sf::View& view){
        
		int times = 0;
	
		// update background
		if (mFloorRenderMethod == Floor_Texture || mCeilingRenderMethod == Ceiling_Texture){
			
			for (unsigned int i = 3; i < mGfxWidth * mGfxHeight * 4; i += 4){
				mFloorcastingPixels[i] = 0;
			}	
		}
		if (mFloorRenderMethod != Floor_Texture) {
			sf::RectangleShape floorshape;
			floorshape.setFillColor(mFloorRenderColor);
			floorshape.setSize(sf::Vector2f(mGfxWidth, mGfxHeight / 2));
			floorshape.setPosition(0, mGfxHeight / 2);
			window.draw(floorshape);							
		}
		if (mCeilingRenderMethod != Ceiling_Texture){
			sf::RectangleShape ceilshape;
			ceilshape.setFillColor(mCeilingRenderColor);
			ceilshape.setSize(sf::Vector2f(mGfxWidth, mGfxHeight / 2));
			ceilshape.setPosition(0, 0);
			window.draw(ceilshape);
		}
		
		for (unsigned int x = 0; x < mGfxWidth; ++x){
			float cameraX = 2 * x / float(mGfxWidth) - 1;
			
			float rayPosX = camera.getPosition().x;
			float rayPosY = camera.getPosition().y;
			
			float rayDirX = camera.getDirection().x + camera.getPlane().x * cameraX;
			float rayDirY = camera.getDirection().y + camera.getPlane().y * cameraX;
			
			//which box of the map we're in
			int mapX = int(rayPosX);
			int mapY = int(rayPosY);
			
			// length of ray from current position to next x or y-side
			float sideDistX = 0.0;
			float sideDistY = 0.0;
			
			// length of ray from one x or y-side to next x or y-side
			float deltaDistX = sqrtf(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			float deltaDistY = sqrtf(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
			float perpWallDist;
			
			// what direction to step in x or y-direction (either +1 or -1)
			int stepX = 0;
			int stepY = 0;
			
			int hit = 0; // was therre a wall hit?
			int side;
			
			if (rayDirX < 0){
				stepX = -1;
				sideDistX = (rayPosX - mapX) * deltaDistX;
			}
			else{
				stepX = 1;
				sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
			}
			
			if (rayDirY < 0){
				stepY = -1;
				sideDistY = (rayPosY - mapY) * deltaDistY;
			}
			else{
				stepY = 1;
				sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
			}
			
			// perform DDA
			while (hit == 0){
				// jump to next map square, OR in x-direction OR in y-direction
				if (sideDistX < sideDistY){
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				
				// check if ray has hit a wall
				if (mMap.GetTile(mapX, mapY).IsWall){
					hit = 1;
				}
			}
			
			// calculate distance projected on camera direction
			if (side == 0){
				perpWallDist = fabs((float(mapX) - rayPosX + float(1 - stepX) / 2.0) / rayDirX);
			}
			else{
				perpWallDist = fabs((float(mapY) - rayPosY + float(1 - stepY) / 2.0) / rayDirY);
			}
			
			// calculate height of line to draw on screen
			int lineHeight = abs(int(mGfxHeight / perpWallDist));
			
			// calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + mGfxHeight / 2;
			int drawEnd = lineHeight / 2 + mGfxHeight / 2;
			
			// texturing calculations
			int texNum = mMap.GetTile(mapX, mapY).TextureIndex;
			
			// calculate value of wallX
			float wallX;
			if (side == 1){
				wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
			}
			else{
				wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
			}
			wallX -= floor((wallX));
			
			// x coordinate on the texture
			int TEXTURE_WIDTH = mMap.GetTexture(texNum).getSize().x;
			int TEXTURE_HEIGHT = mMap.GetTexture(texNum).getSize().y;
			
			int texX = int(wallX * float(TEXTURE_WIDTH));
			if (side == 0 && rayDirX > 0){
				texX = TEXTURE_WIDTH - texX - 1;
			}
			if (side == 1 && rayDirY < 0)
			{
				texX = TEXTURE_WIDTH - texX - 1;
			}
			
			if (mWallRenderMethod == Wall_Texture){

				sf::VertexArray slice(sf::Quads, 4);
				slice[0].position = sf::Vector2f(x, drawStart);
				slice[1].position = sf::Vector2f(x+1, drawStart);
				slice[2].position = sf::Vector2f(x+1, drawEnd);
				slice[3].position = sf::Vector2f(x, drawEnd);
				
				slice[0].texCoords = sf::Vector2f(texX, 0);
				slice[1].texCoords = sf::Vector2f(texX + 1, 0);
				slice[2].texCoords = sf::Vector2f(texX + 1, TEXTURE_HEIGHT);
				slice[3].texCoords = sf::Vector2f(texX, TEXTURE_HEIGHT);
				
				window.draw(slice, &mMap.GetTexture(texNum));
			}
			else if (mWallRenderMethod == Wall_Color){
				sf::VertexArray slice(sf::Quads, 4);
				slice[0].position = sf::Vector2f(x, drawStart);
				slice[1].position = sf::Vector2f(x+1, drawStart);
				slice[2].position = sf::Vector2f(x+1, drawEnd);
				slice[3].position = sf::Vector2f(x, drawEnd);
				
				sf::Color wallColor = mWallRenderColor;
				
				if (side == 1){
					wallColor.r = wallColor.r / 2;
					wallColor.g = wallColor.g / 2;
					wallColor.b = wallColor.b / 2;
				}
				
				slice[0].color = wallColor;
				slice[1].color = wallColor;
				slice[2].color = wallColor;
				slice[3].color = wallColor;
				
				window.draw(slice);
			}
			
			
			// SET THE ZBUFFER FOR THE SPRITE CASTING
			mZBuffer[x] = perpWallDist; // perpendicular distance is used
			
			++times;
			// FLOOR casting
			float floorXWall;
			float floorYWall;
			
			// 4 different wall directions possible
			if (side == 0 && rayDirX > 0){
				floorXWall = mapX;
				floorYWall = mapY + wallX;
			}
			else if (side == 0 && rayDirX < 0){
				floorXWall = mapX + 1.0;
				floorYWall = mapY + wallX;
			}
			else if (side == 1 && rayDirY > 0){
				floorXWall = mapX + wallX;
				floorYWall = mapY;
			}
			else{
				floorXWall = mapX + wallX;
				floorYWall = mapY + 1.0;
			}
			
			float distWall;
			float distPlayer;
			float currentDist;
			
			distWall = perpWallDist;
			distPlayer = 0.0;
			
			if (drawEnd < 0){
				drawEnd = mGfxHeight;
			}
			
			
			if ((mFloorRenderMethod == Floor_Texture || mCeilingRenderMethod == Ceiling_Texture) && camera.moved == true)
			{
				for (unsigned int y = drawEnd + 1; y < mGfxHeight; ++y){						
					currentDist = mHeightMap[y];						
					
					float weight = (currentDist - distPlayer) / (distWall - distPlayer);
					
					float currentFloorX = weight * floorXWall + (1.0 - weight) * camera.getPosition().x;
					float currentFloorY = weight * floorYWall + (1.0 - weight) * camera.getPosition().y;
					
					sfray::MapTile tile = mMap.GetTile(int(currentFloorX), int(currentFloorY));
					
					int floorTextureWidth = tile.TextureWidth;
					int floorTextureHeight = tile.TextureHeight;
					
					int floorTexX = int(currentFloorX * floorTextureWidth) % floorTextureWidth;
					int floorTexY = int(currentFloorY * floorTextureHeight) % floorTextureHeight;
														
					sf::Color pix = mMap.GetPixelFromTexture(tile.TextureIndex, floorTexX, floorTexY);
					
					if (mFloorRenderMethod == Floor_Texture){
						unsigned int index = (y * (mGfxWidth * 4)) + (x*4);
						mFloorcastingPixels[index] = pix.r;
						mFloorcastingPixels[index+1] = pix.g;
						mFloorcastingPixels[index+2] = pix.b;
						mFloorcastingPixels[index+3] = pix.a;
					}
					
					if (mCeilingRenderMethod == Ceiling_Texture){
						unsigned int index2 = ((mGfxHeight-y) * (mGfxWidth * 4)) + (x*4);
						mFloorcastingPixels[index2] = pix.r / 2;
						mFloorcastingPixels[index2+1] = pix.g / 2;
						mFloorcastingPixels[index2+2] = pix.b / 2;
						mFloorcastingPixels[index2+3] = pix.a;
					}
				}	
				
			}
		}
		if (camera.moved && (mFloorRenderMethod == Floor_Texture || mCeilingRenderMethod == Ceiling_Texture)){
			mFloorcastingTexture.update(mFloorcastingPixels);	
		}
		if (mFloorRenderMethod == Floor_Texture || mCeilingRenderMethod == Ceiling_Texture){
			window.draw(mFloorcastingSprite);	
		}
		
		// SPRITE CASTING
		if (mEntityRenderMethod != Entity_None){
			
			// sort sprites from far to close
			std::vector<sfray::Entity*> sprites = mMap.getEntities();
			std::vector<int> mSpriteOrder;
			std::vector<float> mSpriteDistance;
			for (unsigned int i = 0; i < sprites.size(); ++i){
				float dist = ((camera.getPosition().x - sprites[i]->getPosition().x) * (camera.getPosition().x - sprites[i]->getPosition().x) + (camera.getPosition().y - sprites[i]->getPosition().y) * (camera.getPosition().y - sprites[i]->getPosition().y));
				
				if (dist > mMaxObjectRenderDistance){
					continue;
				}
				mSpriteOrder.push_back(i);
				mSpriteDistance.push_back(dist); // sqrt not taken, unneeded
			}
			combSort(mSpriteOrder, mSpriteDistance, mSpriteOrder.size());
			
			// after sorting the sprites, do the projection and draw them
			for (unsigned int i = 0; i < mSpriteOrder.size(); ++i){
				// translate the sprite position to relative to camera
				float spriteX = sprites[mSpriteOrder[i]]->getPosition().x - camera.getPosition().x;
				float spriteY = sprites[mSpriteOrder[i]]->getPosition().y - camera.getPosition().y;					
				
				// transform sprite with the inverse camera matrix
				float invDet = 1.0 / (camera.getPlane().x * camera.getDirection().y - camera.getDirection().x * camera.getPlane().y); // require for correct matrix multiplication.
				
				float transformX = invDet * (camera.getDirection().y * spriteX - camera.getDirection().x * spriteY);
				float transformY = invDet * (-camera.getPlane().y * spriteX + camera.getPlane().x * spriteY); // this is actually the depth inside the screen, that what Z is in the 3D
				
				// we can't see this object because of rotation
				if (transformY < 0){
					continue;
				}
				
				int spriteScreenX = int(( mGfxWidth / 2) * (1 + transformX / transformY));
				
				// calculate height of the sprite on screen
				int spriteHeight = abs(int(mGfxHeight / (transformY))); // using <transformY> instead of the real distance prevents fisheye
				// calculate lowest and highest pixel to fill in current stripe
				int drawStartY = -spriteHeight / 2 + mGfxHeight / 2;
				int drawEndY = spriteHeight / 2 + mGfxHeight / 2;
				
				// calculate width of the sprite
				int spriteWidth = abs(int(mGfxHeight / (transformY)));
				int drawStartX = -spriteWidth / 2 + spriteScreenX;
				int drawEndX = spriteWidth / 2 + spriteScreenX;

				int drawWidth = drawEndX - drawStartX;
				int drawOrigStartX = drawStartX;
				
				// draws vertical slices, iterating through x.
				sf::Texture spriteTexture = mMap.GetTexture(sprites[mSpriteOrder[i]]->getTextureUID());
				int TEXTURE_WIDTH = spriteTexture.getSize().x;
				int TEXTURE_HEIGHT = spriteTexture.getSize().y;
				
				int spriteLeft = 0;
				int spriteRight = TEXTURE_WIDTH;
				int spriteTop = 0;
				int spriteBottom = TEXTURE_HEIGHT;
				
				int width = (int)mGfxWidth;
				
				if (drawStartX > width || drawEndX < 0){
					continue;
				}
				
				for (int stripe = drawStartX; stripe <= drawEndX; ++stripe){				
					if (stripe < 0){
						drawStartX += 1;
						continue;
					}
					// are other walls in front
					if (transformY > mZBuffer[stripe]){
						drawStartX += 1;
						continue;
					}
					
					break;
				}
				
				int newWidth = drawEndX - drawStartX;
				
				float d = (float)drawWidth / (float)newWidth;
				
				spriteLeft = TEXTURE_WIDTH - (float)TEXTURE_WIDTH / d;
				
				for (int stripe = drawEndX; stripe > drawStartX; --stripe){
					if (stripe > width){
						drawEndX -= 1;
						continue;
					}
					
					if (transformY > mZBuffer[stripe]){
						drawEndX -= 1;
						continue;
					}
					
					break;
				}
				
				newWidth = drawEndX - drawOrigStartX;
				d = (float)drawWidth / (float)newWidth;
				
				spriteRight = (float)TEXTURE_WIDTH / d;
				
				sf::VertexArray spriteQuad(sf::Quads, 4);
				
				spriteQuad[0].position = sf::Vector2f(drawStartX, drawStartY);
				spriteQuad[1].position = sf::Vector2f(drawEndX, drawStartY);
				spriteQuad[2].position = sf::Vector2f(drawEndX, drawEndY);
				spriteQuad[3].position = sf::Vector2f(drawStartX, drawEndY);
				
				spriteQuad[0].texCoords = sf::Vector2f(spriteLeft, spriteTop);
				spriteQuad[1].texCoords = sf::Vector2f(spriteRight, spriteTop);
				spriteQuad[2].texCoords = sf::Vector2f(spriteRight, spriteBottom);
				spriteQuad[3].texCoords = sf::Vector2f(spriteLeft, spriteBottom);
				
				window.draw(spriteQuad, &spriteTexture);
				
	//			for (int stripe = drawStartX; stripe < drawEndX; stripe++){
	//				
	//				int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXTURE_WIDTH / spriteWidth) / 256;
	//				if (transformY > 0 && stripe > 0 && stripe < mGfxHeight && transformY < mZBuffer[stripe]){
	//					sf::Sprite slice = sf::Sprite(spriteTexture,
	//												  sf::IntRect(texX,0,1,TEXTURE_HEIGHT));
	//					float sc = float(drawEndY - drawStartY) / TEXTURE_HEIGHT;
	//					slice.setScale(1, sc);
	//					slice.setPosition(stripe, drawStartY);
	//					window.draw(slice);
	//				}
	//			}

			}
		}
    }
    
	WallRenderMethod Raycaster::getWallRenderMethod(){
		return mWallRenderMethod;
	}
	
	void Raycaster::setWallRenderMethod(WallRenderMethod method){
		mWallRenderMethod = method;
	}
	
	FloorRenderMethod Raycaster::getFloorRenderMethod(){
		return mFloorRenderMethod;
	}
	
	void Raycaster::setFloorRenderMethod(FloorRenderMethod method){
		mFloorRenderMethod = method;
	}	
	
	CeilingRenderMethod Raycaster::getCeilingRenderMethod(){
		return mCeilingRenderMethod;
	}
	
	void Raycaster::setCeilingRenderMethod(CeilingRenderMethod method){
		mCeilingRenderMethod = method;
	}
	
	EntityRenderMethod Raycaster::getEntityRenderMethod(){
		return mEntityRenderMethod;
	}
	
	void Raycaster::setEntityRenderMethod(EntityRenderMethod method){
		mEntityRenderMethod = method;
	}
	
	void Raycaster::updateForSize(){
		// distances
		mHeightMap.clear();
		for (unsigned int i = 0; i < mGfxHeight; ++i){
			mHeightMap.push_back(mGfxHeight / (2.0 * i - mGfxHeight));
		}
		
		if (mFloorcastingPixels != nullptr){			
			delete mFloorcastingPixels;
			mFloorcastingPixels = nullptr;
		}
		
		mFloorcastingPixels = new sf::Uint8[mGfxWidth * mGfxHeight * 4];
		mFloorcastingTexture.create(mGfxWidth, mGfxHeight);		
		mFloorcastingSprite = sf::Sprite(mFloorcastingTexture);
		
		mFloorcastingSprite.setPosition(0,0);
		
		mZBuffer.clear();
		for (unsigned int i = 0; i < mGfxWidth; ++i){
			mZBuffer.push_back(0);
		}
	}
	
	void Raycaster::setWidth(unsigned int amount){
		mGfxWidth = amount;
		updateForSize();
	}
	
	void Raycaster::setHeight(unsigned int amount){
		mGfxHeight = amount;
		updateForSize();
	}
	
	void Raycaster::setSize(unsigned int width_, unsigned int height_){
		mGfxWidth = width_;
		mGfxHeight = height_;
		updateForSize();
	}
	
	// sorting algorithm
	void Raycaster::combSort(std::vector<int>& order, std::vector<float>& dist, int amount){
		//std::cout << "start sort" << std::endl;
		
		int gap = amount;
		bool swapped = false;
		while (gap > 1 || swapped){
			// shrink factor 1.3
			gap = (gap * 10) / 13;
			if (gap == 9 || gap == 10){
				gap = 11;
			}
			if (gap < 1){
				gap = 1;
			}
			swapped = false;
			for (int i = 0; i < amount - gap; ++i){
				int j = i + gap;
				if (dist[i] < dist[j]){
					std::swap(dist[i],dist[j]);
					std::swap(order[i],order[j]);
					swapped = true;
				}
			}
		}
		
		//std::cout << "end sort" << std::endl;
	}
	
	float Raycaster::getMaxObjectRenderDistance(){
		return mMaxObjectRenderDistance;
	}
	
	void Raycaster::setMaxObjectRenderDistance(float distance){
		mMaxObjectRenderDistance = distance;
	}
	
	void Raycaster::setWallRenderColor(const sf::Color& color){
		mWallRenderColor = color;
	}
	
	void Raycaster::setCeilingRenderColor(const sf::Color& color){
		mCeilingRenderColor = color;
	}
	
	void Raycaster::setFloorRenderColor(const sf::Color& color){
		mFloorRenderColor = color;
	}
	
}// end of NS

