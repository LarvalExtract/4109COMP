#include "HUD.h"

const sf::Vector2f MARGIN(30.0f, 60.0f);

const sf::Vector2f ICON_EQUIPPED_SCALE(0.5f, 0.5f);
const sf::Vector2f ICON_UNEQUIPPED_SCALE(0.2f, 0.2f);

HUD::HUD() :
	bInitialised(false),
	bulletIcons(30)
{
	
}

HUD::~HUD()
{
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(knifeIcon);
	target.draw(pistolIcon);
	target.draw(rifleIcon);

	for (int i = 0; i < currentWeaponMaxBullets; i++)
		target.draw(bulletIcons[i]);

	target.draw(healthBarMax);
	target.draw(healthBarCurrent);
}

void HUD::Initialise(int screenWidth, int screenHeight, const Player& player)
{
	if (!rifleTexture.loadFromFile("Media/Textures/weapons/ak-47.png"))
		return;

	if (!pistolTexture.loadFromFile("Media/Textures/weapons/glock.png"))
		return;

	if (!knifeTexture.loadFromFile("Media/Textures/weapons/knife.png"))
		return;

	if (!bulletTexture.loadFromFile("Media/Textures/weapons/bullet.png"))
		return;

	rifleIcon.setTexture(rifleTexture);
	pistolIcon.setTexture(pistolTexture);
	knifeIcon.setTexture(knifeTexture);

	for (sf::Sprite& sprite : bulletIcons)
		sprite.setTexture(bulletTexture);

	rifleIcon.setOrigin(0, rifleIcon.getLocalBounds().height / 2);
	pistolIcon.setOrigin(0, pistolIcon.getLocalBounds().height / 2);
	knifeIcon.setOrigin(0, knifeIcon.getLocalBounds().height / 2);

	sf::Vector2f bulletPos(20.0f, 150.0f);
	for (int i = 0; i < bulletIcons.size(); i++)
	{
		bulletIcons[i].setRotation(-90.0f);
		bulletIcons[i].setScale(0.25f, 0.25f);
		bulletIcons[i].setPosition(bulletPos);
		bulletPos.x += bulletIcons[i].getGlobalBounds().width + 4;
	}

	healthBarMax.setPosition(40.0f, screenHeight - 70);
	healthBarMax.setSize(sf::Vector2f(player.GetMaxHealth(), 30.0f));
	healthBarCurrent.setPosition(healthBarMax.getPosition());
	healthBarMax.setFillColor(sf::Color(63, 0, 0, 255));
	healthBarMax.setOutlineColor(sf::Color::Black);
	healthBarMax.setOutlineThickness(2.0f);
	healthBarCurrent.setFillColor(sf::Color::Red);

	PositionWeaponIcons(player.CurrentWeapon());

	bInitialised = true;
}

void HUD::Update(const Player& player)
{
	PositionWeaponIcons(player.CurrentWeapon());

	switch (player.CurrentWeapon())
	{
	case Player::Weapon::PISTOL:
		currentWeaponBullets = player.GetPistolAmmo();
		currentWeaponMaxBullets = player.GetMaxPistolAmmo(); 
		break;
	case Player::Weapon::RIFLE:
		currentWeaponBullets = player.GetRifleAmmo();
		currentWeaponMaxBullets = player.GetMaxRifleAmmo();
		break;
	}

	int bulletsFired = currentWeaponMaxBullets - currentWeaponBullets;

	for (int i = 0; i < currentWeaponMaxBullets; i++)
	{
		if (i > currentWeaponBullets - 1)
			bulletIcons[i].setColor(sf::Color(128, 128, 128, 255));
		else
			bulletIcons[i].setColor(sf::Color::White);
	}

	healthBarCurrent.setSize(sf::Vector2f(player.GetHealth(), 30.0f));
}

void HUD::PositionWeaponIcons(int currentWeapon)
{
	float pistolOffset;
	float knifeOffset;
	float rifleOffset;

	switch (currentWeapon)
	{
	case Player::Weapon::KNIFE:
		knifeIcon.setScale(ICON_EQUIPPED_SCALE);
		pistolIcon.setScale(ICON_UNEQUIPPED_SCALE);
		rifleIcon.setScale(ICON_UNEQUIPPED_SCALE);

		pistolOffset = pistolIcon.getPosition().x + pistolIcon.getGlobalBounds().width;
		knifeOffset = knifeIcon.getPosition().x + knifeIcon.getGlobalBounds().width;
		rifleOffset = rifleIcon.getPosition().x + rifleIcon.getGlobalBounds().width;

		knifeIcon.setPosition(MARGIN);
		pistolIcon.setPosition(knifeOffset + 10, MARGIN.y);
		rifleIcon.setPosition(pistolOffset + 10, MARGIN.y);
		break;

	case Player::Weapon::PISTOL:
		pistolIcon.setScale(ICON_EQUIPPED_SCALE);
		knifeIcon.setScale(ICON_UNEQUIPPED_SCALE);
		rifleIcon.setScale(ICON_UNEQUIPPED_SCALE);

		pistolOffset = pistolIcon.getPosition().x + pistolIcon.getGlobalBounds().width;
		knifeOffset = knifeIcon.getPosition().x + knifeIcon.getGlobalBounds().width;
		rifleOffset = rifleIcon.getPosition().x + rifleIcon.getGlobalBounds().width;

		pistolIcon.setPosition(MARGIN);
		rifleIcon.setPosition(pistolOffset + 10, MARGIN.y);
		knifeIcon.setPosition(rifleOffset + 10, MARGIN.y);
		break;

	case Player::Weapon::RIFLE:
		rifleIcon.setScale(ICON_EQUIPPED_SCALE);
		pistolIcon.setScale(ICON_UNEQUIPPED_SCALE);
		knifeIcon.setScale(ICON_UNEQUIPPED_SCALE);

		pistolOffset = pistolIcon.getPosition().x + pistolIcon.getGlobalBounds().width;
		knifeOffset = knifeIcon.getPosition().x + knifeIcon.getGlobalBounds().width;
		rifleOffset = rifleIcon.getPosition().x + rifleIcon.getGlobalBounds().width;

		rifleIcon.setPosition(MARGIN);
		knifeIcon.setPosition(rifleOffset + 10, MARGIN.y);
		pistolIcon.setPosition(knifeOffset + 10, MARGIN.y);
		break;
	}
}
