#include "Bullet.h"

sf::Texture* Bullet::pBulletTexture = nullptr;
int Bullet::bulletCount = 0;

Bullet::Bullet() :
	bActive(false),
	speed(1000.0f),
	damage(50.0f),
	activeTime(5.0f)
{
	if (Bullet::pBulletTexture == nullptr)
	{
		pBulletTexture = new sf::Texture;
		pBulletTexture->loadFromFile("Media/Textures/weapons/bullet.png");
	}

	this->setTexture(*Bullet::pBulletTexture);
	this->setOrigin(0.0f, this->getLocalBounds().height * 0.5f);
	this->setScale(0.2f, 0.2f);
	bulletCount++;
}

Bullet::~Bullet()
{
	if (pBulletTexture == nullptr && bulletCount == 0)
		delete pBulletTexture;
}

void Bullet::Update(float deltaTime)
{
	if (!bActive)
		return;

	if (accumulatedTime > activeTime)
	{
		bActive = false;
		accumulatedTime = 0.0f;
	}

	move(direction * deltaTime);

	accumulatedTime += deltaTime;
}

void Bullet::Fire(const sf::Vector2f& startPos, float angle)
{
	bActive = true;

	this->setPosition(startPos);
	this->setRotation(angle);

	float radians = angle * (3.14159f / 180.0f);

	direction.x = speed * cosf(radians);
	direction.y = speed * sinf(radians);

	accumulatedTime = 0.0f;
}
