#pragma once
#include <SFML/Graphics.hpp>

#define PATH(path) "../"##path

namespace Config
{
	const bool debugMode = true;
	const sf::Vector2<int> windowDim{ 1290,720 };
	const uint8_t fps{ 60 };
	const float eps{ 0.0005f };

	const std::string titleFontPath{ PATH("fnt/titleFont2.ttf") };
	const std::string buttonFontPath{ PATH("fnt/buttonFont1.ttf") };

	const std::string backgroundTexturePath{ PATH("img/paperBackground.jpg") };
	const std::string playerTexturePath{ PATH("img/Player.png") };

	const std::string vignetteEffectShaderPath{ PATH("shd/VignetteEffect.frag") };
	const std::string filterShaderPath{ PATH("shd/Filter.frag") };
	const std::string barShaderPath{ PATH("shd/Bar.frag") };

	const uint8_t nTypingSounds = 5;
	const std::string typingSoundPathBuffer[nTypingSounds]{ 
		PATH("aud/typing1.wav"),
		PATH("aud/typing2.wav"),
		PATH("aud/typing3.wav"),
		PATH("aud/typing4.wav"),
		PATH("aud/typing5.wav")
	};
};









