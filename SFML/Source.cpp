#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include <thread>
#include <mutex>
#include "Settings.h"
#include "SFMLElements.h"

SFML::Elements sfml_elements;
sf::RenderWindow window(sf::VideoMode(sfml_elements.sfml_settings.get_window_width(),
    sfml_elements.sfml_settings.get_window_height()), "Minimal disjunctive normal form generator", sf::Style::None);

sf::Cursor curs;

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
int main() {
    sfml_elements.Initialize(window);

    std::pair<int, int> prev_pos;
    bool is_pressed = false;
    int new_cursor = 0, prev_cursor = 0;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            sfml_elements.HandleText(event, window);

            switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::MouseButtonPressed: {
                if (sfml_elements.CheckPressed(window)) break;
                if (event.mouseButton.button == sf::Mouse::Left && sf::Mouse::getPosition(window).y < 30) {
                    auto mb = event.mouseButton;
                    if (!is_pressed) {
                        prev_pos = std::make_pair(mb.x, mb.y);
                        is_pressed = true;
                    }
                }
                break;
            }
            case sf::Event::MouseMoved: {
                if (sfml_elements.CheckHowered(window)) new_cursor = 1;
                else new_cursor = 0;

                auto mm = event.mouseMove;
                if (is_pressed) {
                    std::pair<int, int> diff = {
                        prev_pos.first - mm.x,
                        prev_pos.second - mm.y
                    };
                    window.setPosition(sf::Vector2i(window.getPosition().x - diff.first, window.getPosition().y - diff.second));
                }
                break;
            }
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sfml_elements.CheckReleased(window);
                    if (is_pressed)
                        is_pressed = false;
                }
                
                break;
            }
            }
        }

        if (new_cursor != prev_cursor) {
            if (new_cursor == 0) {
                curs.loadFromSystem(curs.Arrow);
            } else if (new_cursor == 1) {
                curs.loadFromSystem(curs.Hand);
            } else if (new_cursor == 2) {
                curs.loadFromSystem(curs.Text);
            }

            window.setMouseCursor(curs);
            prev_cursor = new_cursor;
        }

        window.clear(sf::Color(40, 44, 52));
        sfml_elements.Draw(window);
        window.display();
    }

    return 0;
}