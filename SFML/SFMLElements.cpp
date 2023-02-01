#include "SFMLElements.h"

SFML::Elements::Elements() {
    md = new min_disjunctive(0, 0);
}

void SFML::Elements::InitializeTextures() {
    minimize_button_default.loadFromFile("img\\minimize_button\\default.png");
    minimize_button_activated.loadFromFile("img\\minimize_button\\activated.png");

    maximize_button_default.loadFromFile("img\\maximize_button\\default.png");

    close_button_default.loadFromFile("img\\close_button\\default.png");
    close_button_activated.loadFromFile("img\\close_button\\activated.png");

    next_step_button_default.loadFromFile("img\\next_step_button\\default.png");
    next_step_button_activated.loadFromFile("img\\next_step_button\\activated.png");

    one_button_default.loadFromFile("img\\1_button\\default.png");
    one_button_activated.loadFromFile("img\\1_button\\activated.png");

    zero_button_default.loadFromFile("img\\0_button\\default.png");
    zero_button_activated.loadFromFile("img\\0_button\\activated.png");

    func_texture.loadFromFile("img\\f.png");
}

void SFML::Elements::Initialize(sf::RenderWindow& window) {
    window.setFramerateLimit(sfml_settings.get_frame_limit());

    font1.loadFromFile("Consolas.ttf");
    font2.loadFromFile("Calibri.ttf");

    InitializeTextures();
    minimize_button = Button({ 1, 1 }, minimize_button_default, minimize_button_activated, minimize_button_activated);
    minimize_button.setPosition(sf::Vector2f(sfml_settings.get_window_width() - 138, 0));
    minimize_button.setTexture(minimize_button_default);

    maximize_button = Button({ 1, 1 }, maximize_button_default, maximize_button_default, maximize_button_default);
    maximize_button.setPosition(sf::Vector2f(sfml_settings.get_window_width() - 92, 0));
    maximize_button.setTexture(maximize_button_default);

    close_button = Button({ 1, 1 }, close_button_default, close_button_activated, close_button_activated);
    close_button.setPosition(sf::Vector2f(sfml_settings.get_window_width() - 46, 0));
    close_button.setTexture(close_button_default);

    next_step_button = Button({ 1, 1 }, next_step_button_default, next_step_button_activated, next_step_button_activated);
    next_step_button.setPosition(sf::Vector2f(15, 110));
    next_step_button.setTexture(next_step_button_default);

    logo_text.setFont(font1);
    logo_text.setString("Minimal disjunctive normal form generator - by irval1337.github.io");
    logo_text.setCharacterSize(15);
    logo_text.setFillColor(sf::Color(204, 204, 204));
    logo_text.setPosition(sfml_settings.get_window_width() / 2 - 295, 3);

    count_text.setFont(font1);
    count_text.setString("Variables count:");
    count_text.setCharacterSize(16);
    count_text.setFillColor(sf::Color::White);
    count_text.setPosition(15, 45);

    func_number_text.setFont(font1);
    func_number_text.setString("Build a minimising map");
    func_number_text.setCharacterSize(16);
    func_number_text.setFillColor(sf::Color::White);
    func_number_text.setPosition(150, 110);

    tips_text.setFont(font1);
    tips_text.setString("Function number:");
    tips_text.setCharacterSize(16);
    tips_text.setFillColor(sf::Color::White);
    tips_text.setPosition(15, 75);

    TextBox* text_area = new TextBox(font2, font1);
    text_area->setPosition(150, 45);
    text_area->setSize(100, 18);
    text_area->setBorder(0);
    text_area->setText("0");
    count_box = text_area;

    text_area = new TextBox(font2, font1);
    text_area->setPosition(150, 75);
    text_area->setSize(100, 18);
    text_area->setBorder(0);
    text_area->setText("0");
    func_number_box = text_area;

    f_sprite.setTexture(func_texture);
    f_sprite.setPosition({ 15, 150 });
}

bool SFML::Elements::CheckHowered(sf::RenderWindow& window) {
    bool val = false;
    for (auto& i : nums_buttons) {
        val |= i->autoSetTexture(window);
    }
    val |= minimize_button.autoSetTexture(window);
    val |= close_button.autoSetTexture(window);
    val |= next_step_button.autoSetTexture(window);
    return val;
}

