#include "Menu.h"

#include <fstream>

Menu::Menu(const std::string& menuInfoFile) :
	position(0.0f, 0.0f),
	itemSpacing(10.0f),
	fontSize(40),
	fontColor(sf::Color::White),
	fontHighlightColor(sf::Color::Red),
	bSoundIsPlaying(false)
{
	font.loadFromFile("Media/Fonts/OctoberCrow.ttf");

	std::ifstream file(menuInfoFile);

	if (!file.is_open())
		return;

	std::string line;
	while (std::getline(file, line))
	{
		sf::Text text;
		text.setString(line);
		text.setFont(font);
		text.setCharacterSize(fontSize);

		menuItems.push_back(text);
	}

	UpdateMenuBoundary();
}

Menu::~Menu() 
{

}

int Menu::Update(const sf::Vector2i& mousePosition)
{
	sf::FloatRect itemBounds;
	sf::Vector2f cursor = static_cast<sf::Vector2f>(mousePosition);

	int index = -1;

	// Check if cursor overlaps menu
	if (boundary.contains(cursor))
	{
		// Check which menu item the cursor is highlighting
		for (int i = 0; i < menuItems.size(); i++)
		{
			itemBounds = menuItems[i].getGlobalBounds();

			// Change colour of highlighted item
			if (itemBounds.contains(cursor))
			{
				menuItems[i].setFillColor(fontHighlightColor);

				// Return index of selected menu item
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					index = i;
				}
			}
			else
			{
				menuItems[i].setFillColor(fontColor);
			}
		}
	}
	else
	{
		for (sf::Text& item : menuItems)
			if (item.getFillColor() == fontHighlightColor)
				item.setFillColor(sf::Color::White);

		bSoundIsPlaying = false;
	}

	return index;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Text& menuItem : menuItems)
	{
		target.draw(menuItem);	
	}

	//sf::RectangleShape rect;
	//rect.setPosition(position);
	//rect.setSize(sf::Vector2f(boundary.width, boundary.height));
	//rect.setFillColor(sf::Color::Transparent);
	//rect.setOutlineColor(sf::Color::Red);
	//rect.setOutlineThickness(1.0f);
	//target.draw(rect);
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

void Menu::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;

	PositionMenuItems();
}

void Menu::SetSpacing(float spacing)
{
	spacing > 0.0f ? itemSpacing = spacing : itemSpacing = 1.0f;

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

void Menu::SetFontColour(sf::Color color)
{
	for (sf::Text& menuItem : menuItems)
	{
		menuItem.setFillColor(color);
	}
}

void Menu::SetMenuItemAlignment(MenuAlignment alignment)
{
	UpdateMenuBoundary();

	for (sf::Text& menuItem : menuItems)
	{
		switch (alignment)
		{
		case MenuAlignment::LEFT:	
			menuItem.setPosition(menuItem.getPosition());
			break;

		case MenuAlignment::RIGHT:	
			menuItem.setPosition(position.x + boundary.width - menuItem.getLocalBounds().width, menuItem.getPosition().y); 
			break;

		case MenuAlignment::CENTER:	
			menuItem.setPosition((position.x + boundary.width * 0.5f) - (menuItem.getLocalBounds().width * 0.5f), menuItem.getPosition().y);
			break;
		}
	}

	PositionMenuItems();
}

void Menu::PositionMenuItems()
{
	float yPos = position.y;
	for (sf::Text& menuItem : menuItems)
	{
		menuItem.setPosition(position.x + menuItem.getPosition().x, yPos);

		yPos = menuItem.getPosition().y + fontSize + itemSpacing;
	}

	UpdateMenuBoundary();
}

void Menu::UpdateMenuBoundary()
{
	boundary.left = menuItems[0].getPosition().x;
	boundary.top = menuItems[0].getPosition().y;
	boundary.width = 0.0f;
	boundary.height = 0.0f;

	for (sf::Text& menuItem : menuItems)
	{
		if (menuItem.getLocalBounds().width > boundary.width)
			boundary.width = menuItem.getLocalBounds().width;

		if (menuItem.getPosition().x < boundary.left)
			boundary.left = menuItem.getPosition().x;
	}

	boundary.height = (fontSize + itemSpacing) * (menuItems.size());
}