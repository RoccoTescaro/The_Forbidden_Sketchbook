#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"

class State 
{
public:
	State() : window(Application::getWindow()), dt(Application::getDeltaTime()) {};

	virtual ~State() = default;

	virtual void update() = 0;
	virtual void render() = 0;

private:
	sf::RenderWindow& window;
	const float& dt;
};
