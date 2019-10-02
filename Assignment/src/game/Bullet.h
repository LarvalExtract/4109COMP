#pragma once

#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite
{
public:
	Bullet();
	~Bullet();

	void Update(float deltaTime);

	void Fire(const sf::Vector2f& startPos, float angle);

	inline void SetActive(bool active) { bActive = active; }
	inline bool IsActive() const { return bActive; }

	inline void SetSpeed(float speed) { this->speed = speed; }
	inline void SetDamage(float damage) { this->damage = damage; }
	inline float GetSpeed() const { return this->speed; }
	inline float GetDamage() const { return this->damage; }

private:
	static sf::Texture* pBulletTexture;
	static int bulletCount;

	bool bActive;
	float speed;
	float damage;
	float activeTime;

	float accumulatedTime;

	sf::Vector2f direction;
};

