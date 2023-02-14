#pragma once
#include <SFML/Graphics.hpp>

static std::string path(const std::string& path)
{
	std::string prefix = "../";
	return prefix + path;
};

#define VSC 0
#if VSC
#define PATH(string) path(string) 
#else
#define PATH(string) string 
#endif

namespace Config
{
	const bool debugMode = true;
	const sf::Vector2<int> windowDim{ 1290,720 };
	const uint8_t fps{ 60 };
	const float eps{ 0.0005f };
	const float maxActivationDistance{2048};

	const std::string titleFontPath{ PATH("fnt/TitleFont.ttf") };
	const std::string buttonFontPath{ PATH("fnt/ButtonFont.ttf") };
	const std::string barFontPath{ PATH("fnt/BarFont.ttf") };
	const std::string dialogueFontPath{ PATH("fnt/DialogueFont.ttf") };

	const std::string menuBackgroundTexturePath{ PATH("img/MenuBackground.jpg") };
	const std::string gameBackgroundTexturePath{ PATH("img/GameBackground.jpg") };
	const std::string playerTexturePath{ PATH("img/Player.png") };
	const std::string healthBarTexturePath{ PATH("img/HealthBar.png") };
	const std::string healthBarMaskTexturePath{ PATH("img/HealthBarMask.png") };
	const std::string energyBarTexturePath{ PATH("img/EnergyBar.png") };
	const std::string energyBarMaskTexturePath{ PATH("img/EnergyBarMask.png") };
	const std::string dialogueTexturePath{ PATH("img/Dialogue.png") };

	const std::string vignetteEffectShaderPath{ PATH("shd/VignetteEffect.frag") };
	const std::string filterShaderPath{ PATH("shd/Filter.frag") };
	const std::string barShaderPath{ PATH("shd/Bar.frag") };
	const std::string backgroundShaderPath{ PATH("shd/Background.frag") };

	const uint8_t nTypingSounds = 5;
	const std::string typingSoundPathBuffer[nTypingSounds]{ 
		PATH("aud/Typing1.wav"),
		PATH("aud/Typing2.wav"),
		PATH("aud/Typing3.wav"),
		PATH("aud/Typing4.wav"),
		PATH("aud/Typing5.wav")
	};

	const std::string gameMapPath{ PATH("test.tfs") };
};









