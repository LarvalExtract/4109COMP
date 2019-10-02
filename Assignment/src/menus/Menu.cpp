#include "Menu.h"

#include <fstream>

#include "Application.h"

Menu::Menu() :
	position(0.0f, 0.0f),
	itemSpacing(10.0f),
	fontSize(40),
	fontColor(sf::Color::White),
	fontHighlightColor(sf::Color::Red)
{
	font.loadFromFile("Media/Fonts/SegoeMarker.ttf");
}

Menu::~Menu() 
{

}

int Menu::Update(SFML_Application& app)
{
	sf::Vector2i mousePosition;
	sf::FloatRect itemBounds;

	int index = -1;

	for (int i = 0; i < menuItems.size(); i++)
	{
		mousePosition = app.GetCursorPosition();
		itemBounds = menuItems[i].getGlobalBounds();

		if (mousePosition.x >= itemBounds.left && mousePosition.x <= itemBounds.left + itemBounds.width &&
			mousePosition.y >= itemBounds.top && mousePosition.x <= itemBounds.top + itemBounds.height)
		{
			menuItems[i].setFillColor(fontHighlightColor);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				index = i;
		}
		else
		{
			menuItems[i].setFillColor(fontColor);
		}
	}

	return index;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Text& menuItem : menuItems)
	{
		target.draw(menuItem);	
	}

	sf::RectangleShape rect;
	rect.setPosition(position);
	rect.setSize(sf::Vector2f(boundary.width, boundary.height));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(1.0f);
	target.draw(rect);
}

void Menu::AddMenuItem(const std::string& itemText)
{
	sf::Text text;
	text.setString(itemText);
	text.setFont(font);
	text.setCharacterSize(fontSize);

	menuItems.push_back(text);

	PositionMenuItems();
}

void Menu::SetPosition(const sf::Vector2f& pos)
{
	position = pos;

	PositionMenuItems();
}

void Menu::SetSpacing(float spacing)
{
	spacing > 0.0f ? itemSpacing = spacing : itemSpacing = 0.0f;

	PositionMenuItems();
}

void Menu::SetFontSize(unsigned int size)
{
	size < 100 ? fontSize = size : size = 99;

	for (sf::Text& menuItem : menuItems)
	{
		menuItem.setCharacterSize(fontSize);
	}

	PositionMenuItems();
}

void Menu::SetMenuItemAlignment(MenuAlignment alignment)
{
	UpdateMenuBoundary();

	switch (alignment)
	{
	case MenuAlignment::LEFT:
		for (sf::Text& menuItem : menuItems)
			menuItem.setPosition(boundary.left, menuItem.getPosition().y);
		break;

	case MenuAlignment::RIGHT:
		for (sf::Text& menuItem : menuItems)
			menuItem.setPosition(boundary.left + boundary.width - menuItem.getLocalBounds().width, menuItem.getPosition().y);
		break;

	case MenuAlignment::CENTER:
		for (sf::Text& menuItem : menuItems)
			menuItem.setPosition(boundary.left + (boundary.width / 2) - (menuItem.getLocalBounds().width / 2), menuItem.getPosition().y);
		break;
	}
}

void Menu::PositionMenuItems()
{
	float yPos = position.y;
	for (sf::Text& menuItem : menuItems)
	{
		menuItem.setPosition(position.x, yPos);

		yPos = menuItem.getPosition().y + menuItem.getLocalBounds().height + itemSpacing;
	}

	UpdateMenuBoundary();
}

void Menu::UpdateMenuBoundary()
{
	boundary.left = menuItems[0].getPosition().x;
	boundary.top = menuItems[0].getPosition().y;
	boundary.width = menuItems[0].getLocalBounds().width;
	boundary.height = 0.0f;

	for (sf::Text& menuItem : menuItems)
	{
		boundary.height += menuItem.getLocalBounds().height;

		if (menuItem.getLocalBounds().width > boundary.width)
			boundary.width = menuItem.getLocalBounds().width;

		if (menuItem.getPosition().x < boundary.left)
			boundary.left = menuItem.getPosition().x;
	}

	boundary.height += itemSpacing * (menuItems.size() - 1);
}