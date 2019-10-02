#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct Frame
{
	sf::Vector2f origin;
	sf::IntRect rect;
};

class TextureArray : public sf::Texture
{
public:
	TextureArray() : bLoops(false), bInteruptable(true) {}
	TextureArray(const std::string &spritesheet, bool loops = false, bool interupt = true);
	~TextureArray();

	bool SetSpritesheet(const std::string &spritesheet, bool loops = false, bool interupt = true);

	inline unsigned int FrameCount() const { return frames.size(); }
	inline bool IsLooping() const { return bLoops; }
	inline bool CanBeInterupted() const { return bInteruptable; }

	Frame& operator[] (std::size_t index) { return frames[index]; }

private:
	std::vector<Frame> frames;
	bool bLoops;
	bool bInteruptable;

	bool ParseSpriteInfo(const std::string& spriteInfo);
};

