#pragma once

#include <SFML/Graphics.hpp>
#include "TextureArray.h"

class AnimatedSprite : public sf::Sprite
{
public:
	AnimatedSprite();
	AnimatedSprite(const std::string& spritesheetFile, bool loop = false, bool interupt = true);
	~AnimatedSprite();

	void Update(float deltaTime);

	bool SetSpritesheet(const std::string& spritesheet, bool loop = true, bool interupt = true);
	bool SetSpritesheet(TextureArray *spritesheetPtr);
	void SetFrameTime(float time) { frameTime = time; }

	inline void Play() { bPlay = true; }
	inline void Pause() { bPlay = false; }
	inline void Stop() { bPlay = false; currentFrame = 0; }
	void Reset();

	inline int GetCurrentFrame() const { return currentFrame; }

	inline int LoopCount() const { return loopCount; }
	inline bool IsCompleted() const { return loopCount > 0; }
	inline bool CanBeInterupted() const { return pSpritesheet->CanBeInterupted(); }

private:
	TextureArray* pSpritesheet;
	bool bDeleteTexture;

	bool bPlay;
	float frameTime;
	int currentFrame;
	int loopCount;
};

