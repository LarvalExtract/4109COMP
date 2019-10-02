#include "GameWorld.h"

#include "Application.h"

constexpr float radians_to_degrees = 180.0f / 3.14159f;

GameWorld::GameWorld() :
	bPaused(false),
	pauseMenu("Media/MenuInfo/pause_menu.txt"),
	zombies(70),
	bullets(100),
	blend(0.0f),
	nightColour(0, 0, 0, 0)
{
}


GameWorld::~GameWorld()
{
	for (Layer* layer : worldLayers)
		delete layer;

	worldLayers.clear();
}

bool GameWorld::Initialise(int screenWidth, int screenHeight)
{
	if (!crosshairTexture.loadFromFile("Media/Textures/crosshair.png"))
		return false;

	if (!flashlightTexture.loadFromFile("Media/Textures/flashlight.png"))
		return false;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	flashlightOverlay.create(screenWidth, screenHeight);
	nightOverlay.setSize(sf::Vector2f(screenWidth, screenHeight));

	camera.viewport.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));

	player.setPosition(0, 0);

	hud.Initialise(screenWidth, screenHeight, player);

	crosshair.setTexture(crosshairTexture);
	crosshair.setOrigin(crosshairTexture.getSize().x / 2, crosshairTexture.getSize().y / 2);

	flashlight.setTexture(flashlightTexture);
	flashlight.setOrigin(flashlightTexture.getSize().x / 2, flashlightTexture.getSize().y / 2);

	pauseMenu.SetMenuItemAlignment(MenuAlignment::CENTER);
	pauseMenu.SetPosition((screenWidth / 2) - (pauseMenu.GetLocalBounds().width / 2), (screenHeight / 2) - (pauseMenu.GetLocalBounds().width / 2));

	time_t now;
	srand(time(&now));

	// Choose random level background
	std::string levelTextureName = "Media/Textures/levels/Ground_0x.png";
	levelTextureName[30] = (rand() % 4) + 49;

	worldLayers.push_back(new Layer(levelTextureName, sf::Vector2u(screenWidth, screenHeight), 1.0f));
	worldLayers.push_back(new Layer("Media/Textures/fog.png", sf::Vector2u(screenWidth, screenHeight), 0.6f, sf::Vector2f(5.0f, 0.0f)));

	return true;
}

void GameWorld::Update(float deltaTime, SFML_Application* app)
{
	bPaused = PauseGame(app);

	if (!bPaused)		
		UpdateGameplay(deltaTime, app);

	else
		UpdatePauseMenu(app);
}

void GameWorld::ResetGame()
{
	bPaused = false;

	for (Zombie& zombie : zombies)
	{
		zombie.SetActive(false);
		SpawnZombie(zombie);
	}

	for (Bullet& bullet : bullets)
		bullet.SetActive(false);

	player.Reset();
	player.setPosition(0, 0);
}

void GameWorld::UpdateGameplay(float deltaTime, SFML_Application* app)
{
	crosshair.setPosition(static_cast<sf::Vector2f>(app->GetCursorPosition()));

	player.Update(deltaTime);

	// Calculate vector from middle of screen to cursor position
	sf::Vector2f originToCursor = static_cast<sf::Vector2f>(app->GetCursorPosition()) - app->ScreenCenter();

	// Camera follows mouse
	camera.SetPosition(player.getPosition() + originToCursor * 0.2f);
	flashlight.setPosition(camera.viewport.getCenter());

	// Layers follow camera
	for (Layer* layer : worldLayers)
		layer->FollowCamera(camera);

	// Rotate player sprite towards mouse position
	sf::Vector2f playerToCamera = camera.viewport.getCenter() - player.getPosition();
	float angle = atan2(playerToCamera.y, playerToCamera.x) * radians_to_degrees;

	flashlight.setPosition(flashlightOverlay.getSize().x / 2, flashlightOverlay.getSize().y / 2);

	player.setRotation(angle);

	if (player.FiredWeapon())
		player.ShootBullet(GetFirstInactiveBullet());

	for (Zombie& zombie : zombies)
	{
		if (!zombie.IsActive())
		{
			SpawnZombie(zombie);
		}
		else
		{
			zombie.Update(deltaTime, player.getPosition());

			if (zombie.GetSprite().getGlobalBounds().intersects(player.GetSprite().getGlobalBounds()))
			{
				if (zombie.Attack())
				{
					player.Hurt(10.0f);
				}
				else if (player.Melee())
				{
					zombie.Hurt(25.0f);
				}
			}
		}
	}

	UpdateBullets(deltaTime);

	hud.Update(player);

	UpdateTimeOfDay(deltaTime);
}

