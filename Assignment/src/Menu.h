#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

enum class MenuAlignment : unsigned char
{
	LEFT,
	CENTER,
	RIGHT
};

class Menu : public sf::Drawable, sf::Transformable
{
public:
	Menu(const std::string& menuInfoFile);
	~Menu();

	int Update(const sf::Vector2i& mousePosition);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void AddMenuItem(const std::string& itemText);

	void SetPosition(const sf::Vector2f& pos);
	void SetPosition(float x, float y);
	void SetMenuItemAlignment(MenuAlignment alignment);
	void SetSpacing(float spacing);
	void SetFontSize(unsigned int size);
	void SetFontColour(sf::Color color);
	inline void SetFontHighlightColor(sf::Color color) { fontHighlightColor = color; }

	inline sf::Vector2f GetPosition() const { return position; }
	inline float GetSpacing() const { return itemSpacing; }
	inline unsigned int GetFontSize() const { return fontSize; }
	inline sf::FloatRect GetLocalBounds() const { return boundary; }

private:
	std::vector<sf::Text> menuItems;

	sf::Font font;
	sf::Color fontColor;
	sf::Color fontHighlightColor;

	sf::Vector2f position;
	sf::FloatRect boundary;
	float itemSpacing;
	unsigned int fontSize;

	bool bSoundIsPlaying;

private:
	void PositionMenuItems();
	void UpdateMenuBoundary();
};