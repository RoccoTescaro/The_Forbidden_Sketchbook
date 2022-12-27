#pragma once
#include <SFML/Graphics.hpp>

//#define PATH(path) "../"##path

namespace Config
{
	const bool debugMode = true;
	const sf::Vector2<int> windowDim{ 1290,720 };
	const uint8_t fps{ 60 };
	const float eps{ 0.0005f };

	const std::string titleFontPath{ "../fnt/titleFont2.ttf" };
	const std::string buttonFontPath{ "../fnt/buttonFont1.ttf" };

	const std::string backgroundTexturePath{ "../img/paperBackground.jpg" };
	const std::string playerTexturePath{ "../img/Player.png" };
	
	const std::string vignetteEffectShaderPath{ "../shd/VignetteEffect.frag" };
	const std::string filterShaderPath{ "../shaders/Filter.frag" };

};









