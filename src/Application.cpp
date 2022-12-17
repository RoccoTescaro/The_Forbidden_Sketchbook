#include "../hdr/Application.h"
#include "SFML/Window.hpp"
#include "../hdr/State.h"
#include "../hdr/Menu.h"

State& Application::getState(uint8_t index)
{
	return *app.states[index];
}


void Application::addState(State* state)
{
	app.states.emplace_back(state);
}


void Application::run()
{
	sf::Clock clock;
	float dt = 0.0001f;

	State& state = Application::getState(app.currentStateIndex);
	sf::Event ev{};

	while (app.window.isOpen())
	{
		ASSERT("states got destroyed", state);

		app.input.update();
		state.update();

		app.window.clear(sf::Color(255,255,255,255));
		state.render();
		app.window.display();

		dt = fmax(clock.getElapsedTime().asSeconds(), 0.0001f);
		clock.restart();
	}
};

Application::Application() 
	: window(sf::VideoMode(Config::windowDim.x, Config::windowDim.y), "", sf::Style::Default)
{
	window.setFramerateLimit(Config::fps);
	states.reserve(0);
	states.emplace_back(new Menu);

	run(); //we could call run directly from here, with the constuctor of the singleton or from the main function
};

Application Application::app{};