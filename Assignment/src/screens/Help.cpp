#include "Help.h"

#include "../Application.h"

Help::Help() :
	descriptions("Media/MenuInfo/help_descriptions.txt"),
	keys("Media/MenuInfo/help_keys.txt")
{
}


Help::~Help()
{
}

bool Help::Initialise(int screenWidth, int screenHeight)
{
	if (!bkTexture.loadFromFile("Media/Textures/help_screen.jpg"))
		return false;

	background.setTexture(bkTexture);

	// Stretch background image to fit window
	float scaleX = static_cast<float>(screenWidth) / static_cast<float>(bkTexture.getSize().x);
	float scaleY = static_cast<float>(screenHeight) / static_cast<float>(bkTexture.getSize().y);

	background.scale(scaleX, scaleY);

	int halfWidth = screenWidth / 2;
	int halfHeight = screenHeight / 2;

	descriptions.SetFontSize(20);
	keys.SetFontSize(20);
	keys.SetFontColour(sf::Color::Yellow);

	descriptions.SetMenuItemAlignment(MenuAlignment::RIGHT);
	keys.SetMenuItemAlignment(MenuAlignment::LEFT);

	descriptions.SetPosition(halfWidth - descriptions.GetLocalBounds().width - 5, halfHeight - descriptions.GetLocalBounds().height / 2);
	keys.SetPosition(halfWidth + 5, descriptions.GetLocalBounds().top);
}

void Help::Update(float deltaTime, SFML_Application* app)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		app->appState = SFML_Application::AppState::MAIN_MENU;
}

void Help::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(descriptions);
	target.draw(keys);
}
