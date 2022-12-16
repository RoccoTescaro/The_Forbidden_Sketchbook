#include "../hdr/Application.h"
#include "SFML/Window.hpp"
#include "../hdr/State.h"

Application::~Application()
{
	for (auto& state : app.states)
		delete state;
}


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

		app.window.clear();
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
	//#TODO add states
};

Application Application::app{};