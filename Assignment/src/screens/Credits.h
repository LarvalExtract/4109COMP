#pragma once

#include "Screen.h"

#include <vector>

class Credits : public Screen
{
public:
	Credits();
	~Credits();

	virtual bool Initialise(int screenWidth, int screenHeight) override;
	virtual void Update(float deltaTime, SFML_Application* app) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Reset();

private:
	std::vector<sf::Text> names;
	sf::Font font;
	unsigned int fontSize;
	float scrollSpeed;

	int screenWidth;
	int screenHeight;

	void GetNamesFromFile(const std::string& fileName);
	void UpdatePositions();

	sf::Sprite background;
	sf::Texture bkTexture;
};

