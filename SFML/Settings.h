#pragma once

#include <xutility>
#include <Windows.h>

class Settings {
private:
    int _window_width, _window_height;
    int _frame_limit;
private:
    std::pair<int, int> _get_screen_size();
public:
    Settings();

    void set_to_screen_size();
    void set_custom_size();

    int get_window_width();
    int get_window_height();
    int get_frame_limit();
};