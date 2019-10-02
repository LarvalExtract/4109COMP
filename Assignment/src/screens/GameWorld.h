#pragma once

#include "Screen.h"
#include "Camera.h"
#include "Menu.h"
#include "../Layer.h"
#include "../AnimatedSprite.h"
#include "../game/Player.h"
#include "../game/Zombie.h"
#include "../game/Bullet.h"
#include "../game/HUD.h"

class GameWorld : public Screen
{
public:
	GameWorld();
	~GameWorld();

	bool Initialise(int screenWidth, int screenHeight) override;
	void Update(float deltaTime, SFML_Application* app) override;
	void ResetGame();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Camera camera;

	void ZoomIn();
	void ZoomOut();

private:
	bool bPaused;
	Menu pauseMenu;
	Player player;
	HUD hud;
	std::vector<Zombie> zombies;
	std::vector<Bullet> bullets;

private:
	std::vector<Layer*> worldLayers;

	sf::Texture crosshairTexture;
	sf::Sprite crosshair;

	sf::Texture flashlightTexture;
	sf::Sprite flashlight;

	float blend;
	sf::Color nightColour;
	sf::RectangleShape nightOverlay;
	sf::RenderTexture flashlightOverlay;
	sf::Sprite flashlightSprite;

	int screenWidth, screenHeight;

private:
	void UpdateGameplay(float deltaTime, SFML_Application* app);
	void UpdatePauseMenu(SFML_Application* app);
	bool PauseGame(SFML_Application* app);

	void UpdateBullets(float deltaTime);
	void UpdateTimeOfDay(float deltaTime);

	void SpawnZombie(Zombie& zombie);

	Bullet& GetFirstInactiveBullet();
};

