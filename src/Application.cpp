#include "../hdr/Application.h"
#include "SFML/Window.hpp"
#include "../hdr/State.h"
#include "../hdr/Menu.h"
#include "../hdr/Editor.h"
#include "../hdr/Game.h"
#include "../hdr/Pause.h"

State* Application::getState(uint8_t index)
{
	return app.states[index].get();
}

State* Application::getPrevState()
{
	return app.states[app.previousStateIndex].get();
}

void Application::addState(State* state)
{
	app.states.emplace_back(state);
}

void Application::run()
{
	//TODO add all the states
	//its important that all the states are appended here and not in the Costructor
	//couse most of the code is based on config, which are static variables, and the costructor
	//of the singleton is called aswell as a static variable. Static variable are declare and define
	//in a non deterministic way and we must ensure that config variables are declared before the 
	//istanciation of the singleton

	app.window.reset(new sf::RenderWindow(sf::VideoMode(Config::windowDim.x, Config::windowDim.y), "", sf::Style::Close));
	app.states.emplace_back(new Menu);
	LOG("menu generated");
	app.states.emplace_back(new Game);
	LOG("game generated");
	app.states.emplace_back(new Editor);
	LOG("Editor generated");
	app.states.emplace_back(new Pause);
	LOG("Pause generated");
	app.window->setFramerateLimit(Config::fps);

	sf::Clock clock;
	app.dt = Config::eps;

	sf::Event ev{};

	while (app.window->isOpen())
	{
		State& state = *Application::getState(app.currentStateIndex);
		app.input.update();
		state.update();

		app.window->clear(sf::Color(255,255,255,255));
		state.render();
		app.window->display();

		app.dt = std::max(clock.getElapsedTime().asSeconds(), Config::eps);
		clock.restart();
	}
};

Application::Application() 
{
	states.reserve(0);
};

Application Application::app{};