#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera() : zoomLevel(4), maxZoomLevel(10), minZoomLevel(0) {}
	Camera(int projectionWidth, int projectionHeight);
	~Camera();

	void SetPosition(const sf::Vector2f& target);
	void Pan(float x, float y);
	void ZoomIn();
	void ZoomOut();

	sf::View viewport;
private:
	sf::Vector2f position;

	unsigned char zoomLevel;
	unsigned char maxZoomLevel;
	unsigned char minZoomLevel;
};

