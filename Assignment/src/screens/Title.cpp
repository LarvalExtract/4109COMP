#include "Title.h"
#include "../Application.h"

#include <string>
#include <sstream>
#include <fstream>

Title::Title() :
	storyText(3),
	bFinished(false),
	fontColor(255, 0, 0, 255),
	textIndex(0)
{
}


Title::~Title()
{
}

bool Title::Initialise(int screenWidth, int screenHeight)
{
	if (!font.loadFromFile("Media/Fonts/SegoeMarker.ttf"))
		return false;

	if (!bkTexture.loadFromFile("Media/Textures/title_screen.jpg"))
		return false;

	background.setTexture(bkTexture);

	float scaleX = static_cast<float>(screenWidth) / static_cast<float>(bkTexture.getSize().x);
	float scaleY = static_cast<float>(screenHeight) / static_cast<float>(bkTexture.getSize().y);
	background.scale(scaleX, scaleY);

	float halfWidth = screenWidth / 2;
	float halfHeight = screenHeight / 2;

	storyText[0].setString("at first there was nothing");
	storyText[1].setString("but then...");
	storyText[2].setString("there were ZOMBIES");

	for (sf::Text& text : storyText)
	{
		text.setFont(font);
		text.setFillColor(fontColor);
		text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.5f);
		text.setPosition(halfWidth, halfHeight);
	}
}

void Title::Update(float deltaTime, SFML_Application* app)
{
	static float accumulatedTime = 0.0f;
	static bool fadeIn = true;

	if (accumulatedTime > 3.0f)
	{
		textIndex++;
		accumulatedTime = 0.0f;

		if (textIndex >= storyText.size())
			bFinished = true;
	}
	else
	{
		accumulatedTime += deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		bFinished = true;
}

void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!bFinished)
	{
		target.draw(background);
		target.draw(storyText[textIndex]);
	}
}