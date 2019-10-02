#include "Player.h"

const float MOVEMENT_SPEED = 375.0f;
const int MAX_RIFLE_AMMO = 30;
const int MAX_PISTOL_AMMO = 12;

Player::Player() :
	pCurrentAnim(nullptr),
	playerState(IDLE),
	weaponState(PISTOL),
	maxPistolAmmo(12),
	maxRifleAmmo(30),
	pistolAmmo(12),
	rifleAmmo(30),
	timeSinceLastShot(0.0f),
	overlayColor(sf::Color::White),
	maxHealth(200.0f),
	health(200.0f)
{
	// set flashlight animations
	flashlightAnims[IDLE].SetSpritesheet("hero_flashlight_idle", true);
	flashlightAnims[MOVING].SetSpritesheet("hero_flashlight_move", true);
	flashlightAnims[MELEE].SetSpritesheet("hero_flashlight_melee", false, false);

	// set knife animations
	knifeAnims[IDLE].SetSpritesheet("hero_knife_idle", true);
	knifeAnims[MOVING].SetSpritesheet("hero_knife_move", true);
	knifeAnims[MELEE].SetSpritesheet("hero_knife_melee", false, false);

	// set pistol animations
	pistolAnims[IDLE].SetSpritesheet("hero_pistol_idle", true);
	pistolAnims[MOVING].SetSpritesheet("hero_pistol_move", true);
	pistolAnims[MELEE].SetSpritesheet("hero_pistol_melee", true);
	pistolAnims[FIRE].SetSpritesheet("hero_pistol_shoot", false, false);
	pistolAnims[RELOAD].SetSpritesheet("hero_pistol_reload", false, false);

	// set rifle animations
	rifleAnims[IDLE].SetSpritesheet("hero_rifle_idle", true);
	rifleAnims[MOVING].SetSpritesheet("hero_rifle_move", true);
	rifleAnims[MELEE].SetSpritesheet("hero_rifle_melee", true);
	rifleAnims[FIRE].SetSpritesheet("hero_rifle_shoot", false, false);
	rifleAnims[RELOAD].SetSpritesheet("hero_rifle_reload", false, false);

	setOrigin(80, pistolAnims[IDLE].getLocalBounds().height / 2);
	setScale(0.4f, 0.4f);

	pCurrentAnim = &pistolAnims[IDLE];
	pCurrentAnim->setOrigin(getOrigin());
}


Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	static float accumulatedTime = 0.0f;

	MovePlayer(deltaTime);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		playerState = MELEE;

	switch (weaponState)
	{
	case FLASHLIGHT:	pCurrentAnim = &flashlightAnims[playerState]; break;
	case KNIFE:			pCurrentAnim = &knifeAnims[playerState]; break;
	case PISTOL:		pCurrentAnim = &pistolAnims[playerState]; break;
	case RIFLE:			pCurrentAnim = &rifleAnims[playerState]; break;
	}

	switch (playerState)
	{
	case MELEE:
		if (pCurrentAnim->IsCompleted())
		{
			pCurrentAnim->Reset();
			playerState = IDLE;
		}
		break;
	case FIRE:
		if (pCurrentAnim->IsCompleted())
			playerState = IDLE;
		break;

	case RELOAD:
		if (pCurrentAnim->IsCompleted())
		{
			pCurrentAnim->Reset();
			playerState = IDLE;

			switch (weaponState)
			{
			case PISTOL:	pistolAmmo = maxPistolAmmo;	break;
			case RIFLE:		rifleAmmo = maxRifleAmmo;	break;
			}
		}
		break;

	default:
		if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
			playerState = MOVING;
		else
			playerState = IDLE;

		SwitchWeapon();

		break;
	}

	pCurrentAnim->setPosition(getPosition());
	pCurrentAnim->setRotation(getRotation());
	pCurrentAnim->setScale(getScale());
	pCurrentAnim->setColor(overlayColor);
	pCurrentAnim->Update(deltaTime);

	if (pCurrentAnim->getColor() == sf::Color::Red)
	{
		if (timeSinceHurt > 0.1f)
		{
			timeSinceHurt = 0.0f;
			overlayColor = sf::Color::White;
		}
		else
		{
			timeSinceHurt += deltaTime;
		}
	}

	timeSinceLastShot += deltaTime;
	lastPlayerState = playerState;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*pCurrentAnim);
}

void Player::Reset()
{
	rifleAmmo = maxRifleAmmo;
	pistolAmmo = maxPistolAmmo;
	health = maxHealth;
	weaponState = PISTOL;
	playerState = IDLE;
}

