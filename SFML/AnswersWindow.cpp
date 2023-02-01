#include "AnswersWindow.h"

SFML::AnswersWindow::AnswersWindow(std::vector<std::vector<std::string>>* anses) {
	sf::RenderWindow window(sf::VideoMode(980, 512), "Minimal disjunctive normal form generator", sf::Style::Titlebar | sf::Style::Close);
	sf::Font font;
	font.loadFromFile("Consolas.ttf");

	std::vector<sf::Text*> text;
	sf::Text* label = new sf::Text();
	label->setFont(font);
	label->setString("Answers:");
	label->setFillColor(sf::Color::White);
	label->setCharacterSize(14);
	label->setPosition(10, 10);
	label->setLineSpacing(0);
	label->setOutlineThickness(0);
	text.push_back(label);
	for (size_t i = 0; i < anses->size(); ++i) {
		std::string ans = "";
		for (size_t j = 0; j < anses->at(i).size(); ++j) {
			ans += anses->at(i)[j];
			if (j != anses->at(i).size() - 1) ans += " v ";
		}
		label = new sf::Text();
		label->setFont(font);
		label->setString(ans);
		label->setFillColor(sf::Color::White);
		label->setCharacterSize(14);
		label->setPosition(10, 10 + 18 * (i + 1));
		label->setLineSpacing(0);
		label->setOutlineThickness(0);
		text.push_back(label);
	}

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: {
				window.close();
				break;
			}
			}
		}

		window.clear(sf::Color::Black);
		for (auto& i : text) {
			window.draw(*i);
		}
		window.display();
	}
}