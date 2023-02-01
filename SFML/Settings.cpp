#include "Settings.h"

Settings::Settings() {
    set_custom_size();
    _frame_limit = 144;
}

std::pair<int, int> Settings::_get_screen_size() {
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    return std::make_pair(cx, cy);
}

void Settings::set_to_screen_size() {
    std::pair<int, int> metrics = _get_screen_size();
    _window_width = metrics.first;
    _window_height = metrics.second;
}
void Settings::set_custom_size() {
    _window_width = 1790;
    _window_height = 886;
}

int Settings::get_frame_limit() {
    return _frame_limit;
}
int Settings::get_window_width() {
    return _window_width;
}
int Settings::get_window_height() {
    return _window_height;
}