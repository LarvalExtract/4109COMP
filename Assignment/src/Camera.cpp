#include "Camera.h"



Camera::Camera(int projectionWidth, int projectionHeight) :
	viewport(sf::FloatRect(0, 0, projectionWidth, projectionHeight)),
	zoomLevel(4),
	minZoomLevel(2),
	maxZoomLevel(10)
{
	
}

Camera::~Camera()
{
}

//sf::FloatRect Camera::GetCameraWindow(float parallaxFactor)
//{
//	sf::FloatRect view;
//
//	view.left = position.x - viewportWidth * 0.5f * zoom * parallaxFactor;
//	view.top = position.y - viewportHeight * 0.5f * zoom * parallaxFactor;
//
//	view.width = viewportWidth * zoom * parallaxFactor;
//	view.height = viewportHeight * zoom * parallaxFactor;
//
//	return view;
//}

void Camera::SetPosition(const sf::Vector2f& position)
{
	viewport.setCenter(position);
}

void Camera::Pan(float x, float y)
{
	viewport.move(x, y);
}

void Camera::ZoomIn()
{
	if (zoomLevel > minZoomLevel)
	{
		viewport.zoom(0.8f);
		zoomLevel--;
	}
}

void Camera::ZoomOut()
{
	if (zoomLevel < maxZoomLevel)
	{
		viewport.zoom(1.25f);
		zoomLevel++;
	}
}
