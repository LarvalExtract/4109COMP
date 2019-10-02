#pragma once

#include "Screen.h"
#include "Menu.h"

class MainMenu : public Screen
{
public:
	MainMenu();
	~MainMenu();

	virtual bool Initialise(int screenWidth, int screenHeight) override;
	virtual void Update(float deltaTime, SFML_Application* app) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Menu menu;
	sf::Sprite bkSprite;
	sf::Texture bkTexture;
};

