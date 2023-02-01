#pragma once

#include "MinimalDisjunctive.h"
#include <xutility>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Button.h"
#include "Textbox.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include "AnswersWindow.h"

namespace SFML {
	class Elements {
	private:
        sf::Font font1, font2;

        sf::Texture minimize_button_default;
        sf::Texture minimize_button_activated;

        sf::Texture maximize_button_default;

        sf::Texture close_button_default;
        sf::Texture close_button_activated;

        sf::Texture next_step_button_default;
        sf::Texture next_step_button_activated;

        sf::Texture one_button_default;
        sf::Texture one_button_activated;

        sf::Texture zero_button_default;
        sf::Texture zero_button_activated;

        sf::Texture func_texture;

        Button minimize_button, maximize_button, close_button, next_step_button;
        sf::Sprite f_sprite;
        sf::Text logo_text, count_text, func_number_text, tips_text;
        TextBox *count_box, *func_number_box;
        std::vector<sf::RectangleShape*> lines;
        std::vector<sf::Text*> labels;
        std::vector<Button*> nums_buttons;

        int current_state = 0;
        unsigned long long prev_count = 0, prev_func = 0;
        min_disjunctive* md;
	public:
        Settings sfml_settings;

        Elements();

        void InitializeTextures();
        void Initialize(sf::RenderWindow&);

        bool CheckHowered(sf::RenderWindow&);
        void CheckReleased(sf::RenderWindow&);
        bool CheckPressed(sf::RenderWindow&);
        void HandleText(sf::Event&, sf::RenderWindow&);

        void Draw(sf::RenderWindow&); 
	};
}