#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Camera.h"

class Layer : public sf::Drawable
{
public:
	Layer(const std::string &textureFilePath, sf::Vector2u windowSize, float parallaxFactor = 1.0f, sf::Vector2f scroll = sf::Vector2f(1.0f, 1.0f));
	~Layer();

	void FollowCamera(Camera& camera);
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Texture texture;
	sf::VertexArray verts;
	float parallaxFactor;
	sf::Vector2f scrollSpeed;

	float screenWidth;
	float screenHeight;
};