void SFML::Elements::CheckReleased(sf::RenderWindow& window) {
    if (minimize_button.isMouseOver(window)) {
        ::SendNotifyMessage(window.getSystemHandle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
    } else if (close_button.isMouseOver(window)) {
        window.close();
    } else if (next_step_button.isMouseOver(window)) {
        unsigned long long curr_count = std::stoull(count_box->getCurrentText().toAnsiString());
        unsigned long long curr_func = std::stoull(func_number_box->getCurrentText().toAnsiString());
        if (curr_count > 5) {
            MessageBoxA(
                NULL,
                "The number of variables must be less than 6",
                "Incorrect input",
                MB_OK |
                MB_DEFBUTTON1 |
                MB_ICONEXCLAMATION |
                MB_DEFAULT_DESKTOP_ONLY
            );
            return;
        }
        std::string max_f = "The function number must be less than " + std::to_string(1ULL << (1ULL << curr_count));
        if (curr_func >= (1ULL << (1ULL << curr_count))) {
            MessageBoxA(
                NULL,
                max_f.c_str(),
                "Incorrect input",
                MB_OK |
                MB_DEFBUTTON1 |
                MB_ICONEXCLAMATION |
                MB_DEFAULT_DESKTOP_ONLY
            );
            return;
        }

        if (curr_count != prev_count || curr_func != prev_func) {
            prev_count = curr_count;
            prev_func = curr_func;
            current_state = 0;
            for (auto& i : labels) {
                delete i;
            }
            labels.clear();
            for (auto& i : lines) {
                delete i;
            }
            lines.clear();
            for (auto& i : nums_buttons) {
                delete i;
            }
            nums_buttons.clear();
            if (curr_count == 0) {
                current_state = 5;
                return;
            }
            md = new min_disjunctive(curr_func, curr_count);
            func_number_text.setString("Build a minimising map");
        }
        if (curr_count == 0) {
            return;
        }
        ++current_state;
        current_state = std::min(5, current_state);
        switch (current_state) {
        case 1: {
            for (size_t i = 0; i < (1 << curr_count); ++i) {
                if (md->get_bit(i) == 0) {
                    Button* btn = new Button({ 1, 1 }, zero_button_default, zero_button_activated, zero_button_activated);
                    btn->setPosition(sf::Vector2f(15, 150 + 22 * (i + 1)));
                    btn->setTexture(zero_button_default);
                    nums_buttons.push_back(btn);
                } else {
                    Button* btn = new Button({ 1, 1 }, one_button_default, one_button_activated, one_button_activated);
                    btn->setPosition(sf::Vector2f(15, 150 + 22 * (i + 1)));
                    btn->setTexture(one_button_default);
                    nums_buttons.push_back(btn);
                }
            }
            int width = 0;
            for (size_t i = 1; i <= (1 << curr_count); ++i) {
                if (i != 1) {
                    width += 55;
                }
                sf::RectangleShape* line = new sf::RectangleShape();
                line->setSize(sf::Vector2f(1ULL, 22ULL * (1ULL << curr_count) + 22ULL));
                line->setPosition(sf::Vector2f(15ULL + 22ULL + width, 150ULL));
                line->setFillColor(sf::Color::White);
                lines.push_back(line);

                line = new sf::RectangleShape();
                line->setSize(sf::Vector2f(55 * (1 << curr_count) - 55, 1ULL));
                line->setPosition(sf::Vector2f(15ULL + 22ULL, 150ULL + 22ULL * i - 1));
                line->setFillColor(sf::Color::White);
                lines.push_back(line);

                if (i == (1 << curr_count)) {
                    line = new sf::RectangleShape();
                    line->setSize(sf::Vector2f(55 * (1 << curr_count) - 55, 1ULL));
                    line->setPosition(sf::Vector2f(15ULL + 22ULL, 150ULL + 22ULL * i + 22ULL - 1));
                    line->setFillColor(sf::Color::White);
                    lines.push_back(line);
                }

                if (i == (1 << curr_count)) continue;
                sf::Text* txt = new sf::Text();
                txt->setFont(font1);
                txt->setString(md->get_permutation_name(i, curr_count));
                txt->setCharacterSize(16);
                txt->setFillColor(sf::Color::White);
                sf::FloatRect textRect = txt->getLocalBounds();
                txt->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                txt->setPosition(15ULL + 22ULL + width + 55 / 2, 162);
                labels.push_back(txt);
            }

            md->generate_table();
            std::vector<std::vector<disjunctive_component*>> comps = md->get_components();
            for (long long current_value = 0; current_value < (1 << curr_count); ++current_value) {
                for (size_t i = 1; i < (1 << curr_count); ++i) {
                    sf::Text* txt = new sf::Text();
                    txt->setFont(font1);
                    txt->setString(comps[current_value][i - 1]->get_print_string());
                    txt->setCharacterSize(16);
                    txt->setFillColor(sf::Color::White);
                    sf::FloatRect textRect = txt->getLocalBounds();
                    txt->setOrigin(static_cast<int>(textRect.left + textRect.width / 2.0f), static_cast<int>(textRect.top + textRect.height / 2.0f));
                    txt->setPosition(15ULL + 22ULL + (i - 1) * 55 + 55 / 2, 162 + 22 * current_value + 22);
                    labels.push_back(txt);
                }
            }
            func_number_text.setString("Delete null lines");
            break;
        }
        case 2: {
            md->remove_zeros();
            std::vector<std::vector<disjunctive_component*>> comps = md->get_components();
            for (long long current_value = 0; current_value < (1 << curr_count); ++current_value) {
                if (comps[current_value][0]->get_used() != 1) continue;

                sf::RectangleShape* line = new sf::RectangleShape();
                line->setSize(sf::Vector2f(55 * (1 << curr_count) - 55, 1ULL));
                line->setPosition(sf::Vector2f(15ULL + 22ULL, 150ULL + 22ULL * current_value - 1 + 33ULL));
                line->setFillColor(sf::Color::Red);
                lines.push_back(line);
            }
            func_number_text.setString("Delete matching cells");
            break;
        }
        case 3: {
            md->remove_same();
            std::vector<std::vector<disjunctive_component*>> comps = md->get_components();
            for (long long current_value = 0; current_value < (1ULL << curr_count); ++current_value) {
                for (size_t i = 1; i < (1ULL << curr_count); ++i) {
                    if (comps[current_value][i - 1]->get_used() != 2) continue;

                    sf::RectangleShape* line = new sf::RectangleShape();
                    line->setSize(sf::Vector2f(55, 1ULL));
                    line->setPosition(sf::Vector2f(15ULL + 22ULL + (i - 1) * 55, 150ULL + 22ULL * current_value - 1 + 33ULL));
                    line->setFillColor(sf::Color::Green);
                    lines.push_back(line);
                }
            }
            func_number_text.setString("Absorb occurrences");
            break;
        }
        case 4: {
            md->remove_contains();
            std::vector<std::vector<disjunctive_component*>> comps = md->get_components();
            for (long long current_value = 0; current_value < (1ULL << curr_count); ++current_value) {
                for (size_t i = 1; i < (1ULL << curr_count); ++i) {
                    if (comps[current_value][i - 1]->get_used() != 3) continue;

                    sf::RectangleShape* line = new sf::RectangleShape();
                    line->setSize(sf::Vector2f(56, 1ULL));
                    line->setPosition(sf::Vector2f(15ULL + 24ULL + (i - 1) * 55, 150ULL + 22ULL * current_value - 1 + 22ULL));
                    line->setFillColor(sf::Color::Cyan);
                    line->setRotation(atan(22.0 / 55.0) * 180.0 / acos(-1.0));
                    lines.push_back(line);
                }
            }
            func_number_text.setString("Generate an answer");
            break;
        }
        case 5: {
            md->find_answers();
            std::vector<std::vector<std::string>> anses = md->get_answers();
            AnswersWindow answers_window(&anses);
            break;
        }
        }

    } else {
        size_t ind = 0;
        for (auto& i : nums_buttons) {
            if (i->isMouseOver(window)) {
                md->set_bit(ind, !md->get_bit(ind));
                delete i;

                if (md->get_bit(ind) == 0) {
                    i = new Button({ 1, 1 }, zero_button_default, zero_button_activated, zero_button_activated);
                    i->setPosition(sf::Vector2f(15, 150 + 22 * (ind + 1)));
                    i->setTexture(zero_button_default);
                } else {
                    i = new Button({ 1, 1 }, one_button_default, one_button_activated, one_button_activated);
                    i->setPosition(sf::Vector2f(15, 150 + 22 * (ind + 1)));
                    i->setTexture(one_button_default);
                }

                func_number_box->popText(func_number_box->getCurrentText().getSize());
                func_number_box->setText(std::to_string(md->get_function_number()));
                current_state = 0;
                func_number_text.setString("Build a minimising map");
                break;
            }
            ++ind;
        }
    }
}

bool SFML::Elements::CheckPressed(sf::RenderWindow& window) {
    bool buttons = false;
    for (auto& i : nums_buttons) {
        buttons |= i->isMouseOver(window);
    }
    return minimize_button.isMouseOver(window) || maximize_button.isMouseOver(window) ||
        close_button.isMouseOver(window) || next_step_button.isMouseOver(window) || buttons;
}

void SFML::Elements::HandleText(sf::Event& event, sf::RenderWindow& window) {
    if (CheckHowered(window)) return;

    count_box->handleEvent(event);
    func_number_box->handleEvent(event);
}

void SFML::Elements::Draw(sf::RenderWindow& window) {
    minimize_button.drawTo(window);
    maximize_button.drawTo(window);
    close_button.drawTo(window);
    window.draw(logo_text);
    window.draw(count_text);
    window.draw(func_number_text);
    window.draw(f_sprite);
    window.draw(tips_text);
    count_box->draw(window);
    func_number_box->draw(window);
    next_step_button.drawTo(window);
    for (auto& i : labels) {
        window.draw(*i);
    }
    for (auto& i : lines) {
        window.draw(*i);
    }
    for (auto& i : nums_buttons) {
        i->drawTo(window);
    }
}