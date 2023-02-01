#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

namespace SFML {
	class AnswersWindow {
	public:
		AnswersWindow(std::vector<std::vector<std::string>>*);
	};
}