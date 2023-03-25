#include "../hdr/Application.h"
#include "../hdr/State.h"
#include "../hdr/Menu.h"
#include "../hdr/Editor.h"
#include "../hdr/Game.h"
#include "../hdr/Pause.h"

State* Application::getState(Index index)
{
	return app.states[index].get();
}

State* Application::getCurrentState()
{
	return app.states[app.currentStateIndex].get();
}

State* Application::getPrevState()
{
	return app.states[app.previousStateIndex].get();
}

void Application::run()
{
	app.window.reset(new sf::RenderWindow(sf::VideoMode(Config::windowDim.x, Config::windowDim.y), "", sf::Style::Close));
	app.window->setFramerateLimit(Config::fps);
	app.window->setMouseCursorVisible(false);
	
	//its important that all the states are appended here and not in the costructor
	//couse most of the code is based on config, which are static variables, and the costructor
	//of the singleton is called aswell as a static variable. Static variable are declare and define
	//in a non deterministic way and we must ensure that config variables are declared before the 
	//istanciation of the singleton

	app.states[MENU] = static_cast<std::unique_ptr<State>>(new Menu);
	app.states[GAME] = static_cast<std::unique_ptr<State>>(new Game);
	app.states[EDITOR] = static_cast<std::unique_ptr<State>>(new Editor);
	app.states[PAUSE] = static_cast<std::unique_ptr<State>>(new Pause);
	
	//MAIN LOOP
	sf::Clock clock;
	app.dt = Config::eps;
	sf::Event ev{};

	while (app.window->isOpen())
	{
		State& state = *app.getState(app.currentStateIndex);
		app.input.update();
		state.update();

		app.window->clear();
		state.render();
		app.window->display();

		app.dt = std::max(clock.getElapsedTime().asSeconds(), Config::eps);
		clock.restart();
	}
};

Application Application::app{};