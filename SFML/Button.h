#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
public:
	Button();
	Button(sf::Vector2f, sf::Texture, sf::Texture, sf::Texture);

	void setFont(sf::Font&);
	void setTexture(sf::Texture&);

	void getPressed(bool);
	bool autoSetTexture(sf::RenderWindow&);
	bool autoSetTexture(sf::RenderWindow&, std::vector<Button*>&, bool*);
	void setTextColor(sf::Color);
	bool isMouseOver(sf::RenderWindow&);

	void setPosition(sf::Vector2f);
	void drawTo(sf::RenderWindow&);
	
private:
	sf::Sprite button;
	sf::Text text;

	int btnWidth;
	int btnHeight;

	bool pressed = false;
	bool is_default;

	sf::Texture normalTexture, activatedTexture, pressedTexture;
};