#pragma once
#include <SFML/Graphics.hpp>

namespace Config //#TODO return to namespace after the Application::run call is put in main
{
	const sf::Vector2<int> windowDim{ 1290,720 };
	const uint8_t fps{ 60 };
	const float eps{ 0.0005f };

	const std::string backgroundTexturePath{ "img/paperBackground.jpg" };
	const std::string titleFontPath{ "fnt/titleFont2.ttf" };
	const std::string buttonFontPath{ "fnt/buttonFont1.ttf" };
};









