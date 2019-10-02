#include "Zombie.h"

TextureArray* Zombie::pSpritesheets = nullptr;
int Zombie::zombieCount = 0;

constexpr float degreesPerRadians = 180.0f / 3.14159;

Zombie::Zombie() :
	npcState(IDLE),
	bActive(false),
	patrolMoveSpeed(70.0f),
	fovAngle(90.0f),
	hearingRadius(425.0f),
	viewDistance(800.0f),
	patrolTime(2.5f),
	bAlerted(false),
	timeSinceHurt(0.0f),
	timeSinceLastAttack(0.0f),
	health(100.0f),
	overlayColour(sf::Color::White),
	pCurrentAnim(nullptr),
	bDrawDebugShapes(false)
{
	if (pSpritesheets == nullptr)
	{
		Zombie::pSpritesheets = new TextureArray[MAX_STATES];
		pSpritesheets[IDLE].SetSpritesheet("zombie-idle", true);
		pSpritesheets[MOVING].SetSpritesheet("zombie-move", true);
		pSpritesheets[ATTACK].SetSpritesheet("zombie-attack", false, true);
		pSpritesheets[DEAD].SetSpritesheet("blood", false, false);
	}

	animIdle.SetSpritesheet(&pSpritesheets[IDLE]);
	animMoving.SetSpritesheet(&pSpritesheets[MOVING]);
	animAttack.SetSpritesheet(&pSpritesheets[ATTACK]);
	animBlood.SetSpritesheet(&pSpritesheets[DEAD]);

	pCurrentAnim = &animIdle;

	setScale(0.4f, 0.4f);

	zombieCount++;

	InitDebugShapes();
}

Zombie::~Zombie()
{
	if (zombieCount == 0 && pSpritesheets != nullptr)
	{
		delete[] pSpritesheets;
		pSpritesheets = nullptr;
	}
}

void Zombie::Reset()
{
	bAlerted = false;
	overlayColour = sf::Color::White;
	health = 100.0f;
	npcState = IDLE;
	pCurrentAnim = &animIdle;

	animIdle.setColor(sf::Color::White);
	animMoving.setColor(sf::Color::White);
	animAttack.setColor(sf::Color::White);
	animBlood.Reset();
}

void Zombie::Update(float deltaTime, const sf::Vector2f& playerPos)
{
	if (!bActive)
		return;

	if (npcState == IDLE || npcState == MOVING)
		if (IsInVision(playerPos) || IsInHearingRange(playerPos))
			npcState = ALERTED;

	switch (npcState)
	{
	case IDLE:
		pCurrentAnim = &animIdle;
		Patrol(deltaTime);
		break;

	case MOVING:
		pCurrentAnim = &animMoving;
		MoveTowards(patrolTargetPos, deltaTime);
		Patrol(deltaTime);
		break;

	case ALERTED:
		pCurrentAnim = &animMoving;
		MoveTowards(playerPos, deltaTime, 5.0f);
		break;

	case ATTACK:
		pCurrentAnim = &animAttack;	
		MoveTowards(playerPos, deltaTime, 2.0f);	// Slow zombie down when attacking to ease difficulty

		if (pCurrentAnim->IsCompleted())
		{
			animAttack.Reset();
			npcState = ALERTED;
			timeSinceLastAttack = 0.0f;
		}
		break;

	case DEAD:
		pCurrentAnim = &animBlood;

		if (pCurrentAnim->IsCompleted())
		{
			bActive = false;
		}
		break;
	}

	pCurrentAnim->setPosition(getPosition());
	pCurrentAnim->setRotation(getRotation());
	pCurrentAnim->setScale(getScale());
	pCurrentAnim->setColor(overlayColour);
	pCurrentAnim->Update(deltaTime);

	if (pCurrentAnim->getColor() == sf::Color::Red)
	{
		if (timeSinceHurt > 0.1f)
		{
			timeSinceHurt = 0.0f;
			overlayColour = sf::Color::White;
		}
		else
		{
			timeSinceHurt += deltaTime;
		}
	}

	timeSinceLastAttack += deltaTime;
}

