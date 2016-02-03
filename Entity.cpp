//
//  Entity.cpp
//  SFML Raycast Engine
//
//  Created by Daniel Alexander on 12/9/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//
#include "Entity.hpp"

namespace sfray
{

Entity::Entity()
:mPosition(0, 0)
{
}

Entity::~Entity()
{
}

sf::Vector2f Entity::getPosition(){
	return mPosition;
}

void Entity::setPosition(sf::Vector2f position){
	mPosition = position;
}

void Entity::setPosition(float x, float y){
	mPosition = sf::Vector2f(x, y);
}

int Entity::getTextureUID(){
	return mTextureUID;
}

void Entity::setTextureUID(int uid){
	mTextureUID = uid;
}

}

