#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "VignetteEffect.h"

class State 
{
public:
	State() : window(Application::getWindow()), dt(Application::getDeltaTime()), input(Application::getInput()) {};

	virtual ~State() = default;

	virtual void update() { if (input.resizeEvent()) onResize(); };
	virtual void render() = 0;

	virtual void onResize() {};
protected:
	sf::RenderWindow& window;
	const float& dt;

	Input& input;

	VignetteEffect transitionEffect;
};
