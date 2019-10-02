#include "AnimatedSprite.h"
#include <fstream>
#include <sstream>

AnimatedSprite::AnimatedSprite() : 
	bPlay(true),
	frameTime(1.0f / 30.0f), 
	currentFrame(0), 
	loopCount(-1), 
	pSpritesheet(nullptr),
	bDeleteTexture(true)
{
}

AnimatedSprite::AnimatedSprite(const std::string& spritesheetFile, bool loop, bool interupt) :
	bPlay(true),
	frameTime(1.0f / 30.0f),
	currentFrame(0),
	loopCount(-1),
	pSpritesheet(nullptr),
	bDeleteTexture(true)
{
	SetSpritesheet(spritesheetFile, loop, interupt);
}


AnimatedSprite::~AnimatedSprite()
{
	if (bDeleteTexture)
		if (pSpritesheet != nullptr)
			delete pSpritesheet;
}

void AnimatedSprite::Update(float deltaTime)
{
	static float accumulatedTime = 0.0f;

	accumulatedTime += deltaTime;

	if (bPlay)
	{
		while (accumulatedTime > frameTime)
		{
			if (pSpritesheet->IsLooping())
			{
				if ((currentFrame %= pSpritesheet->FrameCount()) == 0)
					loopCount++;
			}
			else
			{
				if (currentFrame >= pSpritesheet->FrameCount())
				{
					currentFrame = pSpritesheet->FrameCount() - 1;
					loopCount = 1;
				}
			}

			TextureArray& spritesheet = *pSpritesheet;
			this->setTextureRect(spritesheet[currentFrame].rect);
			this->setOrigin(spritesheet[currentFrame].origin);

			accumulatedTime -= frameTime;

			currentFrame++;
		}
	}
}

bool AnimatedSprite::SetSpritesheet(const std::string& spritesheetFile, bool loop, bool interupt)
{
	if (pSpritesheet == nullptr)
		pSpritesheet = new TextureArray(spritesheetFile, loop, interupt);

	TextureArray &spritesheet = *pSpritesheet;

	this->setTexture(spritesheet);
	this->setTextureRect(spritesheet[0].rect);
	this->setOrigin(spritesheet[0].origin);

	return true;
}

bool AnimatedSprite::SetSpritesheet(TextureArray *spritesheetPtr)
{
	if (spritesheetPtr == nullptr)
		return false;

	pSpritesheet = spritesheetPtr;
	bDeleteTexture = false;

	TextureArray &spritesheet = *pSpritesheet;
	this->setTexture(spritesheet);
	this->setTextureRect(spritesheet[0].rect);
	this->setOrigin(spritesheet[0].origin);

	return true;
}

void AnimatedSprite::Reset()
{
	currentFrame = 0;
	loopCount = -1;
}
