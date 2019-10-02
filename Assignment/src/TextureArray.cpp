#include "TextureArray.h"

#include <fstream>
#include <sstream>

TextureArray::TextureArray(const std::string &spritesheet, bool loops, bool interupt)
{
	SetSpritesheet(spritesheet, loops, interupt);
}


TextureArray::~TextureArray()
{
}

bool TextureArray::SetSpritesheet(const std::string &spritesheet, bool loops, bool interupt)
{
	const std::string textureFile = "Media/Textures/" + spritesheet + ".png";
	const std::string spriteinfoFile = "Media/SpriteInfo/" + spritesheet + ".txt";

	if (!this->loadFromFile(textureFile))
		return false;

	if (!ParseSpriteInfo(spriteinfoFile))
		return false;

	bLoops = loops;

	return true;
}

bool TextureArray::ParseSpriteInfo(const std::string& spriteInfo)
{
	std::ifstream file(spriteInfo);

	if (!file.is_open())
		return false;

	std::string line;
	std::stringstream ss;

	while (std::getline(file, line))
	{
		ss << line;

		Frame frameInfo;
		ss >> frameInfo.rect.left;
		ss >> frameInfo.rect.top;
		ss >> frameInfo.rect.width;
		ss >> frameInfo.rect.height;
		ss >> frameInfo.origin.x;
		ss >> frameInfo.origin.y;

		frameInfo.origin.x -= frameInfo.rect.left;
		frameInfo.origin.y -= frameInfo.rect.top;

		frames.push_back(frameInfo);

		ss.clear();
	}

	file.close();

	return true;
}