bool Player::FiredWeapon()
{
	static bool isM1Pressed, wasM1Pressed = false;

	if (playerState == RELOAD)
		return false;

	isM1Pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	switch (weaponState)
	{
	case PISTOL:
		if (isM1Pressed && !wasM1Pressed)	// Single shot fire pistol
		{
			if (pistolAmmo > 0)
			{
				if (timeSinceLastShot > 0.25f)
				{
					timeSinceLastShot = 0.0f;
					playerState = FIRE;
					pistolAnims[FIRE].Reset();
					return true;
				}
			}
			else
			{
				Reload();
			}
		}
		break;
		
	case RIFLE:
		if (isM1Pressed)	// Hold to fire rifle
		{
			if (rifleAmmo > 0)
			{
				if (timeSinceLastShot > 0.1f)
				{
					timeSinceLastShot = 0.0f;
					playerState = FIRE;
					rifleAnims[FIRE].Reset();
					return true;
				}
			}
			else
			{
				Reload();
			}
			break;
		}
	}

	wasM1Pressed = isM1Pressed;

	return false;
}

bool Player::Melee()
{
	if (playerState != MELEE)
	{
		return false;
	}

	switch (weaponState)
	{
	case FLASHLIGHT:	return pCurrentAnim->GetCurrentFrame() == 7;
	case KNIFE:			return pCurrentAnim->GetCurrentFrame() == 7;
	case PISTOL:		return pCurrentAnim->GetCurrentFrame() == 8;
	case RIFLE:			return pCurrentAnim->GetCurrentFrame() == 9;
	}
}

void Player::ShootBullet(Bullet& bullet)
{
	switch (weaponState)
	{
	case PISTOL:
		bullet.setOrigin(-200.0f, -65.0f);
		bullet.Fire(getPosition(), getRotation());
		bullet.SetDamage(10.0f);
		pistolAmmo--;
		break;

	case RIFLE:
		bullet.setOrigin(-300.0f, -45.0f);
		bullet.Fire(getPosition(), getRotation());
		bullet.SetDamage(15.0f);
		rifleAmmo--;
		break;
	}
}

void Player::Hurt(float damage)
{
	overlayColor = sf::Color::Red;
	health -= damage;

	if (health <= 0.0f)
		health = maxHealth;
}

void Player::ResetAnimColours()
{
	flashlightAnims[IDLE].setColor(sf::Color::White);
	flashlightAnims[MOVING].setColor(sf::Color::White);
	flashlightAnims[MELEE].setColor(sf::Color::White);
	knifeAnims[IDLE].setColor(sf::Color::White);
	knifeAnims[MOVING].setColor(sf::Color::White);
	knifeAnims[MELEE].setColor(sf::Color::White);
	pistolAnims[IDLE].setColor(sf::Color::White);
	pistolAnims[MOVING].setColor(sf::Color::White);
	pistolAnims[MELEE].setColor(sf::Color::White);
	pistolAnims[FIRE].setColor(sf::Color::White);
	pistolAnims[RELOAD].setColor(sf::Color::White);
	rifleAnims[IDLE].setColor(sf::Color::White);
	rifleAnims[MOVING].setColor(sf::Color::White);
	rifleAnims[MELEE].setColor(sf::Color::White);
	rifleAnims[FIRE].setColor(sf::Color::White);
	rifleAnims[RELOAD].setColor(sf::Color::White);
}

void Player::MovePlayer(float deltaTime)
{
	moveDirection.x = 0.0f;
	moveDirection.y = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		moveDirection.y = -1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		moveDirection.y = 1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		moveDirection.x = -1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		moveDirection.x = 1.0f;

	if (moveDirection.x == 0.0f && moveDirection.y == 0.0f)
		return;

	float magnitude = sqrtf((moveDirection.x * moveDirection.x) + (moveDirection.y * moveDirection.y));
	moveDirection.x /= magnitude;
	moveDirection.y /= magnitude;

	move(moveDirection * MOVEMENT_SPEED * deltaTime);
}

enum Keys { NUM1, NUM2, NUM3, Q, R };
void Player::SwitchWeapon()
{
	static std::bitset<8> wasKeyPressed, isKeyPressed;

	isKeyPressed[NUM1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1);
	isKeyPressed[NUM2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2);
	isKeyPressed[NUM3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3);
	isKeyPressed[Q] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	isKeyPressed[R] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);

	if (isKeyPressed[NUM1] && !wasKeyPressed[NUM1])
	{
		weaponState = Weapon::KNIFE;
	}
	else if (isKeyPressed[NUM2] && !wasKeyPressed[NUM2])
	{
		weaponState = Weapon::PISTOL;
	}
	else if (isKeyPressed[NUM3] && !wasKeyPressed[NUM3])
	{
		weaponState = Weapon::RIFLE;
	}
	else if (isKeyPressed[Q] && !wasKeyPressed[Q])
	{
		weaponState = Weapon::FLASHLIGHT;
	}
	else if (isKeyPressed[R] && !wasKeyPressed[R])
	{
		Reload();
	}		
	
	wasKeyPressed = isKeyPressed;
}

void Player::Reload()
{
	switch (weaponState)
	{
	case PISTOL:
		if (pistolAmmo < maxPistolAmmo)
		{
			playerState = RELOAD;
			pistolAnims[RELOAD].Reset();
			pistolAnims[RELOAD].Play();
		}
		break;

	case RIFLE:
		if (rifleAmmo < maxRifleAmmo)
		{
			playerState = RELOAD;
			rifleAnims[RELOAD].Reset();
			rifleAnims[RELOAD].Play();
		}
		break;
	}
}
