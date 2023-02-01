#include "Button.h"

Button::Button() {
	is_default = true;
}

Button::Button(sf::Vector2f factors, sf::Texture bgTexture, sf::Texture activatedTexture, sf::Texture pressedTexture) {
	button.setTexture(bgTexture);
	button.setScale(factors);

	btnWidth = bgTexture.getSize().x * factors.x;
	btnHeight = bgTexture.getSize().y * factors.y;

	this->normalTexture = bgTexture;
	this->activatedTexture = activatedTexture;
	this->pressedTexture = pressedTexture;
	is_default = false;
}

void Button::setFont(sf::Font& fonts) {
	text.setFont(fonts);
}

void Button::setTexture(sf::Texture& texture) {
	button.setTexture(texture);
}

void Button::getPressed(bool value) {
	if (this->pressed != value) {
		if (value) setTexture(pressedTexture);
		else setTexture(normalTexture);
		this->pressed = value;
	}
}

bool Button::autoSetTexture(sf::RenderWindow& window) {
	if (this->pressed) return this->isMouseOver(window);
	if (this->isMouseOver(window)) {
		this->setTexture(this->activatedTexture);
	} else {
		this->setTexture(this->normalTexture);
	}
	return this->isMouseOver(window);
}

bool Button::autoSetTexture(sf::RenderWindow& window, std::vector<Button*>& buttons, bool* opened_sub) {
	bool def = autoSetTexture(window);
	if (!*opened_sub) return def;

	for (auto& btn : buttons) {
		def |= btn->autoSetTexture(window);
	}
	if (def) {
		this->setTexture(this->activatedTexture);
	} else {
		this->setTexture(this->normalTexture);
	}
	return def;
}

void Button::setTextColor(sf::Color color) {
	text.setFillColor(color);
}

void Button::setPosition(sf::Vector2f point) {
	button.setPosition(point);
}

void Button::drawTo(sf::RenderWindow& window) {
	if (!is_default)
		window.draw(button);
}

bool Button::isMouseOver(sf::RenderWindow& window) {
	int mouseX = sf::Mouse::getPosition(window).x;
	int mouseY = sf::Mouse::getPosition(window).y;

	int btnPosX = button.getPosition().x;
	int btnPosY = button.getPosition().y;

	int btnxPosWidth = button.getPosition().x + btnWidth;
	int btnyPosHeight = button.getPosition().y + btnHeight;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
		return true;
	}
	return false;
}