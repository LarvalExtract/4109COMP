#pragma once

#include <SFML/Graphics.hpp>
#include "../AnimatedSprite.h"
#include "TextureArray.h"

class Zombie : public sf::Drawable, public sf::Transformable
{
	enum State
	{
		IDLE,
		MOVING,
		ALERTED,
		ATTACK,
		DEAD,
		MAX_STATES
	} npcState;

public:
	Zombie();
	~Zombie();

	void Update(float deltaTime, const sf::Vector2f& playerPos);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline void SetActive(bool value) { bActive = value; bAlerted = false; }
	inline bool IsActive() const { return bActive; }

	bool IsInVision(const sf::Vector2f& target);
	bool IsInHearingRange(const sf::Vector2f& target);

	inline AnimatedSprite& GetSprite() const { return *pCurrentAnim; }

	bool Attack();
	void Hurt(float damage);
	inline float GetHealth() const { return health; }
	inline void SetHealth(float health) { this->health = health; }
	inline void SetAlerted(bool value) { bAlerted = value; }
	void Reset();

private:
	bool bActive;

	AnimatedSprite* pCurrentAnim;
	AnimatedSprite animIdle;
	AnimatedSprite animMoving;
	AnimatedSprite animAttack;
	AnimatedSprite animBlood;

	// Shared spritesheet between all zombies
	static int zombieCount;
	static TextureArray* pSpritesheets;

	float patrolMoveSpeed;
	float fovAngle;
	float hearingRadius;
	float viewDistance;
	float patrolTime;	// zombie 
	bool bAlerted;
	float timeSinceHurt;
	float timeSinceLastAttack;

	sf::Color overlayColour;

	float health;

	sf::Vector2f patrolTargetPos;

	void Patrol(float deltaTime);
	void MoveTowards(const sf::Vector2f& target, float deltaTime, float speedMultiplier = 1.0f);

	// debug stuff
private:
	bool bDrawDebugShapes;
	mutable sf::ConvexShape visionCone;
	mutable sf::CircleShape hearingCircle;

	void InitDebugShapes();
};

