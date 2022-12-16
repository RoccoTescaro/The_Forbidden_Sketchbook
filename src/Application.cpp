#include "../hdr/Application.h"
#include "../hdr/State.h"

void Application::run()
{
	sf::Clock clock;
	float dt = 0.0001f;

	std::unique_ptr<State>& state = app.states[app.currentStateIndex];
	sf::Event ev{};

	while (app.window.isOpen())
	{
		ASSERT("states got destroyed", state);

		//#TODO add input update
		state->update();

		app.window.clear();
		state->render();
		app.window.display();

		dt = fmax(clock.getElapsedTime().asSeconds(), 0.0001f);
		clock.restart();
	}
};

Application::Application() 
	: window(sf::VideoMode(Config::windowDim.x, Config::windowDim.y), "", sf::Style::Default)
{
	window.setFramerateLimit(Config::fps);
	//#TODO add states
};