void Zombie::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!bActive)
		return;

	if (bDrawDebugShapes)
	{
		hearingCircle.setPosition(getPosition());
		visionCone.setPosition(getPosition());
		visionCone.setRotation(getRotation() - (fovAngle * 0.5f));
		target.draw(hearingCircle);
		target.draw(visionCone);
	}

	target.draw(*pCurrentAnim);
}

bool Zombie::IsInVision(const sf::Vector2f& target)
{
	// Calculate vector from zombie to target
	sf::Vector2f zombieToTarget = target - getPosition();

	// Calculate L2 norm of the vector
	float distance = sqrtf((zombieToTarget.x * zombieToTarget.x) + (zombieToTarget.y * zombieToTarget.y));

	if (distance < viewDistance)
	{
		float angle = atan2(zombieToTarget.x, zombieToTarget.y) * degreesPerRadians;

		return (getRotation() - angle < (fovAngle * 0.5f));
	}
	else
	{
		return false;
	}
}

bool Zombie::IsInHearingRange(const sf::Vector2f& target)
{
	// Calculate vector from zombie to target
	sf::Vector2f zombieToTarget = getPosition() - target;

	// Calculate L2 norm of the vector
	float distance = sqrtf((zombieToTarget.x * zombieToTarget.x) + (zombieToTarget.y * zombieToTarget.y));

	return distance < hearingRadius;
}

void Zombie::Patrol(float deltaTime)
{
	static float accummulatedTime = 0.0f;
	accummulatedTime += deltaTime;

	if (accummulatedTime > patrolTime)
	{
		unsigned int action = rand() % 3;

		switch (action)
		{
		case 0:		// randomly change direction
			patrolTargetPos.x = static_cast<float>(rand() % 10000);
			patrolTargetPos.y = static_cast<float>(rand() % 10000);
			npcState = MOVING;
			break;

		case 1:		// change to idle state
			npcState = IDLE;
			break;

		default:	// do nothing
			break;
		}

		accummulatedTime = 0.0f;
	}
}

bool Zombie::Attack()
{
	if (npcState == DEAD)
		return false;

	if (timeSinceLastAttack > 1.0f)
	{
		npcState = ATTACK;

		return animAttack.GetCurrentFrame() == 6;
	}
	else
	{
		return false;
	}
}

void Zombie::Hurt(float damage)
{ 
	if (npcState == DEAD)
		return;

	health -= damage; 
	npcState = ALERTED; 
	overlayColour = sf::Color::Red;

	if (health <= 0.0f)
	{		
		npcState = DEAD;
	}
}

void Zombie::MoveTowards(const sf::Vector2f& target, float deltaTime, float speedMultiplier /*= 1.0f*/)
{
	sf::Vector2f moveDirection = target - getPosition();
	float distance = sqrtf((moveDirection.x * moveDirection.x) + (moveDirection.y * moveDirection.y));

	setRotation(atan2(moveDirection.y, moveDirection.x) * degreesPerRadians);

	sf::Vector2f normalisedVector(moveDirection / distance);

	move(normalisedVector * patrolMoveSpeed * speedMultiplier * deltaTime);
}

void Zombie::InitDebugShapes()
{
	visionCone.setPointCount(18);
	visionCone.setPoint(0, sf::Vector2f(0.0f, 0.0f));

	const int segments = 16;
	float angle = 0.0f;

	constexpr float rads = 3.14159f / 180.0f;

	for (int i = 0; i < segments + 1; i++)
	{
		float x = cosf(i * (fovAngle * rads) / static_cast<float>(segments)) * viewDistance;
		float y = sinf(i * (fovAngle * rads) / static_cast<float>(segments)) * viewDistance;

		visionCone.setPoint(i + 1, sf::Vector2f(x, y));
	}

	visionCone.setFillColor(sf::Color(0, 255, 0, 128));
	visionCone.setOutlineColor(sf::Color::Green);
	visionCone.setOutlineThickness(2.0f);

	hearingCircle.setRadius(hearingRadius);
	hearingCircle.setFillColor(sf::Color(255, 0, 0, 63));
	hearingCircle.setOutlineColor(sf::Color::Red);
	hearingCircle.setOutlineThickness(2.0f);
	hearingCircle.setOrigin(hearingCircle.getLocalBounds().width * 0.5f, hearingCircle.getLocalBounds().height * 0.5f);
}