#include "Camera.hpp"
#include <cmath>

#define PI 3.14

namespace sfray
{

Camera::Camera()
:mDirection(-1.0f, 0)
,mPlane(0, 0.66f)
,mPosition(0, 0)
{
}

Camera::~Camera()
{
}


sf::Vector2f Camera::getDirection(){
	return mDirection;
}

void Camera::setDirection(const sf::Vector2f& direction){
	mDirection = direction;
}

void Camera::setDirection(float x, float y){
	mDirection = sf::Vector2f(x, y);
}

sf::Vector2f Camera::getPlane(){
	return mPlane;
}

void Camera::setPlane(const sf::Vector2f& plane){
	mPlane = plane;
}

void Camera::setPlane(float x, float y){
	mPlane = sf::Vector2f(x, y);
}

sf::Vector2f Camera::getPosition(){
	return mPosition;
}

void Camera::setPosition(const sf::Vector2f& position){
	mPosition = position;
}

void Camera::setPosition(float x, float y){
	mPosition = sf::Vector2f(x, y);
}

float Camera::getDirectionInDegrees(){
	return atan2f(mDirection.y, mDirection.x) * (180.0f / PI);
}

void Camera::setDirectionInDegrees(float degrees){
	float angle = degrees * (PI / 180.0f);
	mDirection.x = cosf(angle);
	mDirection.y = sinf(angle);
	
	float perp = (degrees - 90) * (PI / 180.0f);
	//float fov = 2 * atan(0.66/1.0);
	mPlane.x = cosf(perp) * 0.66f;
	mPlane.y = sinf(perp) * 0.66f;
}

void Camera::rotateByDegrees(float degrees){
	float current = getDirectionInDegrees();
	current += degrees;
	setDirectionInDegrees(current);
}

}

