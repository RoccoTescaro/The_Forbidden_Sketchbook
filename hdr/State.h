#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "VignetteEffect.h"

class State 
{
public:
	State() : window(Application::getWindow()), dt(Application::getDeltaTime()), input(Application::getInput()) {};

	virtual ~State() = default;

	virtual void update() 
	{ 
		if (input.resizeEvent()) 
			onResize(); 
	};

	virtual void render() = 0;

	virtual void onResize()
	{
		//static int i = 1;
		//LOG("onResize call number: {1}", i++);
		//LOG("windowSize: {{1},{2}} ", window.getSize().x, window.getSize().y);
		window.setView(sf::View{ sf::Vector2f{window.getSize()}*0.5f, sf::Vector2f{window.getSize()} });
		//LOG("viewSize: {{1},{2}} ", window.getView().getSize().x, window.getView().getSize().y);
		transitionEffect.onResize();
	};
protected:
	sf::RenderWindow& window;
	const float& dt;

	Input& input;

	VignetteEffect transitionEffect;
};
