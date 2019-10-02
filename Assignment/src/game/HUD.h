#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"

class HUD : public sf::Drawable
{
public:
	HUD();
	~HUD();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Initialise(int screenWidth, int screenHeight, const Player& player);
	void Update(const Player& player);

private:
	bool bInitialised; 

	int screenWidth;
	int screenHeight;
	char currentWeaponBullets;
	char currentWeaponMaxBullets;

	sf::Sprite rifleIcon;
	sf::Sprite pistolIcon;
	sf::Sprite knifeIcon;
	std::vector<sf::Sprite> bulletIcons;

	sf::Texture rifleTexture;
	sf::Texture pistolTexture;
	sf::Texture knifeTexture;
	sf::Texture bulletTexture;

	sf::RectangleShape healthBarMax;
	sf::RectangleShape healthBarCurrent;

	void PositionWeaponIcons(int currentWeapon);
};