void GameWorld::UpdatePauseMenu(SFML_Application* app)
{
	int selectedMenuIndex = pauseMenu.Update(app->GetCursorPosition());

	switch (selectedMenuIndex)
	{
	case 0:
		bPaused = false;
		app->ShowMouseCursor(false);
		break;
	case 1:
		app->appState = SFML_Application::AppState::MAIN_MENU;
		break;
	}
}

bool GameWorld::PauseGame(SFML_Application* app)
{
	static bool isEscPressed = false;
	static bool wasEscPressed = false;

	isEscPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

	if (isEscPressed && !wasEscPressed)
	{
		bPaused = !bPaused;

		app->ShowMouseCursor(bPaused);
	}

	wasEscPressed = isEscPressed;

	return bPaused;
}

void GameWorld::UpdateBullets(float deltaTime)
{
	for (Bullet& bullet : bullets)
	{
		if (!bullet.IsActive())
			continue;
		
		bullet.Update(deltaTime);

		for (Zombie& zombie : zombies)
		{
			if (zombie.IsActive())
			{
				if (bullet.getGlobalBounds().intersects(zombie.GetSprite().getGlobalBounds()))
				{
					bullet.SetActive(false);

					zombie.Hurt(bullet.GetDamage());
				}
			}
		}		
	}
}

void GameWorld::UpdateTimeOfDay(float deltaTime)
{
	// Update night overlay
	static float accumulatedTime = 0.0f;

	flashlightOverlay.clear();

	blend = sinf(accumulatedTime * 0.1f) * 100.0f;
	blend += 155.0f;

	nightColour.r = static_cast<unsigned char>(blend);
	nightColour.g = static_cast<unsigned char>(blend);
	nightColour.b = static_cast<unsigned char>(blend);
	nightColour.a = static_cast<unsigned char>(blend);

	nightOverlay.setFillColor(nightColour);

	flashlightOverlay.draw(nightOverlay);

	if (player.CurrentWeapon() == Player::Weapon::FLASHLIGHT)
		flashlightOverlay.draw(flashlight, sf::BlendAdd);

	flashlightSprite.setTexture(flashlightOverlay.getTexture());

	accumulatedTime += deltaTime;
}

void GameWorld::SpawnZombie(Zombie& zombie)
{
	if (zombie.IsActive())
		return;

	zombie.Reset();

	sf::Vector2f spawnPosition = player.getPosition();

	// Choose random spawn position
	float offsetX = static_cast<float>((rand() % 10000) - 5000);
	float offsetY = static_cast<float>((rand() % 10000) - 5000);

	spawnPosition.x += offsetX;
	spawnPosition.y += offsetY;

	zombie.setPosition(spawnPosition);

	zombie.SetActive(true);
}

Bullet& GameWorld::GetFirstInactiveBullet()
{
	for (Bullet& bullet : bullets)
	{
		if (!bullet.IsActive())
		{
			return bullet;
		}
	}

	bullets.push_back(Bullet());
	return bullets.back();
}

void GameWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(camera.viewport);

	// Draw game world
	target.draw(*worldLayers[0]);

	for (const Zombie& zombie : zombies)
		target.draw(zombie);

	target.draw(player);

	for (const Bullet& bullet : bullets)
		if (bullet.IsActive())
			target.draw(bullet);

	// Draw fog 
	target.draw(*worldLayers[1]);

	// Reset view to screen coordinates
	target.setView(target.getDefaultView());

	// Draw screen-space items
	target.draw(flashlightSprite, sf::BlendMultiply);
	target.draw(crosshair);
	target.draw(hud);

	if (bPaused)
		target.draw(pauseMenu);
}

void GameWorld::ZoomIn()
{
	camera.ZoomIn();
	
	for (Layer* layer : worldLayers)
		layer->FollowCamera(camera);

	flashlight.setPosition(crosshair.getPosition());
}

void GameWorld::ZoomOut()
{
	camera.ZoomOut();

	for (Layer* layer : worldLayers)
		layer->FollowCamera(camera);

	flashlight.setPosition(crosshair.getPosition());
}
