#pragma once

#include "Screen.h"
#include "../Menu.h"

#include <vector>

class SFML_Application;

class Title : public Screen
{
public:
	Title();
	~Title();

	virtual bool Initialise(int screenWidth, int screenHeight) override;
	virtual void Update(float deltaTime, SFML_Application* app = nullptr) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline bool Finished() const { return bFinished; }

private:
	bool bFinished;

	sf::Font font;
	sf::Color fontColor;
	std::vector<sf::Text> storyText;
	int textIndex;

	sf::Sprite background;
	sf::Texture bkTexture;
};

