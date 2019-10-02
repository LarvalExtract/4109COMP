#include "Application.h"

SFML_Application::SFML_Application(int screenWidth, int screenHeight, const sf::String &windowTitle, bool fullScreen) :
	bInitialised(false),
	bQuit(false),
	bShowFPS(false),
	bWatchedStory(false),
	updateRate(1.0f / 60.0f),
	appState(AppState::MAIN_MENU)
{
	if (fullScreen == true)
		window.create(sf::VideoMode(screenWidth, screenHeight), windowTitle, sf::Style::Fullscreen);
	else
		window.create(sf::VideoMode(screenWidth, screenHeight), windowTitle);
}

SFML_Application::~SFML_Application()
{
	delete mainMenuLayer;
	delete creditsLayer;
	delete gameWorldLayer;
	delete titleScreen;
	delete helpScreen;
}

void SFML_Application::Start()
{
	static float accumulatedTime = 0.0f;

	if (!Initialise())
		return;

	sf::Time newTime;
	sf::Time currentTime = clock.getElapsedTime();
	sf::Time deltaTime;

	while (!bQuit)
	{
		newTime = clock.getElapsedTime();
		deltaTime = newTime - currentTime;
		currentTime = newTime;

		bool validPoll = window.pollEvent(event);

		if ((event.type == sf::Event::Closed) && validPoll)
			bQuit = true;

		ProcessInputs();

		accumulatedTime += deltaTime.asSeconds();

		if (accumulatedTime > updateRate)
		{
			Update(updateRate);
			Render();
			accumulatedTime = 0.0f;
		}

		if (bShowFPS == true)
			UpdateFPSOverlay(deltaTime);


		lastEvent = event;
	}

	window.close();
}

bool SFML_Application::Initialise()
{
	defaultFont.loadFromFile("Media/Fonts/SegoeMarker.ttf");

	mainMenuLayer = new MainMenu();
	creditsLayer = new Credits();
	gameWorldLayer = new GameWorld();
	titleScreen = new Title();
	helpScreen = new Help();

	mainMenuLayer->Initialise(window.getSize().x, window.getSize().y);
	creditsLayer->Initialise(window.getSize().x, window.getSize().y);
	gameWorldLayer->Initialise(window.getSize().x, window.getSize().y);
	titleScreen->Initialise(window.getSize().x, window.getSize().y);
	helpScreen->Initialise(window.getSize().x, window.getSize().y);

	fpsDisplay.setPosition(5.0f, 5.0f);
	fpsDisplay.setFont(defaultFont);
	fpsDisplay.setCharacterSize(15);

	

	return bInitialised = true;
}

void SFML_Application::Update(float deltaTime)
{
	switch (appState)
	{
	case AppState::STORY_SCREEN:
		UpdateTitleScreen(deltaTime);
		break;

	case AppState::MAIN_MENU:
		UpdateMainMenu(deltaTime);
		break;

	case AppState::IN_GAME:
		UpdateGame(deltaTime);
		break;

	case AppState::HELP:
		UpdateHelpScreen(deltaTime);
		break;

	case AppState::CREDITS:
		UpdateCredits(deltaTime);
		break;
	}

	
}

void SFML_Application::Render()
{
	window.clear();

	switch (appState)
	{
	case AppState::STORY_SCREEN:
		window.draw(*titleScreen);
		break;

	case AppState::MAIN_MENU:
		window.draw(*mainMenuLayer);
		break;

	case AppState::IN_GAME:
		RenderGame();
		break;

	case AppState::HELP:
		window.draw(*helpScreen);
		break;

	case AppState::CREDITS:
		window.draw(*creditsLayer);
		break;
	}

	if (bShowFPS == true)
		window.draw(fpsDisplay);

	window.display();
}

void SFML_Application::ProcessInputs()
{
	if (event.type == sf::Event::MouseWheelScrolled && lastEvent.type != sf::Event::MouseWheelScrolled)
	{
		if (event.mouseWheelScroll.delta > 0.0f)
			gameWorldLayer->ZoomIn();
		else if (event.mouseWheelScroll.delta < 0.0f)
			gameWorldLayer->ZoomOut();
	}

	static bool bWasF1Pressed = false;
	static bool bIsF1Pressed = false;

	bIsF1Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::F1);

	if (bIsF1Pressed && !bWasF1Pressed)
		bShowFPS = !bShowFPS;

	bWasF1Pressed = bIsF1Pressed;
}

void SFML_Application::UpdateTitleScreen(float deltaTime)
{
	if (titleScreen->Finished())
	{
		bWatchedStory = true;
		appState = AppState::IN_GAME;
		ShowMouseCursor(false);
	}
	else
		titleScreen->Update(deltaTime, this);
}

void SFML_Application::UpdateMainMenu(float deltaTime)
{
	mainMenuLayer->Update(deltaTime, this);
}

void SFML_Application::UpdateGame(float deltaTime)
{
	gameWorldLayer->Update(deltaTime, this);
}

void SFML_Application::UpdateCredits(float deltaTime)
{
	creditsLayer->Update(deltaTime, this);
}

void SFML_Application::UpdateHelpScreen(float deltaTime)
{
	helpScreen->Update(deltaTime, this);
}

void SFML_Application::UpdateFPSOverlay(sf::Time deltaTime)
{
	fpsDisplay.setString(std::to_string(deltaTime.asSeconds() * 1000.0f) + "ms, " + std::to_string(1.0f / deltaTime.asSeconds()) + " fps");
}

void SFML_Application::RenderTitleScreen()
{
	window.draw(*titleScreen);
}

void SFML_Application::RenderMainMenu()
{
	window.draw(*mainMenuLayer);
}

void SFML_Application::RenderGame()
{
	window.draw(*gameWorldLayer);
}

void SFML_Application::RenderCredits()
{
	window.draw(*creditsLayer);
}
