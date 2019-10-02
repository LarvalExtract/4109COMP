#include "Layer.h"



Layer::Layer(const std::string &textureFilePath, sf::Vector2u windowSize, float parallaxFactor, sf::Vector2f scroll) :
	verts(sf::Quads, 4),
	screenWidth(windowSize.x),
	screenHeight(windowSize.y),
	parallaxFactor(parallaxFactor),
	scrollSpeed(scroll)
{
	if (!texture.loadFromFile(textureFilePath))
		return;

	this->texture.setRepeated(true);

	int halfWidth = windowSize.x / 2;
	int halfHeight = windowSize.y / 2;

	verts[0].position = sf::Vector2f(-halfWidth, -halfHeight);
	verts[1].position = sf::Vector2f(-halfWidth, halfHeight);
	verts[2].position = sf::Vector2f(halfWidth, halfHeight);
	verts[3].position = sf::Vector2f(halfWidth, -halfHeight);

	verts[0].texCoords = verts[0].position;
	verts[1].texCoords = verts[1].position;
	verts[2].texCoords = verts[2].position;
	verts[3].texCoords = verts[3].position;
}


Layer::~Layer()
{
}

void Layer::FollowCamera(Camera& camera)
{
	sf::Vector2f camWindow = camera.viewport.getSize();

	float halfWidth = camWindow.x * 0.5f;
	float halfHeight = camWindow.y * 0.5f;

	sf::Vector2f camPosition = camera.viewport.getCenter();

	// Move background image
	verts[0].position.x = camPosition.x - halfWidth;
	verts[0].position.y = camPosition.y - halfHeight;
	verts[1].position.x = camPosition.x - halfWidth;
	verts[1].position.y = camPosition.y + halfHeight;
	verts[2].position.x = camPosition.x + halfWidth;
	verts[2].position.y = camPosition.y + halfHeight;
	verts[3].position.x = camPosition.x + halfWidth;
	verts[3].position.y = camPosition.y - halfHeight;

	// Scroll background texture coordinates
	verts[0].texCoords.x = (parallaxFactor * camPosition.x) - halfWidth;
	verts[0].texCoords.y = (parallaxFactor * camPosition.y) - halfHeight;
	verts[1].texCoords.x = (parallaxFactor * camPosition.x) - halfWidth;
	verts[1].texCoords.y = (parallaxFactor * camPosition.y) + halfHeight;
	verts[2].texCoords.x = (parallaxFactor * camPosition.x) + halfWidth;
	verts[2].texCoords.y = (parallaxFactor * camPosition.y) + halfHeight;
	verts[3].texCoords.x = (parallaxFactor * camPosition.x) + halfWidth;
	verts[3].texCoords.y = (parallaxFactor * camPosition.y) - halfHeight;
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &this->texture;
	target.draw(verts, states);
}