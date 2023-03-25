#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "VignetteEffect.h"

class State 
{
public:
	State() : window(Application::get().getWindow()), dt(Application::get().getDeltaTime()), input(Application::get().getInput()) {};

	virtual ~State() = default;

	virtual void update() = 0;
	virtual void render() = 0;

protected:
	sf::RenderWindow& window;
	const float& dt;

	Input& input;

	VignetteEffect transitionEffect;
};

