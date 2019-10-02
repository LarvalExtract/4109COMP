#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>

#include "../AnimatedSprite.h"
#include "Bullet.h"

class Player : public sf::Drawable, public sf::Transformable
{
public:
	enum State : unsigned char
	{
		IDLE,
		MOVING,
		MELEE,
		FIRE,
		RELOAD,
		MAX_STATES
	} playerState, lastPlayerState;

	enum Weapon : unsigned char
	{
		FLASHLIGHT,
		KNIFE,
		PISTOL,
		RIFLE,
		MAX_WEAPONS
	} weaponState;

public:
	Player();
	~Player();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Reset();

	bool FiredWeapon();
	inline Weapon CurrentWeapon() const { return weaponState; }
	inline State PlayerState() const { return playerState; }

	bool Melee();
	void ShootBullet(Bullet& bullet);

	void Hurt(float damage);

	inline float GetHealth() const { return health; }
	inline float GetMaxHealth() const { return maxHealth; }
	inline char GetPistolAmmo() const { return pistolAmmo; }
	inline char GetRifleAmmo() const { return rifleAmmo; }
	inline char GetMaxPistolAmmo() const { return maxPistolAmmo; }
	inline char GetMaxRifleAmmo() const { return maxRifleAmmo; }

	AnimatedSprite& GetSprite() const { return *pCurrentAnim; }

private:
	float timeSinceHurt;

	sf::Color overlayColor;

	AnimatedSprite* pCurrentAnim;
	AnimatedSprite flashlightAnims[3];
	AnimatedSprite knifeAnims[3];
	AnimatedSprite pistolAnims[5];
	AnimatedSprite rifleAnims[5];

	sf::Vector2f moveDirection;

	void ResetAnimColours();
	void MovePlayer(float deltaTime);
	void SwitchWeapon();
	void Reload();

	float health;
	const float maxHealth;
	char pistolAmmo;
	char rifleAmmo;
	const char maxPistolAmmo;
	const char maxRifleAmmo;

	bool bFiring;
	bool bReloading;
	float timeSinceLastShot;
};

