#pragma once

#include "Screen.h"
#include "../Menu.h"

class Help : public Screen
{
public:
	Help();
	~Help();

	bool Initialise(int screenWidth, int screenHeight) override;
	void Update(float deltaTime, SFML_Application* app) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite background;
	sf::Texture bkTexture;

	Menu descriptions;
	Menu keys;
};

