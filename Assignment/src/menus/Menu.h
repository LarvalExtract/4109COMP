#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class SFML_Application;

enum class MenuAlignment : unsigned char
{
	LEFT,
	CENTER,
	RIGHT
};

class Menu : public sf::Drawable, sf::Transformable
{
public:
	Menu();
	~Menu();

	int Update(SFML_Application& app);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void AddMenuItem(const std::string& itemText);

	void SetPosition(const sf::Vector2f& pos);
	void SetMenuItemAlignment(MenuAlignment alignment);
	void SetSpacing(float spacing);
	void SetFontSize(unsigned int size);

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

private:
	void PositionMenuItems();
	void UpdateMenuBoundary();
};