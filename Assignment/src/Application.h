#pragma once

#include <SFML/Graphics.hpp>

#include "screens/MainMenu.h"
#include "screens/Credits.h"
#include "screens/GameWorld.h"
#include "screens/Title.h"
#include "screens/Help.h"

class SFML_Application : public sf::NonCopyable
{
	// Game states
public:
	enum class AppState : unsigned char
	{
		STORY_SCREEN,
		MAIN_MENU,
		IN_GAME,
		HELP,
		PAUSED,
		CREDITS
	} appState;

	SFML_Application(int screenWidth, int screenHeight, const sf::String& windowTitle = "4109COMP Assignment", bool fullScreen = false);
	~SFML_Application();

	void Start();

	bool bQuit;

private:
	bool bInitialised;
	bool bShowFPS;
	bool bWatchedStory;

	float updateRate;

	bool Initialise();
	void Update(float deltaTime);
	void Render();

private:
	sf::RenderWindow window;
	sf::Event event, lastEvent;
	sf::Clock clock;
	sf::Text fpsDisplay;
	sf::Font defaultFont;

	MainMenu* mainMenuLayer;
	Credits* creditsLayer;
	GameWorld* gameWorldLayer;
	Title* titleScreen;
	Help* helpScreen;

public:
	inline sf::Vector2f ScreenCenter() const { return sf::Vector2f(static_cast<float>(window.getSize().x) * 0.5f, static_cast<float>(window.getSize().y) * 0.5f); }
	inline sf::Vector2i GetCursorPosition() const { return sf::Mouse::getPosition(window); }
	inline void ShowMouseCursor(bool visible) { window.setMouseCursorVisible(visible); }
	inline void SetCursorPosition(sf::Vector2i position) { sf::Mouse::setPosition(position); }
	inline bool StoryScreenFinished() const { return bWatchedStory; }

	void ResetGameState() { gameWorldLayer->ResetGame(); }
	void ResetCredits() { creditsLayer->Reset(); }

private:
	void ProcessInputs();

	void UpdateTitleScreen(float deltaTime);
	void UpdateMainMenu(float deltaTime);
	void UpdateGame(float deltaTime);
	void UpdateCredits(float deltaTime);
	void UpdateHelpScreen(float deltaTime);

	void UpdateFPSOverlay(sf::Time deltaTime);

	void RenderTitleScreen();
	void RenderMainMenu();
	void RenderGame();
	void RenderCredits();
};