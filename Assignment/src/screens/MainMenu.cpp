#include "MainMenu.h"

#include "Application.h"

MainMenu::MainMenu() :
	Screen(),
	menu("Media/MenuInfo/main_menu.txt")
{
}


MainMenu::~MainMenu()
{
}

bool MainMenu::Initialise(int screenWidth, int screenHeight)
{
	bkTexture.loadFromFile("Media/Textures/main_menu_bg.jpg");
	bkSprite.setTexture(bkTexture);

	float scaleX = static_cast<float>(screenWidth) / static_cast<float>(bkTexture.getSize().x);
	float scaleY = static_cast<float>(screenHeight) / static_cast<float>(bkTexture.getSize().y);

	bkSprite.scale(scaleX, scaleY);

	menu.SetFontSize(40);
	menu.SetPosition(20.0f, screenHeight / 2);

	return true;
}

void MainMenu::Update(float deltaTime, SFML_Application* app)
{
	int selectedMenuIndex = menu.Update(app->GetCursorPosition());

	// Player selected a menu item
	switch (selectedMenuIndex)
	{
	case 0:
		if (app->StoryScreenFinished())
		{
			app->ShowMouseCursor(false);
			app->ResetGameState();
			app->appState = SFML_Application::AppState::IN_GAME;
		}
		else
		{
			app->appState = SFML_Application::AppState::STORY_SCREEN;
		}
		break;
	case 1: 
		app->appState = SFML_Application::AppState::HELP; 
		break;
	case 2: 
		app->appState = SFML_Application::AppState::CREDITS;
		app->ResetCredits();
		break;
	case 3: 
		app->bQuit = true; 
		break;
	}
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bkSprite);
	target.draw(menu);
}
