//
//  Entity.hpp
//  SFML Raycast Engine
//
//  Created by Daniel Alexander on 12/9/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//
#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <SFML/Graphics.hpp>

namespace sfray
{

class Entity
{
protected:
	sf::Vector2f	mPosition;
	int				mTextureUID;
	
public:
	Entity();
	~Entity();	

	sf::Vector2f		getPosition();
	void		 		setPosition(sf::Vector2f position);
	void				setPosition(float x, float y);
	
	int					getTextureUID();
	void				setTextureUID(int uid);
};

}

#endif // ENTITY_HPP
