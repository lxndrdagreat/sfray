#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

namespace sfray
{

class Camera
{
private:
	sf::Vector2f		mDirection;
	sf::Vector2f		mPlane;
	sf::Vector2f		mPosition;

public:
	Camera();
	~Camera();

	bool				moved;

	sf::Vector2f		getDirection();
	void 				setDirection(const sf::Vector2f& direction);
	void				setDirection(float x, float y);
	
	sf::Vector2f		getPlane();
	void				setPlane(const sf::Vector2f& plane);
	void				setPlane(float x, float y);
	
	sf::Vector2f		getPosition();
	void				setPosition(const sf::Vector2f& position);
	void				setPosition(float x, float y);
	
	void				setDirectionInDegrees(float degrees);
	float				getDirectionInDegrees();
	
	void				rotateByDegrees(float degrees);
};

}

#endif // CAMERA_HPP
