#pragma once

#include <SFML/Graphics.hpp>

class SFML_Application;

class Screen : public sf::Drawable
{
public:
	~Screen() {}

	virtual bool Initialise(int screenWidth, int screenHeight) = 0;
	virtual void Update(float deltaTime, SFML_Application* app = nullptr) = 0;

protected:
	Screen() {}
};

