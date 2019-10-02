#include "Credits.h"
#include "Application.h"

#include <fstream>
#include <sstream>

Credits::Credits() :
	fontSize(20),
	scrollSpeed(-50.0f)
{
	font.loadFromFile("Media/Fonts/SegoeMarker.ttf");
}


Credits::~Credits()
{
}

bool Credits::Initialise(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	GetNamesFromFile("Media/MenuInfo/credits.txt");
	UpdatePositions();

	bkTexture.loadFromFile("Media/Textures/title-screen.png");
	background.setTexture(bkTexture);

	return true;
}

void Credits::Update(float deltaTime, SFML_Application* app)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		app->appState = SFML_Application::AppState::MAIN_MENU;
		return;
	}

	for (sf::Text& text : names)
	{
		text.move(0.0f, scrollSpeed * deltaTime);
	}
}

void Credits::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);

	for (const sf::Text& text : names)
	{
		target.draw(text);
	}
}

void Credits::Reset()
{
	UpdatePositions();
}

void Credits::GetNamesFromFile(const std::string& fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open())
		return;

	sf::Text text;

	std::string line;
	while (std::getline(file, line))
	{
		// Line is a job category
		if (line[0] == '[')
		{
			// Extract job role between square bracket
			text.setString(line.substr(1, line.size() - 2));

			text.setFont(font);
			text.setCharacterSize(32);
			text.setFillColor(sf::Color(255, 255, 0));
		}
		// Line is an employee name
		else
		{
			text.setString(line);

			text.setFont(font);
			text.setCharacterSize(24);
			text.setFillColor(sf::Color(255, 255, 255));
		}

		sf::FloatRect boundary;
		boundary = text.getLocalBounds();
		text.setOrigin(boundary.width * 0.5f, boundary.height * 0.5f);

		names.push_back(text);
	}
}

void Credits::UpdatePositions()
{
	float yPos = screenHeight;

	for (int i = 0; i < names.size(); i++)
	{
		if (names[i].getCharacterSize() == 32)
			yPos += 30.0f;

		names[i].setPosition(screenWidth / 2, yPos);

		yPos += names[i].getLocalBounds().height;	
	}
}
